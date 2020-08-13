#
# Copyright 2014-2018 Neueda Ltd.
#
# Include python
# find_package(PythonInterp ${PYTHON_VER} REQUIRED)
# find_package(PythonLibs ${PYTHON_VER} REQUIRED)
# include_directories(${PYTHON_INCLUDE_PATH})

# set the flags for swig
set(CMAKE_SWIG_FLAGS "")
set_source_files_properties(../cdr.py.i PROPERTIES CPLUSPLUS ON)

execute_process(
    COMMAND bash -c "${PYTHON_CONFIG} --includes"
    OUTPUT_VARIABLE PYTHON_INCLUDE_PATH
    RESULT_VARIABLE RET
    )
if(NOT ${RET} EQUAL "0")
    message(FATAL_ERROR "failed to run python-config --includes")
endif()

execute_process(
    COMMAND bash -c "${PYTHON_CONFIG} --ldflags"
    OUTPUT_VARIABLE PYTHON_LDFLAGS
    RESULT_VARIABLE RET
    )
if(NOT ${RET} EQUAL "0")
    message(FATAL_ERROR "failed to run python-config --ldflags")
endif()

execute_process(
    COMMAND bash -c "${PYTHON_CONFIG} --libs"
    OUTPUT_VARIABLE PYTHON_LIBS
    RESULT_VARIABLE RET
    )
if(NOT ${RET} EQUAL "0")
    message(FATAL_ERROR "failed to run python-config --libs")
endif()

string(STRIP ${PYTHON_LDFLAGS} PYTHON_LDFLAGS)
string(STRIP ${PYTHON_INCLUDE_PATH} PYTHON_INCLUDE_PATH)
string(STRIP ${PYTHON_LIBS} PYTHON_LIBS)

message(STATUS "Python includes: ${PYTHON_INCLUDE_PATH}")
message(STATUS "Python ldflags: ${PYTHON_LDFLAGS}")
message(STATUS "Python libs: ${PYTHON_LIBS}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${PYTHON_INCLUDE_PATH}")
link_libraries(${PYTHON_LDFLAGS})

# Add swig module
swig_add_LIBRARY(cdr LANGUAGE python SOURCES ../cdr.py.i)
swig_link_libraries(cdr cdr cdrpy ${PYTHON_LIBS})

# Files to install with Python
set(PYTHON_INSTALL_FILES
  ${CMAKE_CURRENT_BINARY_DIR}/cdr.py)

# Declare install target for python
install(TARGETS _cdr
  DESTINATION lib/python)
install(FILES ${PYTHON_INSTALL_FILES}
  DESTINATION lib/python)

set(SWIG_PY_RUNTIME ${CMAKE_CURRENT_BINARY_DIR}/swigPyRuntime.h)
if(NOT EXISTS ${SWIG_PY_RUNTIME})
    add_custom_command(OUTPUT ${SWIG_PY_RUNTIME}
                       COMMAND swig -c++ -python -external-runtime ${SWIG_PY_RUNTIME})
endif(NOT EXISTS ${SWIG_PY_RUNTIME})

install(FILES ${SWIG_PY_RUNTIME} DESTINATION include)
install(FILES cdrPy.h DESTINATION include/cdr)

add_custom_target(generate-swig-runtime DEPENDS ${SWIG_PY_RUNTIME})

set(CDRPY_SOURCES cdrPy.cpp)

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
  ${PROJECT_SOURCE_DIR}/src/bindings/python
  ${CMAKE_INSTALL_PREFIX}/include
  ${CMAKE_INSTALL_PREFIX}/include/cdr
  )

add_library(cdrpy SHARED ${CDRPY_SOURCES})
add_dependencies(cdrpy generate-swig-runtime cdr)
target_link_libraries(cdrpy cdr)

install(TARGETS cdrpy DESTINATION lib)
