%feature("autodoc");

%feature("python:slot", "tp_str", functype="reprfunc") CommonDataRepresentation::cdr::toString;

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

%include "cdr.i"
