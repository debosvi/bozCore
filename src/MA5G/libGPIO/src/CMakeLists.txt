
set(OUTNAME GPIO)

set(SOURCES 
    GPIO_Exception.cpp
    Lib_GPIO.cpp
    )

add_library(${OUTNAME} SHARED
    ${SOURCES}
	)

target_link_libraries(${OUTNAME}
    MEP_logger
    ${Boost_THREAD_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    )

