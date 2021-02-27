cmake_minimum_required(VERSION 2.8.11)

if(gtest_SOURCE_DIR)
    message(STATUS "gtest variables already defined. Skipping.")
else()
    add_subdirectory(googletest-1.10.0)
endif()

SET(GTEST_INCLUDE_DIRS ${gtest_SOURCE_DIR}/include)
SET(GTEST_LIBRARY_DIRS ${gtest_BINARY_DIR})
SET(GTEST_BOTH_LIBRARIES gtest gtest_main)
# Force include and link directory
include_directories(${GTEST_INCLUDE_DIRS})
link_directories(${GTEST_LIBRARY_DIRS})

add_subdirectory(unitary-tests)
