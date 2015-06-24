
set(BOZ_CONNECTION_LIB bozconnection)

set(SOURCES
    bozconnection_init.c
    bozconnection_free.c
    bozconnection_zero.c
    )
    
add_library(${BOZ_CONNECTION_LIB} STATIC
    ${SOURCES}
    )

target_link_libraries(${BOZ_CONNECTION_LIB}
    bozmessage
    )
    
BOZ_INSTALL_ITEM(${BOZ_CONNECTION_LIB})
