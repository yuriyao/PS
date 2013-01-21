/**
 *  实现模块的加载
 * 编译 gcc -fPIC -shared -o test_/psimport.so import_module.c ../../util/log.c ../../lib/mm.c ../../lib/web.c -lpython2.6 -lcurl
 **/
#include <python2.6/Python.h> 
#include <python2.6/pythread.h>
#include <python2.6/pyarena.h>
#include <python2.6/code.h>
#include <python2.6/compile.h>
#include <python2.6/pythonrun.h>
#include <python2.6/pystate.h>
#include <python2.6/Python-ast.h>
#include <python2.6/frameobject.h>
#include "../include/import_module.h"
#include "../../include/web.h"

/*缓存由本模块加载的模块*/
static PyObject *modules_pool; 
static PyThread_type_lock module_lock = NULL;
#define MODULE_LOCK_INIT() PyThread_allocate_lock()
#define GET_MODULE_LOCK() PyThread_acquire_lock(module_lock, WAIT_LOCK)
#define RELEASE_MODULE_LOCK() PyThread_release_lock(module_lock)
#define FREE_MODULE_LOCK() PyThread_free_lock(module_lock)

static unsigned int file_name_no = 0;

/*使用2.6a1的pyc文件*/
#define PYC_MAGIC (62161 | ((long)'\r'<<16) | ((long)'\n'<<24))

BOOL
Ps_ImportInit()
{
	if((modules_pool = PyDict_New()) == NULL)
	{
		Ps_Log("Create pydict failed\n", Ps_LOG_WARING);
		return FALSE;
	}
	if((module_lock = MODULE_LOCK_INIT()) == NULL)
	{
		Ps_Log("Init module lock failed\n", Ps_LOG_WARING);
		return FALSE;
	}
	return TRUE;
}

/*将源文件编译为字节码文件*/
static PyCodeObject*
compile(FILE *fp)
{
	PyCodeObject *co = NULL;
	mod_ty mod;
	PyCompilerFlags flags;
	PyArena *arena = PyArena_New();
	
	if(arena == NULL)
	{
		Ps_Log("Py arena new failed\n", Ps_LOG_WARING);
		return NULL;
	}
	
	flags.cf_flags = 0;
	mod = PyParser_ASTFromFile(fp, "", Py_file_input, 0, 0, &flags, NULL, arena);
	
	if(mod)
	{
		co = PyAST_Compile(mod, "", NULL, arena);
	}
	
	PyArena_Free(arena);
	
	return co;
}

static PyObject*
get_exists(char *name, char *path)
{
	assert(modules_pool);
	PyObject *m = NULL;
	m = PyDict_GetItemString(modules_pool, name);
	return m;
}

static PyObject*
create_module(char *name, PyObject *co)
{
	PyObject *m;
	PyObject *d;
	PyObject *v;
	PyObject *err;
	assert(name);
	assert(co);
	/*创建一个新的模块*/
	m = PyModule_New(name);
	if(m == NULL)
	{
		Ps_Log("create new module failed\n", Ps_LOG_WARING);
		return NULL;
	}
	/*初始化这个新的模块*/
	d = PyModule_GetDict(m);
	if(PyDict_SetItemString(d, "__builtins__", PyEval_GetBuiltins()) != 0)
	{
		Ps_Log("Set new module builtin failed\n", Ps_LOG_WARING);
		PyErr_SetString(PyExc_ImportError, "Cannot get builtin dict");
		goto error;
	}
	v= ((PyCodeObject*)co)->co_filename;
	Py_INCREF(v);
	if(PyDict_SetItemString(d, "__file__", v) != 0)
		PyErr_Clear();
	Py_DECREF(v);
#ifdef IMPORT_DEBUG
	Ps_Log("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n", Ps_LOG_NORMAL);
	//Ps_LogObject(d, Ps_LOG_NORMAL);
	Ps_Log("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n", Ps_LOG_NORMAL);
#endif

	/*执行字节码*/
	v = PyEval_EvalCode((PyCodeObject*)co, d, d);
	if(v == NULL || (err = PyErr_Occurred()))
	{
		Ps_LogObject(d, Ps_LOG_WARING);
		Ps_Log("PyEval_EvalCode failed\n", Ps_LOG_WARING);
		return NULL;
		//goto error;
	}
	Py_XDECREF(v);

	Py_INCREF(m);
	return m;
error:
	Py_DECREF(m);
	return NULL;
}

