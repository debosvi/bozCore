
set(BOZ_TREE_LIB boztree)

set(SOURCES
    boztree_cmp_f.c
    boztree_dtok_f.c
    boztree_free.c
    boztree_id_zero.c
    boztree_init.c
    boztree_zero.c
    )
    
add_library(${BOZ_TREE_LIB} STATIC
    ${SOURCES}
    )

target_link_libraries(${BOZ_TREE_LIB}
    skarnet
    )
    
