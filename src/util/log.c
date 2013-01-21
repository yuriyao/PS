
/**
 *  Ps的log
 **/
#include "../include/log.h"
#include <stdarg.h>
#include <assert.h>

#if LOG_LOCK_ENABLE
static PyThread_type_lock log_lock;
#endif

/*static*/ FILE *log_normal;
/*static*/ FILE *log_waring;
/*static*/ FILE *log_error;

BOOL 
Ps_InitLog(FILE *normal, FILE *error, FILE *waring)
{
	static int init = 0;

	//只能初始化一次
	if(init)
		return TRUE;
	init = 1;
#if LOG_LOCK_ENABLE
	log_lock = PyThread_allocate_lock();
	if(!log_lock)
	{
		return FALSE;
	}
#endif
	GET_LOG_LOCK();
	log_normal = normal ? normal : stdout;
	log_error = error ? error : stderr;
	log_waring = waring ? waring : stdout;
	RELEASE_LOG_LOCK();
	assert(log_waring);
	assert(log_error);
	assert(log_normal);
	return TRUE;
}

BOOL 
Ps_Log(char *info, int type)
{
	FILE *file = NULL;
	if(!info)
		return FALSE;
	//assert(log_waring);
	//assert(log_error);
	//assert(log_normal);
	GET_LOG_LOCK();
	//printf("%d\n", type);
	switch(type)
	{
	case Ps_LOG_WARING:
		file = log_waring;
		break;
	case Ps_LOG_ERROR:
		file = log_error;
		break;
	default:
		file = log_normal;
		break;
	}
	assert(file);
	if(fputs(info, file) < 0)
	{
		RELEASE_LOG_LOCK();
		return FALSE;
	}
	RELEASE_LOG_LOCK();
	return TRUE;
}

BOOL 
Ps_LogFormat(char *format, int type, ...)
{
	FILE *file = NULL;
	va_list ap;
	if(!format)
		return FALSE;
	GET_LOG_LOCK();
	switch(type)
	{
	case Ps_LOG_WARING:
		file = log_waring;
		break;
	case Ps_LOG_ERROR:
		file = log_error;
		break;
	default:
		file = log_normal;
		break;
	}
	assert(file);
	va_start(ap, type);
	if(vfprintf(file, format, ap) < 0)
	{
		RELEASE_LOG_LOCK();
		return FALSE;
	}
	RELEASE_LOG_LOCK();
	return TRUE;
}

BOOL 
Ps_LogFile(char *info, int type, FILE *file)
{
	FILE *_file;

	if(!info)
		return FALSE;
	GET_LOG_LOCK();
	switch(type)
	{
	case Ps_LOG_WARING:
		_file = log_waring;
		break;
	case Ps_LOG_ERROR:
		_file = log_error;
		break;
	default:
		_file = log_normal;
		break;
	}
	_file = file ? file : _file;
	assert(_file);
	if(fputs(info, _file) < 0)
	{
		RELEASE_LOG_LOCK();
		return FALSE;
	}
	RELEASE_LOG_LOCK();
	return TRUE;
}

BOOL 
Ps_LogFileFormat(char *format, int type, FILE *file, ...)
{
	FILE *_file;
	va_list ap;

	if(!format)
		return FALSE;
	GET_LOG_LOCK();
	switch(type)
	{
	case Ps_LOG_WARING:
		_file = log_waring;
		break;
	case Ps_LOG_ERROR:
		_file = log_error;
		break;
	default:
		_file = log_normal;
		break;
	}
	_file = file ? file : _file;
	assert(_file);
	va_start(ap, file);
	if(vfprintf(_file, format, ap) < 0)
	{
		RELEASE_LOG_LOCK();
		return FALSE;
	}
	RELEASE_LOG_LOCK();
	return TRUE;
}

