#ifndef IMPORT_MODULE_H
#define IMPORT_MODULE_H

#ifdef _cplusplus
extern "C"
{
#endif

#include <python2.6/Python.h>
#include "../../include/web.h"
#include "../../include/mm.h"

//完成from...import...
PyObject* Ps_FromImport(char *path, char *module);
//完成from...import...as...
PyObject* Ps_FromImportAs(char *path, char *module, char *name);
//完成from [website] import ...功能
PyObject* Ps_FromWebImport(PyObject *self, PyObject *args, PyObject *kwds);
//完成from [website] import ...as...功能
PyObject* Ps_FromWebImportAs(PyObject *self, PyObject *args, PyObject *kwds);
//完成import...
PyObject* Ps_Import(PyObject *self, PyObject *args, PyObject *kwds);
//完成import...as...
PyObject* Ps_ImportAs(char *module, char *name);

#define IMPORT_DEBUG

#ifdef _cplusplus
}
#endif

#endif