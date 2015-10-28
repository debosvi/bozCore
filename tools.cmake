
#set(CMAKE_PREFIX_PATH /usr/share/lib/cmake)
#
#find_package(Skarnet REQUIRED)
#message(STATUS "Skarnet found: " ${SKARNET_VERSION_STRING})
#include_directories(${SKARNET_INCLUDE_DIRS})
#link_directories(${SKARNET_LIBRARY_DIRS})

find_package(Boost REQUIRED COMPONENTS thread system)
include_directories(${Boost_INCLUDE_DIRS})
link_libraries(${Boost_LIBRARY_DIRS})

find_package(DBusCpp REQUIRED )
include_directories(${DBusCpp_INCLUDE_DIRS})
link_libraries(${DBusCpp_LIBRARY_DIRS})

