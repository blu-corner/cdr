/*
 * Copyright 2014-2018 Neueda Ltd.
 */
%feature("autodoc");

%feature("python:slot", "tp_str", functype="reprfunc") neueda::cdr::toString;

%{
#include <cdr.h>

#include <stdexcept>
#include <sstream>
#include <string>
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
    PyObject* __getitem__(const cdrKey_t& key)
    {
		const neueda::cdrItem* item = self->getItem (key);
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
			case neueda::CDR_STRING:
			{
				o = PyString_FromStringAndSize (item->mString.c_str (),
												item->mString.size ());
				return o;
			}
		}
		Py_INCREF (Py_None);
		return Py_None;
    }

    void __setitem__(const cdrKey_t& key, PyObject* v)
    {
		if (PyString_Check (v))
		{
			char* val = PyString_AsString (v);
			self->setString (key, val);
		}
    }
}

%include "cdr.i"
