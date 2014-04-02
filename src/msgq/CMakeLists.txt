
set(BOZ_MSG_LIB bozmsg_queue)

set(SOURCES
    boz_queue_p.c
    boz_queue_new.c
    boz_queue_release.c
    )
    
add_library(${BOZ_MSG_LIB} SHARED
    ${SOURCES}
    )

set_target_properties(${BOZ_MSG_LIB} PROPERTIES
    VERSION ${boz_queue_VERSION}
    SOVERSION ${boz_queue_SOVERSION}
    )
    
target_link_libraries(${BOZ_MSG_LIB}
    datastruct stddjb
    bozmsg
    )
    
