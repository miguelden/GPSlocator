cmake_minimum_required(VERSION 2.8.11)

set(library "gpslocator")

SET(CMAKE_C_FLAGS " -Wall ${TLSF_FLAGS} ${CMAKE_C_FLAGS}")
set (CMAKE_C_STANDARD 99)

include(lib-version.cmake)
STRING(TOUPPER ${library} LIB)
set_property(GLOBAL PROPERTY ${LIB} ${library}${VERSION_SONAME})
set(VERSION_STRING ${VERSION_CURRENT}.${VERSION_AGE}.${VERSION_REVISION})

#############################################################################
#   P A C K A G E S
#############################################################################
find_package(PkgConfig)

#############################################################################
#   S O U R C E S
#############################################################################

file(GLOB  lib_srcs  "src/*.c")
file(GLOB  lib_hdrs  "include/*.h")

include_directories( include )

#############################################################################
#   E X E C U T A B L E S
#############################################################################

add_library(${library}${VERSION_SONAME} ${lib_mode}
            ${lib_srcs}
            ${lib_hdrs})

target_link_libraries(${library}${VERSION_SONAME} ${used_libs})
set_target_properties(${library}${VERSION_SONAME} PROPERTIES VERSION ${VERSION_STRING} SOVERSION ${VERSION_SONAME})
target_include_directories(${library}${VERSION_SONAME} PUBLIC include)
