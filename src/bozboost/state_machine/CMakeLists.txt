
include_directories(inc)

###############################################################################
###############################################################################
set(BOZ_STATEMACHINE_LIB dbus_statemachine)

set(SOURCES
    src/dbus_statemachine.cpp
    # src/dbus_sm1.cpp
    # src/dbus_sm2.cpp
    # src/dbus_sm3.cpp
    )
    
add_library(${BOZ_STATEMACHINE_LIB} SHARED
    ${SOURCES}
    )

target_link_libraries(${BOZ_STATEMACHINE_LIB}
    boost_system
    )
    
BOZ_INSTALL_ITEM(${BOZ_STATEMACHINE_LIB})

###############################################################################
###############################################################################
set(BOZ_STATEMACHINE_APP AppStateMachine)

set(SOURCES
    test/main.cpp
    )
    
add_executable(${BOZ_STATEMACHINE_APP} 
    ${SOURCES}
    )

target_link_libraries(${BOZ_STATEMACHINE_APP}
    dbus_statemachine
    )
    
BOZ_INSTALL_ITEM(${BOZ_STATEMACHINE_APP})
