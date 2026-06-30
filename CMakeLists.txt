cmake_minimum_required(VERSION 3.28.3)

project(hm-focx)

set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

file(GLOB SRC_LIST ${CMAKE_SOURCE_DIR}/src/*.c)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_library(hmfocx STATIC ${SRC_LIST})

option(BUILD_TESTS "Build test programs" OFF)

if (BUILD_TESTS)
    enable_testing()
    add_subdirectory(test/list)
    add_subdirectory(test/map)
    add_subdirectory(test/pool)
    add_subdirectory(test/stack)
endif()