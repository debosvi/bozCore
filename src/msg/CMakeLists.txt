
set(BOZ_MSG_LIB bozmsg)

set(SOURCES
    boz_msg_p.c
    )
    
add_library(${BOZ_MSG_LIB} SHARED
    ${SOURCES}
    )
    
target_link_libraries(${BOZ_MSG_LIB}
    datastruct stddjb
    )
    