BOOL 
Ps_LogObject(PyObject *object, int _type)
{
	FILE *file;
	PyTypeObject *type;
	PyObject *tmp;
	if(object == NULL)
		return FALSE;
	type = object->ob_type;

	GET_LOG_LOCK();
	switch(_type)
	{
	case Ps_LOG_ERROR:
		file = log_error;
		break;
	case Ps_LOG_WARING:
		file = log_waring;
		break;
	default:
		file = log_normal;
		break; 
	}
	assert(file);
	if(!type)
	{
		fprintf(file, "Object at %p is unknowntype\n", object);
		return FALSE;
	}
	fputs("--------------------\n", file);
	/*打印object的名字*/
	if(type->tp_name)
	{
		fprintf(file, "Object %s info:\n", type->tp_name);
	}
	else
	{
		fprintf(file, "Object at %p info:\n", object);
	}
	/*打印文档信息*/
	if(type->tp_doc)
		fprintf(file, "doc: %s\n", type->tp_doc);
	/*打印内存分配信息*/
	fprintf(file, "base size is %d\n", type->tp_basicsize);
	if(type->tp_itemsize)
	{
		fprintf(file, "Length changable object, Item size is %d\n", type->tp_itemsize);
	}
	else
		fputs("Length unchangable object.\n", file);
	/*repr和str的内容*/
	if(type->tp_repr)
	{
		tmp = type->tp_repr(object);
		if(tmp && PyString_Check(tmp) && PyString_AsString(tmp))
		{
			fprintf(file, "repr:%s\n", PyString_AsString(tmp));
		}
		else
		{
			fprintf(file, "repr:--Waring-- __repr__ donesn't return a PyStringObject\n");
		}
	}
	if(type->tp_str)
	{
		tmp = type->tp_str(object);
		if(tmp && PyString_Check(tmp) && PyString_AsString(tmp))
		{
			fprintf(file, "str:%s\n", PyString_AsString(tmp));
		}
		else
		{
			fprintf(file, "str:--Waring-- __str__ donesn't return a PyStringObject\n");
		}
	}
	/*HASH值*/
	if(type->tp_hash)
	{
		fprintf(file, "hash value: %ld\n", type->tp_hash(object));
	}
	/*是否是可调用的*/
	if(type->tp_call)
	{
		fprintf(file, "It's a callable object\n");
	}
	else
		fputs("It's an uncallable object\n", file);
	/*是否可迭代*/
	if(type->tp_iter && type->tp_iternext)
		fputs("It's a iterable object\n", file);
	else
		fputs("It's an uniterable object\n", file);

	fputs("----------------------\n", file);
	RELEASE_LOG_LOCK();
	return TRUE;
}

BOOL 
Ps_CloseLog()
{
	static int init = 0;
	//只能关闭一次
	if(init)
		return TRUE;
	init = 1;
	//将文件指针置空
	GET_LOG_LOCK();
	log_normal = NULL;
	log_waring = NULL;
	log_error = NULL;
	RELEASE_LOG_LOCK();
//释放锁
#if LOG_LOCK_ENABLE
	PyThread_free_lock(log_lock);
#endif
	return TRUE;
}


#ifdef LOG_DEBUG

#define _SO_TEST

#ifdef _MAIN_TEST
int main(int argc, char **argv)
{
	FILE *file = fopen("log.txt", "w");
	Ps_InitLog(NULL, file, file);
	Ps_Log("Hello waring\n", Ps_LOG_WARING);
	Ps_Log("Hello error\n", Ps_LOG_ERROR);
	Ps_Log("Hello normal\n", Ps_LOG_NORMAL);
	Ps_LogFormat("%s %s %d\n", Ps_LOG_ERROR, "hello", "yuri", 1234);
	Ps_LogFormat("%s %s %d\n", Ps_LOG_WARING, "hello", "yuri", 1234);
	Ps_LogFormat("%s %s %d\n", Ps_LOG_NORMAL, "hello", "yuri", 1234);
	Ps_LogFile("Hello file waring\n", Ps_LOG_WARING, file);
	Ps_LogFile("Hello file error\n", Ps_LOG_ERROR, file);
	Ps_LogFile("Hello file normal\n", Ps_LOG_NORMAL, file);
	Ps_CloseLog();
	fclose(file);
	return 0;
}
#endif

/*测试Ps_LogObject函数*/
#ifdef _SO_TEST
static PyObject*
_Ps_LogObject(PyObject *sel, PyObject *args, PyObject *kwds)
{
	char *kwdlists[] = {"obj", "type", NULL};
	PyObject *obj;
	int type;
	if(!PyArg_ParseTupleAndKeywords(args, kwds, "Oi", kwdlists, &obj, &type))
	{
		PyErr_SetString(PyExc_TypeError, "accept obj and type as arguments");
		return NULL;
	}
	return Py_BuildValue("i", Ps_LogObject(obj, type));
}

static PyMethodDef
log_methods[] = 
{
	{"log", (PyCFunction)_Ps_LogObject, METH_VARARGS | METH_KEYWORDS, "log"},
	{NULL}
};

PyMODINIT_FUNC
initlog(void)
{
	Ps_InitLog(NULL, NULL, NULL);
	Py_InitModule("log", log_methods);

}
#endif

#endif
