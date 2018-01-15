%feature("autodoc");

%feature("python:slot", "tp_str", functype="reprfunc") CommonDataRepresentation::cdr::toString;

%include "cdr.i"
