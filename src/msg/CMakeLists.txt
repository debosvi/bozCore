
set(BOZ_MSG_LIB bozmsg)

set(SOURCES
    boz_msg_p.c
    boz_msg_new.c
    boz_msg_release.c
    boz_msg_type.c
    boz_msg_dup.c
    boz_msg_payload.c
    boz_msg_lg.c
    boz_msg_append.c
    boz_msg_ushort.c
    boz_msg_ulong.c
    boz_msg_load.c
    boz_msg_vsprintf.c
    )
    
add_library(${BOZ_MSG_LIB} STATIC
    ${SOURCES}
    )

target_link_libraries(${BOZ_MSG_LIB}
    skarnet
    )
    
BOZ_INSTALL_ITEM(${BOZ_MSG_LIB})

