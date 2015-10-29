
set(OUTNAME GPIO_app)

set(SOURCES 
    GPIO.cpp
    )

add_executable(${OUTNAME} 
    ${SOURCES}
	)

target_link_libraries(${OUTNAME}
    GPIO
    )

