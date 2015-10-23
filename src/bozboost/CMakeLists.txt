
set(BOZ_BOOST_APP bozApp)

set(SOURCES
    MA5G_ThreadAbstract.cpp
    MyAppThread.cpp
    MyAppClass.cpp
	main.cpp
    )
    
add_executable(${BOZ_BOOST_APP} 
    ${SOURCES}
    )

target_link_libraries(${BOZ_BOOST_APP}
    boost_thread
	boost_system
	stdc++
    )
    

# BOZ_INSTALL_ITEM(${BOZ_BOOST_APP})
