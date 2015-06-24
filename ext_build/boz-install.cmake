
macro(BOZ_INSTALL_HEADER arg)
#    foreach(arg IN LISTS ARGN)
        message(STATUS "Install header: ${arg}")
        install(FILES ${arg}
            DESTINATION ${HEADER_INSTALL_DIR}/${CMAKE_PROJECT_NAME}
            )
 #   endforeach()
endmacro()

macro(BOZ_INSTALL_ITEM target)
    message(STATUS "Install target ${target}")
    install(TARGETS ${target}
        RUNTIME DESTINATION ${BINARY_INSTALL_DIR}
        LIBRARY DESTINATION ${LIBRARY_INSTALL_DIR}
        ARCHIVE DESTINATION ${LIBRARY_INSTALL_DIR}/static
    )
endmacro()

