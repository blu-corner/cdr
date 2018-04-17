#
#   Copyright 2014-2018 Neueda Ltd.
#
#   Generated 17/04/18
#
include_directories(
  ${PROJECT_SOURCE_DIR}/src
  ${PROJECT_SOURCE_DIR}/test
  ${CMAKE_BINARY_DIR}/googletest/include
  )

link_directories(${CMAKE_BINARY_DIR}/googletest/lib)

file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/test/*.cc)
  
add_executable(unittest ${SRC_FILES})

target_link_libraries(unittest
  cdr
  gtest
  gmock
  pthread
  )

add_test(NAME unittest
  COMMAND unittest
)

install(TARGETS unittest
    RUNTIME DESTINATION bin)
