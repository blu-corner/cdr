%typemap(throws, canthrow=1) std::runtime_error {
    SWIG_CSharpSetPendingExceptionArgument(SWIG_CSharpApplicationException, $1.what(), NULL);
    return $null;
}

%include "cdr.i"
