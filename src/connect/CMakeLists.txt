
set(BOZ_MSG_LIB bozconnect)

set(SOURCES
    boz_connect_p.c
    boz_connect_new.c
    boz_connect_release.c
    boz_connect_type.c
    boz_connect_fd.c
    boz_connect_events.c
    boz_connect_flush.c
    boz_connect_put.c
    boz_connect_yield.c
    boz_connect_get.c
    )
    
add_library(${BOZ_MSG_LIB} STATIC
    ${SOURCES}
    )

target_link_libraries(${BOZ_MSG_LIB}
    skarnet
    )
    
BOZ_INSTALL_ITEM(${BOZ_MSG_LIB})

