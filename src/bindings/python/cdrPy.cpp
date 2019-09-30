#include "cdrPy.h"
#include "swigPyRuntime.h"

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

static PyObject* CDR_MODULE = NULL;
static PyObject* CDR_CLASS = NULL;
static swig_type_info* CDR_TYPE = NULL;

bool
cdrCheckType (PyObject* o)
{
    if (PyObject_IsInstance (o, CDR_CLASS) != 1)
        return false;

    return true;
}

bool
cdrImportModule ()
{
    if (CDR_MODULE == NULL &&
        (CDR_MODULE = PyImport_ImportModuleNoBlock ("cdr")) == NULL)
        return false;

    if (CDR_TYPE == NULL)
    {
        CDR_TYPE = SWIG_TypeQuery ("_p_neueda__cdr");
        if (!CDR_TYPE)
            return NULL;
    }

    return true;
}

PyObject*
cdrToPyObj (cdr* d)
{
    if (!cdrImportModule ())
        return NULL;

    PyObject* o = SWIG_NewPointerObj (d, CDR_TYPE, SWIG_POINTER_OWN);
    Py_INCREF (o);
    return o;
}

bool
cdrFromPyObj (PyObject* pycdr, cdr*& d)
{
    void* argp = 0;
    int res = 0;

    if (!cdrImportModule ())
        return false;

    res = SWIG_ConvertPtr (pycdr, &argp, CDR_TYPE, 0 | 0);
    if (!SWIG_IsOK (res))
        return false;

    d = (cdr*)argp;
    return true;
}

bool
cdrPySetItem (const cdrKey_t& key, PyObject* v, cdr* d)
{
    PyDateTime_IMPORT;

    if (PyInt_Check (v))
    {
        int val = PyLong_AsLongLong (v);
        d->setInteger (key, val);

    }
    else if (PyFloat_Check (v))
    {
        double val = PyFloat_AsDouble (v);
        d->setDouble (key, val);
    }
    else if (PyDate_CheckExact(v))
    {
        neueda::cdrDateTime dt;
        dt.mYear = PyDateTime_GET_YEAR (v);
        dt.mMonth = PyDateTime_GET_MONTH (v);
        dt.mDay = PyDateTime_GET_DAY (v);

        d->setDateTime (key, dt);
    }
    else if (PyDateTime_CheckExact(v))
    {
        neueda::cdrDateTime dt;
        dt.mYear = PyDateTime_GET_YEAR (v);
        dt.mMonth = PyDateTime_GET_MONTH (v);
        dt.mDay = PyDateTime_GET_DAY (v);

        dt.mHour = PyDateTime_DATE_GET_HOUR (v);
        dt.mMinute = PyDateTime_DATE_GET_MINUTE (v);
        dt.mSecond = PyDateTime_DATE_GET_SECOND (v);
        dt.mNanosecond = PyDateTime_DATE_GET_MICROSECOND (v) * 1000;

        d->setDateTime (key, dt);
    }
    else if (PyList_Check (v))
    {
        neueda::cdrArray arr;
        swig_type_info* i = SWIG_TypeQuery ("_p_neueda__cdr");
        Py_ssize_t len    = PyList_Size (v);

        for (Py_ssize_t idx = 0; idx < len; ++idx)
        {
            PyObject* py_obj = PyList_GetItem (v, idx);
            neueda::cdr* obj = NULL;

            SWIG_ConvertPtr (py_obj, (void **)&obj, i, 0);

            if (obj != NULL)
                arr.push_back (*obj);
        }

        d->setArray (key, arr);
    }
    else if (PyString_Check (v))
    {
        d->setString (key, PyString_AsString (v));
    }
    else
        return false;

    return true;
}

bool
cdrFromPyDict (PyObject* pyDict, cdr*& d)
{
    Py_ssize_t pos = 0;
    PyObject *key, *value;

    while (PyDict_Next(pyDict, &pos, &key, &value))
    {
        if (!PyInt_Check (key))
            return false;

        cdrKey_t k = PyLong_AsLong (key);
        if (!cdrPySetItem (k, value, d))
            return false;
    }

    return true;
}
