
set(BOZ_MESSAGE_LIB bozmessage)

set(SOURCES
#    bozmessage_drop.c
#    bozmessage_handle.c
    bozmessage_put.c
#    bozmessage_receive.c
#    bozmessage_receiver_0.c
    bozmessage_receiver_free.c
    bozmessage_receiver_init.c
#    bozmessage_sender_1.c
#    bozmessage_sender_flush.c
    bozmessage_sender_free.c
#    bozmessage_sender_getfd.c
    bozmessage_sender_init.c
#    bozmessage_sender_timed_flush.c
#    bozmessage_sender_x.c
#    bozmessage_sender_zero.c
#    bozmessage_timed_handle.c
#    bozmessage_timed_receive.c
    bozmessage_unput.c
    bozmessage_v_zero.c
    bozmessage_zero.c

    )
    
add_library(${BOZ_MESSAGE_LIB} STATIC
    ${SOURCES}
    )

target_link_libraries(${BOZ_MESSAGE_LIB}
    skarnet
    )
    
