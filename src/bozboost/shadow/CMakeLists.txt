
set(BOZ_SHADOW_APP shadow)

set(SOURCES
    main.cpp
    ShadowPublic.cpp
    ShadowInternal.cpp
    )
    
add_executable(${BOZ_SHADOW_APP} 
    ${SOURCES}
    )
    
target_link_libraries(${BOZ_SHADOW_APP}
    boost_thread
    boost_system
    )
    
# BOZ_INSTALL_ITEM(${BOZ_SHADOW_APP})

