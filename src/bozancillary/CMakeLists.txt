
set(BOZ_ANCILLARY_LIB bozancillary)

set(SOURCES
    boz_ancillary_p.c
    boz_ancil_send_fds.c
    boz_ancil_recv_fds.c
    )
    
add_library(${BOZ_ANCILLARY_LIB} STATIC
    ${SOURCES}
    )

target_link_libraries(${BOZ_ANCILLARY_LIB}
    
    )
    

BOZ_INSTALL_ITEM(${BOZ_ANCILLARY_LIB})