static PyObject*
__import__(FILE *file, char *name, char *path, char *as, PyObject *local, PyObject *global)
{
	assert(file);
	assert(name);
	PyObject *m;
	PyCodeObject *co = NULL;
	char *module_name = as ? as : name;
	if((m = get_exists(name, path)))
	{
		Py_INCREF(m);
		return m;
	}
	
	co = compile(file);
	//Ps_LogObject((PyObject*)co, Ps_LOG_WARING);
	if(co == NULL)
	{
		Ps_Log("compile failed\n", Ps_LOG_WARING);
		return NULL;
	}
	m = create_module(name, (PyObject*)co);
	if(m == NULL)
	{
		Ps_Log("create_module failed\n", Ps_LOG_WARING);
		return NULL;
	}
	Py_DECREF(co);

	if(local && PyDict_Check(local))
	{
		Py_INCREF(m);
#ifdef IMPORT_DEBUG
		Ps_LogFormat("The module name is %s\n", Ps_LOG_NORMAL, module_name);
		Ps_LogObject(local, Ps_LOG_WARING);
		Ps_LogObject(m, Ps_LOG_WARING);
		int ret = 
#endif
		PyDict_SetItemString(local, module_name, m);
#ifdef IMPORT_DEBUG
		if(ret == 0)
			Ps_LogFormat("ret is %d, Import module %s successfully\n", Ps_LOG_NORMAL, ret, module_name);
		else
			Ps_LogFormat("ret is %d, Import module %s failed\n", Ps_LOG_NORMAL, ret, module_name);
#endif
	}
	else
	{
		PyObject *info = PyString_FromFormat("Import module %s failed", name);
		if(!info)
		{
			PyErr_SetString(PyExc_ImportError, "Import module failed");
		}
		else
			PyErr_SetObject(PyExc_ImportError, info);
		return NULL;
	}
	
	return m;
}

static PyObject*
__import__compiled(FILE *f, char *name, char *path, char *as, PyObject *local, PyObject *global)
{
	char *module_name = as ? as : name;
	PyCodeObject *co;
	PyObject *m;

	if(name == NULL)
		return NULL;
	//比较文件的魔数
	if(PyMarshal_ReadLongFromFile(f) != PYC_MAGIC)
	{
		PyErr_Format(PyExc_ImportError, "Bad magic number of %s", name);
		return NULL;
	}
	//读掉时间信息
	(void*)PyMarshal_ReadLongFromFile(f);
	//创建PyCodeObject
	co = (PyCodeObject*)PyMarshal_ReadLastObjectFromFile(f);
	if(co == NULL)
	{
		PyErr_Format(PyExc_ImportError, "Cannot create code object from module %s", name);
		return NULL;
	}
	if(!PyCode_Check(co))
	{
		PyErr_Format(PyExc_ImportError, "Non-code object in module %s", name);
		Py_DECREF(co);
		return NULL;
	}
	/*创建模块*/
	m = create_module(name, (PyObject*)co);
	if(m == NULL)
	{
		Ps_Log("create_module failed\n", Ps_LOG_WARING);
		return NULL;
	}
	Py_DECREF(co);

	/*将模块导入命名空间*/
	if(local && PyDict_Check(local))
	{
		Py_INCREF(m);
#ifdef IMPORT_DEBUG
		Ps_LogFormat("The module name is %s\n", Ps_LOG_NORMAL, module_name);
		Ps_LogObject(local, Ps_LOG_WARING);
		Ps_LogObject(m, Ps_LOG_WARING);
		int ret = 
#endif
		PyDict_SetItemString(local, module_name, m);
#ifdef IMPORT_DEBUG
		if(ret == 0)
			Ps_LogFormat("ret is %d, Import module %s successfully\n", Ps_LOG_NORMAL, ret, module_name);
		else
			Ps_LogFormat("ret is %d, Import module %s failed\n", Ps_LOG_NORMAL, ret, module_name);
#endif
	}
	else
	{
		PyObject *info = PyString_FromFormat("Import module %s failed", name);
		if(!info)
		{
			PyErr_SetString(PyExc_ImportError, "Import module failed");
		}
		else
			PyErr_SetObject(PyExc_ImportError, info);
		return NULL;
	}
	
	return m;

}

