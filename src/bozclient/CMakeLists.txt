
set(BOZ_CLIENT_LIB bozclient)

set(SOURCES
    bozclient_default_cb.c
    bozclient_end.c
    bozclient_init.c
    bozclient_put.c
    bozclient_putmsg.c
    bozclient_putmsgv.c
    bozclient_putv.c
    bozclient_send.c
    bozclient_sendv.c
    bozclient_server_ack.c
    bozclient_server_init.c
    bozclient_start.c
    bozclient_start_async.c
    bozclient_start_cb.c
    bozclient_syncify.c
    bozclient_zero.c

    )
    
add_library(${BOZ_CLIENT_LIB} STATIC
    ${SOURCES}
    )

target_link_libraries(${BOZ_CLIENT_LIB}
    skarnet
    )
    

BOZ_INSTALL_ITEM(${BOZ_CLIENT_LIB})
