cmake_minimum_required(VERSION 3.28.3)

project(hm-focx)

set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

file(GLOB SRC_LIST ${CMAKE_SOURCE_DIR}/src/*.c)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_library(hmfocx STATIC ${SRC_LIST})

option(BUILD_TEST_LIST "Build test programs" OFF)
option(BUILD_TEST_MAP "Build test programs" OFF)

message(STATUS "BUILD_TESTS = ${BUILD_TESTS}")

if (BUILD_TEST_LIST)
    enable_testing()
    add_subdirectory(test/list)
endif()

if (BUILD_TEST_MAP)
    enable_testing()
    add_subdirectory(test/map)
endif()