static FILE*
open_file(char *path, char *module)
{
#define MAX_PATH_LEN 1024
	char buff[MAX_PATH_LEN] = {'0'};
	int len = 0;
	FILE *f;
	if(path != NULL)
	{
		len = strlen(path);
		if(len >= MAX_PATH_LEN)
		{
			Ps_Log("Path length is too long\n", Ps_LOG_WARING);
			return NULL;
		}
		strcpy(buff, path);
#if (PLATFORM & LINUX32) || (PLATFORM & LINUX64)
		buff[len] = '/';	
#endif
#if (PLATFORM & WIN32) || (PLATFORM & WIN64)
		buff[len] = '\\';
#endif
		len ++;
	}

	
	if(module && (len + strlen(module)) < MAX_PATH_LEN)
	{
		//printf("%d\n", len);
		strcpy(buff + len, module);
	}
	else
	{
		Ps_LogFormat("Open file from dir %s failed\n", Ps_LOG_WARING, buff);
		return NULL;
	}
#ifdef IMPORT_DEBUG
	len += strlen(module);
	Ps_LogFormat("import module name is %s %s %d\n", Ps_LOG_WARING, buff, module, len);
#endif
	f = fopen(buff, "r");
	return f;
}

static BOOL
get_url(char *website, char *module, BOOL pyc, char *buffer, int len)
{
    /*pyc文件后缀名和'\0'总计长度为4*/
    int total_len = pyc ? 6 : 5;
    if(!website || !module)
    {
        Ps_Log("Both Website and module cannot be NULL\n", Ps_LOG_WARING);
        return FALSE;
    }
    total_len += strlen(website) + strlen(module);

#ifdef IMPORT_DEBUG
    Ps_LogFormat("Total_len is %d and buffer len is %d\n", Ps_LOG_NORMAL, total_len, len);
#endif
    if(total_len > len)
    {
        Ps_Log("The path to website module is too long", Ps_LOG_WARING);
        return FALSE;
    }
    if(pyc)
        sprintf(buffer, "%s/%s.pyc", website, module);
    else
        sprintf(buffer, "%s/%s.py", website, module);
    return TRUE;
}

 /**
  *  完成from...import...
  **/
PyObject* 
Ps_FromImport(char *path, char *module)
{
	FILE *fp;
	PyThreadState *thread = PyThreadState_Get();

}

/**
 *  完成from...import...as...
 **/
PyObject* 
Ps_FromImportAs(char *path, char *module, char *name)
{

}

static BOOL
from_long_get_name(unsigned int name, char *name_buffer, int len)
{
    if(len < sizeof(unsigned int) * 2 + 1)
    {
        Ps_Log("From long get name's name buffer is too short", Ps_LOG_WARING);
        return FALSE;
    }
    sprintf(name_buffer, "%X", name);
    return TRUE;
}

/*调用这个函数前必须加锁*/
static PyObject*
from_web_import_as(char *website, char *module, char *as)
{
#define Ps_NAME_MAX (sizeof(unsigned int) << 1 + 10)
#define WEB_PATH_MAX 1024
    char buffer[WEB_PATH_MAX];
    char file_name[Ps_NAME_MAX];
    /*加载函数的函数指针*/
    PyObject* (*loader)(FILE *f, char *name, char *path, char *as, PyObject *local, PyObject *global);

    FILE *f;
    PyObject *m;

    assert(website);
    assert(module);

#if (PLATFORM & WIN32) || (PLATFORM & WIN64)
    sprintf(file_name, "cache\\%X", file_name_no);
#elif (PLATFORM & LINUX32) || (PLATFORM & LINUX64)
    sprintf(file_name, "cache/%X", file_name_no);
#endif

#ifdef IMPORT_DEBUG
    Ps_LogFormat("Creat temp file %s\n", Ps_LOG_NORMAL, file_name);
    Ps_LogFormat("From %s import %s\n", Ps_LOG_NORMAL, website, module);
#endif

    file_name_no ++;
    if((f = fopen((char*)file_name, "w")) == NULL)
    {
        Ps_Log("Create temp file failed\n", Ps_LOG_WARING);
        PyErr_SetString(PyExc_ImportError, "Create temp file failed");
        return NULL;
    }
    /*下载py文件*/
    if(!get_url(website, module, FALSE, buffer, WEB_PATH_MAX))
    {
       Ps_Log("Get url failed\n", Ps_LOG_WARING);
       PyErr_SetString(PyExc_ImportError, "The path to get python module is too long");
       return NULL;
    }
    loader = __import__;
    if(!Ps_DownloadTimeout(buffer, f, TIME_OUT))
    {
        Ps_Log("Cannot download %s's py file\n", Ps_LOG_WARING);
        /*再次尝试下载pyc文件*/
        //
        (void)get_url(website, module, TRUE, buffer, WEB_PATH_MAX);
        if(!Ps_DownloadTimeout(buffer, f, TIME_OUT))
        {
            Ps_Log("Download pyc file also failed\n", Ps_LOG_WARING);
            PyErr_Format(PyExc_ImportError, "Cannot download module %s file from %s\n", module, website);
            return NULL;
        }
        /*使用加载pyc文件的加载函数*/
        loader = __import__compiled;
    }
    /*加载py文件*/
    fclose(f);
    if((f = fopen(file_name, "r")) == NULL)
    {
    	Ps_Log("Open temp file failed\n", Ps_LOG_WARING);
    	PyErr_SetString(PyExc_ImportError, "open temp file failed");
    	return NULL;
    }
    if(!(m = loader(f, module, NULL, as, PyEval_GetLocals(), PyEval_GetGlobals())))
    {
        fclose(f);
        return NULL;
    }
#ifdef IMPORT_DEBUG
    Ps_Log("******************************************", Ps_LOG_NORMAL);
    Ps_LogObject(m, Ps_LOG_NORMAL);
    Ps_Log("******************************************", Ps_LOG_NORMAL);
#endif
    Py_INCREF(Py_True);
    return Py_True;
}

