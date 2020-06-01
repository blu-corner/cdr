#
# Copyright 2014-2018 Neueda Ltd.
#
set(CDR_HEADERS
  cdr.h
  )

set (CDR_SOURCES cdr.cpp)

if(WIN32)
  set(CDR_HEADERS ${CDR_HEADERS} asprintf.h)
  set(CDR_SOURCES ${CDR_SOURCES}
                  asprintf.cpp
                  vasprintf.cpp)
endif()

if(JSON)
    include_directories(jsoncpp)
endif()

install(FILES ${CDR_HEADERS}
  DESTINATION include/cdr
    )

include_directories(
  ${PROJECT_SOURCE_DIR}/src
  )


add_library(cdr SHARED ${CDR_SOURCES})

if(JSON)
    target_link_libraries(cdr jsoncpp)
endif()

install(TARGETS cdr
  EXPORT ${PROJECT_NAME}
  COMPONENT library
  RUNTIME DESTINATION lib
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib)

install(
  EXPORT ${PROJECT_NAME}
  DESTINATION lib/cmake/${PROJECT_NAME} COMPONENT dev
  PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
  )

set(${PROJECT_NAME}_INCLUDE_DIR ${CMAKE_INSTALL_PREFIX}/include)
set(${PROJECT_NAME}_LIBRARIES cdr)
configure_file(
  ${PROJECT_SOURCE_DIR}/${PROJECT_NAME}.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config-install.cmake
  @ONLY
  )
install(
  FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config-install.cmake
  DESTINATION lib/cmake/${PROJECT_NAME} COMPONENT dev
  RENAME ${PROJECT_NAME}Config.cmake
  PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
  )

add_subdirectory(bindings)
