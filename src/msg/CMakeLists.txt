
set(BOZ_MSG_LIB bozmsg)

set(SOURCES
    boz_msg_p.c
    boz_msg_new.c
    boz_msg_release.c
    boz_msg_type.c
    boz_msg_dup.c
    boz_msg_payload.c
    boz_msg_append.c
    )
    
add_library(${BOZ_MSG_LIB} SHARED
    ${SOURCES}
    )

set_target_properties(${BOZ_MSG_LIB} PROPERTIES
    VERSION ${boz_msg_VERSION}
    SOVERSION ${boz_msg_SOVERSION}
    )
    
target_link_libraries(${BOZ_MSG_LIB}
    datastruct stddjb
    )
    
