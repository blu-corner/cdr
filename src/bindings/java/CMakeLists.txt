#
# Copyright 2014-2018 Neueda Ltd.
#
# Include Java
find_package(Java REQUIRED COMPONENTS Runtime Development)
find_package(JNI REQUIRED)
include(UseJava)

include_directories(${JNI_INCLUDE_DIRS})

# set the flags for swig
set(CMAKE_SWIG_FLAGS -package com.neueda.cdr)
set_source_files_properties(../cdr.java.i PROPERTIES CPLUSPLUS ON)

# Add swig module
add_compile_options(-fno-strict-aliasing)
swig_add_library(Cdr LANGUAGE java SOURCES ../cdr.java.i)
swig_link_libraries(Cdr cdr ${JNI_LIBRARIES})

set(CMAKE_JAR_CLASSES_PREFIX com/neueda/cdr)
set(JNI_SOURCES
  ${CMAKE_CURRENT_BINARY_DIR}/cdr.java
  ${CMAKE_CURRENT_BINARY_DIR}/cdrJNI.java
  ${CMAKE_CURRENT_BINARY_DIR}/SWIGTYPE_p_double.java
  ${CMAKE_CURRENT_BINARY_DIR}/SWIGTYPE_p_p_std__vectorT_neueda__cdr_t.java
  ${CMAKE_CURRENT_BINARY_DIR}/SWIGTYPE_p_pairT_int_const_neueda__cdrItem_t.java
  ${CMAKE_CURRENT_BINARY_DIR}/SWIGTYPE_p_size_t.java
  ${CMAKE_CURRENT_BINARY_DIR}/SWIGTYPE_p_std__string.java
  ${CMAKE_CURRENT_BINARY_DIR}/SWIGTYPE_p_std__vectorT_neueda__cdrItem_const_p_t.java
  ${CMAKE_CURRENT_BINARY_DIR}/SWIGTYPE_p_u_int.java
  ${CMAKE_CURRENT_BINARY_DIR}/Cdr.java
  ${CMAKE_CURRENT_BINARY_DIR}/cdrArray.java
  ${CMAKE_CURRENT_BINARY_DIR}/cdrDateTime.java
  ${CMAKE_CURRENT_BINARY_DIR}/cdrItem.java
  ${CMAKE_CURRENT_BINARY_DIR}/cdrItemType.java
  )

add_jar(CdrJNI
  ${JNI_SOURCES}
  )

if(TARGET _Cdr)
  add_dependencies(CdrJNI _Cdr)
else()
  add_dependencies(CdrJNI Cdr)
endif()

# Files to install with Java
if(APPLE)
  set(JAVA_DYLIB_SUFFIX ".jnilib")
else()
  set(JAVA_DYLIB_SUFFIX ".so")
endif()

# cmake 2.8 swig has issues on figuring out naming libs
if(TARGET _Cdr)
  set_target_properties(_Cdr PROPERTIES OUTPUT_NAME "Cdr")
  set_target_properties(_Cdr PROPERTIES SUFFIX ${JAVA_DYLIB_SUFFIX})
endif()

set(JAVA_INSTALL_FILES
  ${CMAKE_CURRENT_BINARY_DIR}/CdrJNI.jar)

# Declare install target for java
install(FILES ${JAVA_INSTALL_FILES}
  DESTINATION lib/java)

if(TARGET _Cdr)
  install(TARGETS _Cdr
	DESTINATION lib/java)
else()
  install(TARGETS Cdr
	DESTINATION lib/java)
endif()
