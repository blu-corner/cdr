#
# Copyright 2014-2018 Neueda Ltd.
#
include_directories(
  ${PROJECT_SOURCE_DIR}/src
  ${PROJECT_SOURCE_DIR}/test
  ${CMAKE_BINARY_DIR}/googletest/include
  )

link_directories(${CMAKE_BINARY_DIR}/googletest/lib)

file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/test/*.cc)
set (LIBS cdr
          gtest
          gmock)

if(NOT WIN32)
    set (LIBS "${LIBS}"
              pthread)
endif()
  
add_executable(unittest ${SRC_FILES})
target_link_libraries(unittest ${LIBS})

add_test(NAME unittest
  COMMAND unittest
)

install(TARGETS unittest
    RUNTIME DESTINATION bin)

if(PYTHON)
    install(FILES python/test.py DESTINATION bin)
    add_test(NAME pyunittest
        COMMAND ${CMAKE_COMMAND} -E env PYTHONPATH=${CMAKE_INSTALL_PREFIX}/lib/python
        ${PYTHON_EXECUTABLE} ${CMAKE_INSTALL_PREFIX}/bin/test.py
    )
endif(PYTHON)
