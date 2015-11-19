
macro(DBUS_GEN_CPP_ADAPTOR source prefix target)
    set(OUTNAME "${prefix}-adaptor.hpp")
    message(STATUS "Generate DBus CPP adaptor for XML file (${source}) to class (${OUTNAME})")
    add_custom_command(OUTPUT ${OUTNAME}
        COMMAND dbusxx-xml2cpp ${CMAKE_CURRENT_SOURCE_DIR}/${source} --adaptor=${CMAKE_CURRENT_BINARY_DIR}/${OUTNAME}
        DEPENDS ${source}
    )
    add_custom_target(${prefix}_CPP_ADAPTOR
        DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/${OUTNAME}"
    )

    add_dependencies(${target} ${prefix}_CPP_ADAPTOR)
    include_directories(${CMAKE_CURRENT_BINARY_DIR})
endmacro(DBUS_GEN_CPP_ADAPTOR)

macro(DBUS_GEN_CPP_PROXY source prefix target)
    set(OUTNAME "${prefix}-proxy.hpp")
    message(STATUS "Generate DBus CPP proxy for XML file (${source}) to class (${OUTNAME})")
    add_custom_command(OUTPUT ${OUTNAME}
        COMMAND dbusxx-xml2cpp ${CMAKE_CURRENT_SOURCE_DIR}/${source} --proxy=${CMAKE_CURRENT_BINARY_DIR}/${OUTNAME}
        DEPENDS ${source}
    )
    add_custom_target(${prefix}_CPP_PROXY
        DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/${OUTNAME}"
    )

    add_dependencies(${target} ${prefix}_CPP_PROXY)
    include_directories(${CMAKE_CURRENT_BINARY_DIR})
endmacro(DBUS_GEN_CPP_PROXY)


macro(DBUS_GEN_QT_ADAPTOR source prefix target)
    set(OUTNAME "${prefix}-adaptor")
    message(STATUS "Generate DBus QT adaptor for XML file (${source}) to class (${OUTNAME})")
    add_custom_command(OUTPUT ${OUTNAME}
        COMMAND qdbusxml2cpp -a ${CMAKE_CURRENT_BINARY_DIR}/${OUTNAME} ${CMAKE_CURRENT_SOURCE_DIR}/${source}
        DEPENDS ${source}
    )
    add_custom_target(${prefix}_QT_ADAPTOR
        DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/${OUTNAME}"
    )

    add_dependencies(${target} ${prefix}_QT_ADAPTOR)
    include_directories(${CMAKE_CURRENT_BINARY_DIR})
endmacro(DBUS_GEN_QT_ADAPTOR)

macro(DBUS_GEN_QT_PROXY source prefix target)
    set(OUTNAME "${prefix}-proxy")
    message(STATUS "Generate DBus QT proxy for XML file (${source}) to class (${OUTNAME})")
    add_custom_command(OUTPUT ${OUTNAME}
        COMMAND qdbusxml2cpp -p ${CMAKE_CURRENT_BINARY_DIR}/${OUTNAME} ${CMAKE_CURRENT_SOURCE_DIR}/${source}
        DEPENDS ${source}
    )
    add_custom_target(${prefix}_QT_PROXY
        DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/${OUTNAME}"
    )

    add_dependencies(${target} ${prefix}_QT_PROXY)
    include_directories(${CMAKE_CURRENT_BINARY_DIR})
endmacro(DBUS_GEN_QT_PROXY)



