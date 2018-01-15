%feature("autodoc");

%exception {
    try {
        $action
    } catch (std::exception &e) {
        PyErr_SetString(PyExc_Exception, const_cast<char*>(e.what()));
        SWIG_fail;
    }
}

%feature("python:slot", "tp_str", functype="reprfunc") CommonDataRepresentation::cdr::toString;

%include "cdr.i"
