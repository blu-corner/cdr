# Include python
find_package(PythonInterp REQUIRED)
set(Python_ADDITIONAL_VERSIONS 2.7)
find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_PATH})

# set the flags for swig
set(CMAKE_SWIG_FLAGS "")
set_source_files_properties(../cdr.py.i PROPERTIES CPLUSPLUS ON)

# Add swig module
swig_add_module(Cdr python ../cdr.py.i)
swig_link_libraries(Cdr cdr ${PYTHON_LIBRARIES})

# Files to install with Python
set(PYTHON_INSTALL_FILES
  ${CMAKE_CURRENT_BINARY_DIR}/Cdr.py
  ${CMAKE_CURRENT_BINARY_DIR}/_Cdr.so)

# Declare install target for python
install(FILES ${PYTHON_INSTALL_FILES}
  DESTINATION lib/python)

if(TESTS)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/test.py
    DESTINATION bin)
endif()
