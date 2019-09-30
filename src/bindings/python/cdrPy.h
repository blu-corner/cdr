#pragma once

#include <Python.h>
#include "cdr.h"
#include "datetime.h"

#if PY_MAJOR_VERSION >= 3

#define PyString_AsStringAndSize PyBytes_AsStringAndSize
#define PyString_FromStringAndSize PyBytes_FromStringAndSize
#define PyString_Check PyUnicode_Check
#define PyString_AsString PyUnicode_AsUTF8

#else

#define PyBytes_Check PyString_Check

#define PyDateTime_IMPORT \
    PyDateTimeAPI = (PyDateTime_CAPI*) PyCObject_Import((char *) "datetime", (char *) "datetime_CAPI"); \

#endif

using namespace neueda;

bool cdrImportModule ();

bool cdrPySetItem (const cdrKey_t& key, PyObject* v, cdr* d);

bool cdrFromPyDict (PyObject* pyDict, cdr*& d);

PyObject* cdrToPyObj (cdr* d);

bool cdrFromPyObj (PyObject* pycdr, cdr*& d);
