cmake_minimum_required(VERSION 2.8)

set(tool ${PROJECT_NAME}_tests)

############################################################################
#   O P T I O N S
############################################################################
option(CHECK_MEMORY_LEAKS "Make a memory leak analysis in every test" OFF)
if (CHECK_MEMORY_LEAKS)
  add_definitions("-DTEST_MEM_LEAK")
endif()


#############################################################################
#   P A C K A G E S
#############################################################################

find_package(PkgConfig)
find_package (Threads REQUIRED)

include_directories(${GTEST_INCLUDE_DIRS})


#############################################################################
#   L I B R A R I E S
#############################################################################

get_property(GPSLOCATOR GLOBAL PROPERTY GPSLOCATOR)

list(APPEND used_libs ${GPSLOCATOR})


#############################################################################
#   S O U R C E S
#############################################################################

file(GLOB test_srcs "src/*.cpp")
file(GLOB test_module "module/*.cpp")
file(GLOB test_hdrs "include/*.h")
include_directories("include")


#############################################################################
#   E X E C U T A B L E S
#############################################################################

add_executable(${tool} ${test_srcs} ${test_module} ${test_hdrs} )
target_link_libraries(${tool} ${used_libs} ${GTEST_BOTH_LIBRARIES})

add_test(Suite-${tool} ${tool} --gtest_repeat=2)

install (TARGETS ${tool} DESTINATION bin)

