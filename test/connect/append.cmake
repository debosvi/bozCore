
set(TEST_BIN test_connect_append)
add_executable(${TEST_BIN}
    append.c
    )

target_link_libraries(${TEST_BIN}
    bozconnect
    )
