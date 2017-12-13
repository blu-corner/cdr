include_directories(
  ${PROJECT_SOURCE_DIR}/include
  ${PROJECT_BINARY_DIR}/src
  )

add_library(cdr SHARED
  cdr.cpp
  )

install(TARGETS cdr EXPORT cdr
  LIBRARY DESTINATION lib)

