
include_directories(inc)

###############################################################################
###############################################################################
set(BOZ_STATEMACHINE_LIB statemachine)

set(SOURCES
    src/BaseState.cpp
    src/BaseTransition.cpp
    src/State.cpp
    src/StateMachinePrivate.cpp
    src/StateMachine.cpp
    )
    
add_library(${BOZ_STATEMACHINE_LIB} STATIC
    ${SOURCES}
    )

target_link_libraries(${BOZ_STATEMACHINE_LIB}
    # boost_system
    )
    
# BOZ_INSTALL_ITEM(${BOZ_STATEMACHINE_LIB})

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
    ${BOZ_STATEMACHINE_LIB}
    )
    
# BOZ_INSTALL_ITEM(${BOZ_STATEMACHINE_APP})
