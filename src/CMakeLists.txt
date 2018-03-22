set(CDR_HEADERS
  cdr.h
  )

install(FILES ${CDR_HEADERS}
  DESTINATION include/cdr
    )

include_directories(
  ${PROJECT_SOURCE_DIR}/src
  ${PROJECT_BINARY_DIR}/src
  )

add_library(cdr SHARED
  cdr.cpp
  )

install(TARGETS cdr EXPORT cdr
  LIBRARY DESTINATION lib)

add_subdirectory(bindings)
