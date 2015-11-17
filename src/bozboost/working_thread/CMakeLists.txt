
include_directories(inc)

###############################################################################
###############################################################################
set(BOZ_WORKING_THREAD_LIB working_thread)

set(SOURCES
    src/working_thread.cpp
    src/bulk_thread.cpp
    src/event_status.cpp
    src/thread_os.cpp
    )
    
add_library(${BOZ_WORKING_THREAD_LIB} STATIC
    ${SOURCES}
    )

target_link_libraries(${BOZ_WORKING_THREAD_LIB}
    boost_thread
    boost_system
    pthread
    )
    
# BOZ_INSTALL_ITEM(${BOZ_WORKING_THREAD_LIB})

###############################################################################
###############################################################################
set(BOZ_WORKING_THREAD_APP AppWorkingThread)

set(SOURCES
    test/main.cpp
    )
    
add_executable(${BOZ_WORKING_THREAD_APP} 
    ${SOURCES}
    )

target_link_libraries(${BOZ_WORKING_THREAD_APP}
    working_thread
    )
    
# BOZ_INSTALL_ITEM(${BOZ_WORKING_THREAD_APP})
