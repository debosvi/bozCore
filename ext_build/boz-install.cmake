
macro(BOZ_INSTALL_ITEM target)
    message(STATUS "Install target ${target}")
    install(TARGETS ${target}
        RUNTIME DESTINATION ${BINARY_INSTALL_DIR}
        LIBRARY DESTINATION ${LIBRARY_INSTALL_DIR}
        ARCHIVE DESTINATION ${LIBRARY_INSTALL_DIR}/static
    )
endmacro()

