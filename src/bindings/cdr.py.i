/*
 * Copyright 2014-2018 Neueda Ltd.
 */
%feature("autodoc");

%{
#include <cdr.h>

#include <stdexcept>
#include <sstream>
#include <string>
#include "datetime.h"
#include "Python.h"

#if PY_MAJOR_VERSION >= 3
#define PyString_AsStringAndSize PyBytes_AsStringAndSize
#define PyString_FromStringAndSize PyBytes_FromStringAndSize
#endif

%}

%include "exception.i"

// simple exception handler
%exception {
    try {
        $action
    } catch (std::exception &e) {
        std::string s("cdr-error: "), s2(e.what());
        s = s + s2;
        SWIG_exception(SWIG_RuntimeError, s.c_str());
    }
}

%extend neueda::cdr {
    PyObject* __repr__ ()
    {
        std::string op = self->toString ();
        return PyString_FromString (op.c_str ());
    }

    PyObject* __getitem__(const cdrKey_t& key)
    {
        //const neueda::cdrItem* item = self->getItem (key);
        neueda::cdrItem* item = NULL;
        for (neueda::cdr::iterator cdr_it = self->begin(); cdr_it != self->end(); ++cdr_it)
        {
            if (cdr_it->first == key)
            {
                item = &(cdr_it->second);
                break;
            }
        }
        if (item == NULL)
        {
            std::ostringstream oss;
            oss << "failed to find key: "
                << key;
            throw std::runtime_error (oss.str ());
        }

        PyObject* o = NULL;
        switch (item->mType)
        {
            case neueda::CDR_ARRAY:
                {
                    o = PyList_New (item->mArray.size ());

                     int idx = 0;
                     neueda::cdrArray::iterator it = item->mArray.begin();
                     for (; it != item->mArray.end (); ++it)
                     {
                         for (neueda::cdr::iterator cdr_it = it->begin(); cdr_it != it->end(); ++cdr_it)
                         {
                             swig_type_info* i = SWIG_TypeQuery ("_p_neueda__cdr");
                             PyObject* obj = SWIG_NewPointerObj(&(*it), i, SWIG_POINTER_OWN);
                             Py_INCREF (obj);
                             PyList_SetItem (o, idx++, obj);
                         }
                     }

                    return o;
                }
            case neueda::CDR_STRING:
                {
                    o = PyString_FromString (item->mString.c_str ());
                    return o;
                }
            case neueda::CDR_DOUBLE:
                {
                    o = PyFloat_FromDouble (item->mDouble);
                    return o;
                }
            case neueda::CDR_INTEGER:
                {
                    o = Py_BuildValue ("L", (int64_t)item->mInteger);
                    return o;
                }
            case neueda::CDR_DATETIME:
                {
#if !(PY_MAJOR_VERSION == 2 && PY_MINOR_VERSION <= 6)
                    PyDateTime_IMPORT;
#else
                    PyDateTimeAPI = (PyDateTime_CAPI*) PyCObject_Import((char *) "datetime",
                            (char *) "datetime_CAPI");
#endif
                    o = PyDateTime_FromDateAndTime (
                            item->mDateTime.mYear,
                            item->mDateTime.mMonth,
                            item->mDateTime.mDay,
                            item->mDateTime.mHour,
                            item->mDateTime.mMinute,
                            item->mDateTime.mSecond,
                            item->mDateTime.mNanosecond / 1000);
                    return o;
                }


        }
        Py_INCREF (Py_None);
        return Py_None;
    }

    void __setitem__(const cdrKey_t& key, PyObject* v)
    {
#if !(PY_MAJOR_VERSION == 2 && PY_MINOR_VERSION <= 6)
        PyDateTime_IMPORT;
#else
        PyDateTimeAPI = (PyDateTime_CAPI*) PyCObject_Import((char *) "datetime",
                (char *) "datetime_CAPI");
#endif
        if (PyString_Check (v))
        {
            char* val = PyString_AsString (v);
            self->setString (key, val);
        }
        else if (PyInt_Check (v))
        {
            int val = PyLong_AsLongLong (v);
            self->setInteger (key, val);

        }
        else if (PyFloat_Check (v))
        {
            double val = PyFloat_AsDouble (v);
            self->setDouble (key, val);
        }
        else if (PyDate_CheckExact(v))
        {
            neueda::cdrDateTime dt;
            dt.mYear = PyDateTime_GET_YEAR (v);
            dt.mMonth = PyDateTime_GET_MONTH (v);
            dt.mDay = PyDateTime_GET_DAY (v);

            self->setDateTime (key, dt);
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
            dt.mNanosecond = PyDateTime_DATE_GET_MICROSECOND (v);

            self->setDateTime (key, dt);
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

            self->setArray (key, arr);
        }
    }

    PyObject* keys () {
        PyObject* ret = PyList_New (self->size ());
        neueda::cdr::iterator it = self->begin ();
        int idx = 0;

        while (it != self->end ())
        {
            PyList_SetItem (ret, idx++, PyLong_FromUnsignedLongLong (it->first));
            it++;
        }

        return ret;
    }

    PyObject* serialize ()
    {
        char* buf = new char[self->serializedSize ()];
        size_t used = 0;

        if (!self->serialize (buf, used, true))
            return NULL;

        PyObject* bytes;
        bytes = PyString_FromStringAndSize (buf, used);

        delete[] buf;
        return bytes;
    }

    PyObject* deserialize (PyObject* v)
    {
        char* buf;
        Py_ssize_t len;
        if (!PyString_Check (v))
            return NULL;

        PyString_AsStringAndSize (v, &buf, &len);

        size_t used = 0;
        if (!self->deserialize (buf, used))
            return NULL;

        Py_INCREF (Py_None);
        return Py_None;
    }

    PyObject* toPythonDict ()
    {
        PyGILState_STATE gil = PyGILState_Ensure ();
        PyObject* dict = PyDict_New ();
        PyObject* key;
        PyObject* val;
        PyObject* d;

        neueda::cdr::iterator it = self->begin ();

        while (it != self->end ())
        {
            key = Py_BuildValue ("K", it->first);
            if (it->second.mType == neueda::CDR_ARRAY)
            {
                // cdrArray
                int idx = 0;
                PyObject* cdrs = PyList_New (it->second.mArray.size ());
                for (neueda::cdrArray::const_iterator aIt = it->second.mArray.begin ();
                     aIt != it->second.mArray.end ();
                     ++aIt)
                {
                    d = neueda_cdr_toPythonDict (const_cast<neueda::cdr*>(&(*aIt)));
                    PyList_SetItem (cdrs, idx++, d);
                }
                PyDict_SetItem (dict, key, cdrs);
            }
            else
            {
                val = neueda_cdr___getitem__ (self, it->first);
                PyDict_SetItem (dict, key, val);
            }
            it++;
        }

        PyGILState_Release (gil);
        return dict;
    }
}

%include "cdr.i"
