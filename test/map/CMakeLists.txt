cmake_minimum_required(VERSION 3.28.3)

project(test_hm_map)

set(map ../../src/hm_map.c)

set(hash ../../function/hash/hm_hash.c)

set(cmp ../../function/cmp/hm_cmp.c)

set(test test.c)

set(func ../hm_test.c)

set(CMAKE_BUILD_TYPE Debug)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

add_executable(test_hm_map ${test} ${map} ${cmp} ${hash} ${func})

add_test(NAME test_hm_map COMMAND $<TARGET_FILE:test_hm_map>)