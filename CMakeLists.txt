cmake_minimum_required(VERSION 2.8.11)

#############################################################################
#   Project
#############################################################################

set(package "gpslocator")
project(${package})

option(SHARED_LIBRARY "Disable to make static library" ON)
if(SHARED_LIBRARY)
    set(lib_mode SHARED)
else()
    set(lib_mode STATIC)
endif()

#############################################################################
#   Testing
#############################################################################

option(COVERAGE "Extracts coverage" OFF)
if(COVERAGE)
    SET(CMAKE_C_FLAGS " -g -O0 --coverage -fprofile-arcs -ftest-coverage ${CMAKE_C_FLAGS} ")
    SET(CMAKE_CXX_FLAGS " -g -O0 --coverage -fprofile-arcs -ftest-coverage ${CMAKE_CXX_FLAGS} ")
    MESSAGE(STATUS "Coverage active. USE DEBUG mode and do not release this code!")
endif()

#############################################################################
#   S U B D I R E C T O R I E S
#############################################################################

add_subdirectory(module)
if(NOT DEFINED EXCLUDE_GTEST)
    add_subdirectory(tests)
endif()
