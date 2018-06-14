#
# Copyright 2014-2018 Neueda Ltd.
#
# Include python
find_package(PythonInterp REQUIRED)
set(Python_ADDITIONAL_VERSIONS 2.7)
find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_PATH})

# set the flags for swig
set(CMAKE_SWIG_FLAGS "")
set_source_files_properties(../cdr.py.i PROPERTIES CPLUSPLUS ON)

# Add swig module
swig_add_module(cdr python ../cdr.py.i)
swig_link_libraries(cdr cdr ${PYTHON_LIBRARIES})

# Files to install with Python
set(PYTHON_INSTALL_FILES
  ${CMAKE_CURRENT_BINARY_DIR}/cdr.py
  ${CMAKE_CURRENT_BINARY_DIR}/_cdr.so)

# Declare install target for python
install(FILES ${PYTHON_INSTALL_FILES}
  DESTINATION lib/python)

set(SWIG_PY_RUNTIME ${CMAKE_INSTALL_PREFIX}/include/swigPyRuntime.h)
if(NOT EXISTS ${SWIG_PY_RUNTIME})
    install(CODE "execute_process(COMMAND swig -c++ -python -external-runtime ${SWIG_PY_RUNTIME})")
endif(NOT EXISTS ${SWIG_PY_RUNTIME})
