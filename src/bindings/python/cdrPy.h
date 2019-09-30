#pragma once

#include <Python.h>
#include "cdr.h"
#include "datetime.h"

using namespace neueda;

bool cdrCheckType (PyObject* o);

bool cdrImportModule ();

bool cdrPySetItem (const cdrKey_t& key, PyObject* v, cdr* d);

bool cdrFromPyDict (PyObject* pyDict, cdr*& d);

PyObject* cdrToPyObj (cdr* d);

bool cdrFromPyObj (PyObject* pycdr, cdr*& d);
