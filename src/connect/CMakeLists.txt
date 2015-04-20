
set(BOZ_MSG_LIB bozconnect)

set(SOURCES
    boz_connect_p.c
    boz_connect_new.c
    boz_connect_release.c
    boz_connect_type.c
    boz_connect_read_io.c
    boz_connect_write_io.c
    )
    
add_library(${BOZ_MSG_LIB} STATIC
    ${SOURCES}
    )

target_link_libraries(${BOZ_MSG_LIB}
    skarnet
    )
    
