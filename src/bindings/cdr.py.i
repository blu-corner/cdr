%feature("autodoc");

%exception {
    try {
        $action
    } catch (std::exception &e) {
        std::string s("cdr-error: "), s2(e.what());
        s = s + s2;
        SWIG_exception(SWIG_RuntimeError, s.c_str());
    }
}

%feature("python:slot", "tp_str", functype="reprfunc") CommonDataRepresentation::cdr::toString;

%include "cdr.i"
