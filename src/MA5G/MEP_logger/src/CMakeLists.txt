
set(OUTNAME MEP_logger)

set(SOURCES 
	rollingpolicy_type_complete.c
	layout_type_encrypted.c
	appender_type_socket.c
	MEP_log_rc.c
	MEP_logger.c
	MEP_logger_init.c
	)

add_library(${OUTNAME} SHARED
	${SOURCES}
	)

target_link_libraries(${OUTNAME}
	log4c
	expat
	crypto
	)