/**
 *  完成from [website] import ...功能
 **/
PyObject* 
Ps_FromWebImport(PyObject *self, PyObject *args, PyObject *kwds)
{
    char *website;
    char *module;
    char *kwdslists[] = {"website", "module", 0};
    if(!PyArg_ParseTupleAndKeywords(args, kwds, "ss", kwdslists, &website, &module))
    {
        PyErr_SetString(PyExc_TypeError, "args website, module");
        return NULL;
    }
    return from_web_import_as(website, module, NULL); 
}

/**
 *  完成from [website] import ...as...功能
 **/
PyObject* 
Ps_FromWebImportAs(PyObject *self, PyObject *args, PyObject *kwds)
{
    char *website;
    char *module;
    char *as;
    char *kwdslist[] = {"website", "module", "as", 0};
    
    if(!PyArg_ParseTupleAndKeywords(args, kwds, "sss", kwdslist, &website, &module, &as))
    {
        PyErr_SetString(PyExc_ImportError, "args website, module, as");
        return NULL;
    }
    return from_web_import_as(website, module, as);

}

/**
 *  完成import...
 **/
PyObject* 
Ps_Import(PyObject *self, PyObject *args, PyObject *kws)
{
	//struct _ts *state;
	char *module;
	char *arglist[] = {"module", 0};                                                                                           
	FILE *f;
	PyObject *m;
	//PyObject *local = NULL, *global = NULL;

	if(!PyArg_ParseTupleAndKeywords(args, kws, "s:import", arglist, &module))
	{
		Ps_Log("Import args or kws is faile, function is that ps_import(module)\n", Ps_LOG_WARING);
		return NULL;
	}
	if((f = (open_file(NULL, module))) == NULL)
	{
		Ps_LogFormat("Open file %s failed\n", Ps_LOG_WARING, module);
		goto error;
	}
	/*state = PyThreadState_Get();
	
	if(state->frame)
	{
		local = state->frame->f_locals;
		global = state->frame->f_globals;
	}*/

	if((m = __import__(f, module, NULL, NULL, PyEval_GetLocals(), PyEval_GetGlobals())) == NULL)
	{
		if(PyErr_Occurred())
		{
			return NULL;
		}
		goto error;
	}
	Py_INCREF(Py_True);
	return Py_True;
error:
	PyErr_SetString(PyExc_ImportError, "Import error");
	return NULL;
}

/**
 *  完成import...as...
 **/
PyObject* 
Ps_ImportAs(char *module, char *name)
{

}

static PyMethodDef
import_module_methods[] = 
{
	{"ps_fromimport", (PyCFunction)Ps_FromImport, METH_VARARGS, ""},
	{"ps_fromimportas", (PyCFunction)Ps_FromImportAs, METH_VARARGS, ""},
	{"ps_import", (PyCFunction)Ps_Import, METH_VARARGS | METH_KEYWORDS, ""},
	{"ps_importas", (PyCFunction)Ps_ImportAs, METH_VARARGS, ""},
	{"ps_fromwebimport", (PyCFunction)Ps_FromWebImport, METH_VARARGS, ""},
	{"ps_fromwebimportas", (PyCFunction)Ps_FromWebImportAs, METH_VARARGS, ""},
	{NULL, NULL, 0, NULL}
};

#ifdef IMPORT_DEBUG
#include "../../include/init_clear.h"
PyMODINIT_FUNC
initpsimport(void)
{
	FILE *f = fopen("log.txt", "w");
	Ps_INIT(f, f, f);
	//printf("I am debug\n");
	Ps_ImportInit();
	Py_InitModule("psimport", import_module_methods);
	//Ps_CLEAR();
}
#else
PyMODINIT_FUNC
initpsimport(void)
{
	Py_InitModule("psimport", import_module_methods);
}
#endif
