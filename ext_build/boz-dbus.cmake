
macro(DBUS_GEN_ADAPTOR source prefix target)
    set(OUTNAME "${prefix}-adaptor.hpp")
    message(STATUS "Generate DBus adaptor for XML file (${source}) to class (${OUTNAME})")
    add_custom_command(OUTPUT ${OUTNAME}
        COMMAND dbusxx-xml2cpp ${CMAKE_CURRENT_SOURCE_DIR}/${source} --adaptor=${CMAKE_CURRENT_BINARY_DIR}/${OUTNAME}
        DEPENDS ${source}
    )
    add_custom_target(${prefix}_ADAPTOR
        DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/${OUTNAME}"
    )

    add_dependencies(${target} ${prefix}_ADAPTOR)
    include_directories(${CMAKE_CURRENT_BINARY_DIR})
endmacro(DBUS_GEN_ADAPTOR)

macro(DBUS_GEN_PROXY source prefix target)
    set(OUTNAME "${prefix}-proxy.hpp")
    message(STATUS "Generate DBus proxy for XML file (${source}) to class (${OUTNAME})")
    add_custom_command(OUTPUT ${OUTNAME}
        COMMAND dbusxx-xml2cpp ${CMAKE_CURRENT_SOURCE_DIR}/${source} --proxy=${CMAKE_CURRENT_BINARY_DIR}/${OUTNAME}
        DEPENDS ${source}
    )
    add_custom_target(${prefix}_PROXY
        DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/${OUTNAME}"
    )

    add_dependencies(${target} ${prefix}_PROXY)
    include_directories(${CMAKE_CURRENT_BINARY_DIR})
endmacro(DBUS_GEN_PROXY)


