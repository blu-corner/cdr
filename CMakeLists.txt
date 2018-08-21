#
# Copyright 2014-2018 Neueda Ltd.
#
cmake_minimum_required(VERSION 2.8.0)
project(cdr CXX)

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set (CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install" CACHE PATH "default install path" FORCE )
endif()

if(APPLE)
  set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
  set(CMAKE_XCODE_ATTRIBUTE_GCC_VERSION "com.apple.compilers.llvm.clang.1_0")
  set(CMAKE_SHARED_LINKER_FLAGS "-Wl,-undefined,dynamic_lookup")
  set(CMAKE_MACOSX_RPATH 1)
endif()

# options
option(DEBUG "Enable debug build" OFF)
option(JAVA "Enable Java bindings" OFF)
option(PYTHON "Enable Python bindings" OFF)
option(CSHARP "Enable C# bindings" OFF)
option(COVERAGE "Enable gcov coverage" OFF)
option(EXAMPLES "Enable examples" ON)

if(COVERAGE)
    set(COVERAGE_COMPILE_FLAGS "-g -O0 -coverage -fprofile-arcs -ftest-coverage")
    set(COVERAGE_LINK_FLAGS    "-coverage -lgcov")
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${COVERAGE_COMPILE_FLAGS}" )
    set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${COVERAGE_LINK_FLAGS}" ) 
endif(COVERAGE)

# set version info
set(CDR_MAJOR_VERSION 0)
set(CDR_MINOR_VERSION 1)
set(CDR_PATCH_VERSION 0)
set(CDR_VERSION ${CDR_MAJOR_VERSION}.${CDR_MINOR_VERSION}.${CDR_PATCH_VERSION})

if (UNIX)
  macro (add_library _name)
    _add_library(${ARGV})
    if (NOT APPLE AND TARGET ${_name})
      SET_TARGET_PROPERTIES(${_name}
        PROPERTIES
            VERSION ${CDR_VERSION}
            # SOVERSION 0.0.0
        )
    endif()
  endmacro()
endif (UNIX)

# compile options
add_compile_options(-Wall -c -W -g)
if (DEBUG)
  message(STATUS "cdr DEBUG: ON")
  set(SUBMODULE_FLAGS ${SUBMODULE_FLAGS} -DDEBUG=on)
else ()
  message(STATUS "cdr DEBUG: OFF")
  add_compile_options(-O3)
endif (DEBUG)

# allow external projects
include(${CMAKE_ROOT}/Modules/ExternalProject.cmake)

# add source
add_subdirectory(src)

# unit-tests
option(TESTS "Enable unit-tests" OFF)
if(TESTS)
  ExternalProject_Add(
    googletest
    SOURCE_DIR ${PROJECT_SOURCE_DIR}/ext/googletest
    CMAKE_ARGS -DBUILD_GMOCK=ON -DBUILD_GTEST=ON -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/googletest -DCMAKE_INSTALL_LIBDIR=${CMAKE_BINARY_DIR}/googletest/lib
    )
  
  include(CTest)
  enable_testing()
  
  # add test dir
  add_subdirectory(test)
endif()

if(EXAMPLES)
    add_subdirectory(examples)
endif()

SET(CPACK_GENERATOR "TGZ")
INCLUDE(CPack)
