#
# Copyright 2014-2018 Neueda Ltd.
#

if(NOT CSHARP_COMPILER)
  if(NOT WIN32)
    find_program(CSHARP_COMPILER mcs)
  else()
    find_program(CSHARP_COMPILER csc)
  endif()

  if(NOT CSHARP_COMPILER)
    message(FATAL_ERROR "Failed to find csharp compiler")
  endif()
endif()
message(STATUS "Found csharp compiler: ${CSHARP_COMPILER}")
set(CSHARP_OUTPUT_NAME "CdrBindings.dll")


# set the flags for swig
set(CMAKE_SWIG_FLAGS -namespace Neueda.Cdr -dllimport CdrCsharp -outfile Cdr.cs)
set_source_files_properties(../cdr.csharp.i PROPERTIES CPLUSPLUS ON)

# Add swig module
add_compile_options(-fno-strict-aliasing)
swig_add_module(CdrCsharp csharp ../cdr.csharp.i)
swig_link_libraries(CdrCsharp cdr)

# Files to install with csharp
add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${CSHARP_OUTPUT_NAME}
  COMMAND ${CSHARP_COMPILER} -target:library -out:${CMAKE_CURRENT_BINARY_DIR}/${CSHARP_OUTPUT_NAME} ${CMAKE_CURRENT_BINARY_DIR}/*.cs
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  DEPENDS ${CSHARP_SRC_FILES}
  COMMENT "Generating Csharp DLL"
  VERBATIM
  )
add_custom_target(CsharpDLL ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${CSHARP_OUTPUT_NAME})
add_dependencies(CsharpDLL ${SWIG_MODULE_CdrCsharp_REAL_NAME})

if(APPLE)
  set(CSHARP_DYLIB_PREFIX "")
  set(CSHARP_DYLIB_SUFFIX ".so")
elseif(WIN32)
  set(CSHARP_DYLIB_PREFIX "")
  set(CSHARP_DYLIB_SUFFIX ".dll")
else()
  set(CSHARP_DYLIB_PREFIX "lib")
  set(CSHARP_DYLIB_SUFFIX ".so")
endif()

set_target_properties(${SWIG_MODULE_CdrCsharp_REAL_NAME} PROPERTIES OUTPUT_NAME "CdrCsharp")
if(NOT CSHARP_DYLIB_PREFIX STREQUAL "")
  set_target_properties(${SWIG_MODULE_CdrCsharp_REAL_NAME} PROPERTIES PREFIX ${CSHARP_DYLIB_PREFIX})
endif()
set_target_properties(${SWIG_MODULE_CdrCsharp_REAL_NAME} PROPERTIES SUFFIX ${CSHARP_DYLIB_SUFFIX})

set(CSHARP_INSTALL_FILES
  # ${CMAKE_CURRENT_SOURCE_DIR}/run.sh
  # ${CMAKE_CURRENT_SOURCE_DIR}/build.sh
  # ${CMAKE_CURRENT_SOURCE_DIR}/example.cs
  ${CMAKE_CURRENT_BINARY_DIR}/${CSHARP_OUTPUT_NAME})

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
