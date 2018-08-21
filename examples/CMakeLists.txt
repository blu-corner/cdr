INSTALL(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cpp/ DESTINATION
        examples/cdr/cpp/)

if(JAVA)
    INSTALL(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/java/ DESTINATION
            examples/cdr/java/)
endif()

if(PYTHON)
    INSTALL(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/python/ DESTINATION
            examples/cdr/python/)
endif()
