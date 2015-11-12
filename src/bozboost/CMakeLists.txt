
include_directories(thread)

set(BOZ_BOOST_APP bozApp)

set(SOURCES
    thread/MA5G_ThreadAbstract.cpp
    example/MyAppThread.cpp
    example/MyAppClass.cpp
    example/main.cpp
    )
    
add_executable(${BOZ_BOOST_APP} 
    ${SOURCES}
    )

target_link_libraries(${BOZ_BOOST_APP}
    ${Boost_THREAD_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    pthread
    )
    
add_subdirectory(dbus)
add_subdirectory(shadow)
add_subdirectory(working_thread)
# BOZ_INSTALL_ITEM(${BOZ_BOOST_APP})
