
if("${CMAKE_BUILD_TYPE}" STREQUAL "DEBUG")
add_definitions(-DBOZTREE_DEBUG)
endif()

set(BOZ_TREE_LIB boztree)

set(SOURCES
    boztree_check.c
    boztree_cmp_f.c
    boztree_delete.c
    boztree_dtok_f.c
    boztree_free.c
    boztree_id_zero.c
    boztree_init.c
    boztree_insert.c
    boztree_data.c
    boztree_zero.c
    )
    
add_library(${BOZ_TREE_LIB} STATIC
    ${SOURCES}
    )

target_link_libraries(${BOZ_TREE_LIB}
    skarnet
    )
    
