
set(BOZ_MSG_LIB bozmsg_queue)

set(SOURCES
    boz_queue_p.c
    boz_queue_new.c
    boz_queue_release.c
    )
    
add_library(${BOZ_MSG_LIB} STATIC
    ${SOURCES}
    )

target_link_libraries(${BOZ_MSG_LIB}
    datastruct stddjb
    bozmsg
    )
    
BOZ_INSTALL_ITEM(${BOZ_MSG_LIB})

