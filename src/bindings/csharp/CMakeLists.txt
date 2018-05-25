#
# Copyright 2014-2018 Neueda Ltd.
#
find_program(MCS mcs)
if(NOT MCS)
  message(FATAL_ERROR "Failed to find mono compiler")
endif()
message(STATUS "Found mono compiler: ${MCS}")

# set the flags for swig
set(CMAKE_SWIG_FLAGS -namespace Neueda.Cdr -dllimport CdrCsharp)
set_source_files_properties(../cdr.csharp.i PROPERTIES CPLUSPLUS ON)

# Add swig module
add_compile_options(-fno-strict-aliasing)
swig_add_module(CdrCsharp csharp ../cdr.csharp.i)
swig_link_libraries(CdrCsharp cdr)

# Files to install with csharp
add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/Cdr.dll
  COMMAND ${MCS} -target:library -out:${CMAKE_CURRENT_BINARY_DIR}/Cdr.dll ${CMAKE_CURRENT_BINARY_DIR}/*.cs
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  DEPENDS ${CSHARP_SRC_FILES}
  COMMENT "Generating Csharp DLL"
  VERBATIM
  )
add_custom_target(CsharpDLL ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/Cdr.dll)

if(TARGET _CdrCsharp)
  add_dependencies(CsharpDLL _CdrCsharp)
else()
  add_dependencies(CsharpDLL CdrCsharp)
endif()

if(APPLE)
  set(CSHARP_DYLIB_PREFIX "")
  set(CSHARP_DYLIB_SUFFIX ".so")
else()
  set(CSHARP_DYLIB_PREFIX "lib")
  set(CSHARP_DYLIB_SUFFIX ".so")
endif()

# cmake 2.8 swig has issues on figuring out naming libs
if(TARGET _CdrCsharp)
  set_target_properties(_CdrCsharp PROPERTIES OUTPUT_NAME "CdrCsharp")
  set_target_properties(_CdrCsharp PROPERTIES SUFFIX ${CSHARP_DYLIB_SUFFIX})
endif()

set(CSHARP_INSTALL_FILES
  # ${CMAKE_CURRENT_SOURCE_DIR}/run.sh
  # ${CMAKE_CURRENT_SOURCE_DIR}/build.sh
  # ${CMAKE_CURRENT_SOURCE_DIR}/example.cs
  ${CMAKE_CURRENT_BINARY_DIR}/Cdr.dll)

# Declare install target for csharp
install(FILES ${CSHARP_INSTALL_FILES}
  DESTINATION lib/csharp)

if(TARGET _CdrCsharp)
  install(TARGETS _CdrCsharp
    DESTINATION lib/csharp)
else()
  install(TARGETS CdrCsharp
    DESTINATION lib/csharp)
endif()
