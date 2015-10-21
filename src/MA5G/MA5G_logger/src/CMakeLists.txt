
set(OUTNAME MA5G_logger)

set(SOURCES 
	MA5G_logger.cpp
	)

add_library(${OUTNAME} SHARED
	${SOURCES}
	)

target_link_libraries(${OUTNAME}
	boost_thread
	boost_system
	stdc++
	)

