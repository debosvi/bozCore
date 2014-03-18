
set(TEST_BIN init_fini)
add_executable(${TEST_BIN}
    init_fini.c
    )

target_link_libraries(${TEST_BIN}
    bozmsg
    )
