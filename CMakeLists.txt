cmake_minimum_required(VERSION 3.28.3)

project(hmfocx)

set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)

file(GLOB SRC_LIST ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)

add_library(hmfocx STATIC ${SRC_LIST})

target_include_directories(hmfocx PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

option(BUILD_TESTS "Build test programs" OFF)

if (BUILD_TESTS)
    enable_testing()
    add_subdirectory(test/list)
    add_subdirectory(test/map)
    add_subdirectory(test/pool)
    add_subdirectory(test/stack)
    add_subdirectory(test/queue)
    add_subdirectory(test/heap)
    add_subdirectory(test/set)
endif()