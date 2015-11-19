
set(BOZ_DBUS_SERVER_APP dbus-server)

set(SOURCES
    MyDBusClassServer.cpp
    dbus-server.cpp
    )
    
add_executable(${BOZ_DBUS_SERVER_APP} 
    ${SOURCES}
    )
    
DBUS_GEN_CPP_ADAPTOR("Morpho.MA5G.MyDBusClass.xml" "MyDBusClass" ${BOZ_DBUS_SERVER_APP})

target_link_libraries(${BOZ_DBUS_SERVER_APP}
    ${DBusCpp_LIBRARIES}
    pthread
    )
    
# BOZ_INSTALL_ITEM(${BOZ_DBUS_SERVER_APP})

set(BOZ_DBUS_CLIENT_APP dbus-client)

set(SOURCES
    MyDBusClassClient.cpp
    dbus-client.cpp
    )
    
add_executable(${BOZ_DBUS_CLIENT_APP} 
    ${SOURCES}
    )

DBUS_GEN_QT_PROXY("Morpho.MA5G.MyDBusClass.xml" "MyDBusClass" ${BOZ_DBUS_CLIENT_APP})
    
target_link_libraries(${BOZ_DBUS_CLIENT_APP}
    ${DBusCpp_LIBRARIES}
    pthread
    )
    
# BOZ_INSTALL_ITEM(${BOZ_DBUS_CLIENT_APP})
