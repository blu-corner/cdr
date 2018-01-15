# Include Java
find_package(Java REQUIRED COMPONENTS Runtime Development)
find_package(JNI REQUIRED)
include_directories(${JNI_INCLUDE_DIRS})

# set the flags for swig
set(CMAKE_SWIG_FLAGS -package com.neueda.cdr)
set_source_files_properties(../cdr.java.i PROPERTIES CPLUSPLUS ON)

# Add swig module
add_compile_options(-fno-strict-aliasing)
swig_add_module(Cdr java ../cdr.java.i)
swig_link_libraries(Cdr cdr ${JNI_LIBRARIES})

set(CMAKE_JAR_CLASSES_PREFIX com/neueda/cdr)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_JAR_CLASSES_PREFIX})

# file(GLOB JAVA_SRC_FILES ${CMAKE_CURRENT_BINARY_DIR}/*.java)
# file(COPY ${JAVA_SRC_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_JAR_CLASSES_PREFIX})
# file(GLOB JAVA_JAR_SRC_FILES ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_JAR_CLASSES_PREFIX}/*.java)

# add_jar(CdrJNI
#   SOURCES
#   ${JAVA_JAR_SRC_FILES})
# add_dependencies(CdrJNI Cdr)

# Files to install with Java
set(JAVA_INSTALL_FILES
  #  CdrJNI
  ${CMAKE_CURRENT_BINARY_DIR}/_Cdr.so)

# Declare install target for java
install(FILES ${JAVA_INSTALL_FILES}
  DESTINATION lib/java)
