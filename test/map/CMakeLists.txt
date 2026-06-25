cmake_minimum_required(VERSION 3.28.3)

project(test_hm_map)

set(hm_map ../../src/hm_map.c)

set(hm_hash ../../function/hash/hm_hash.c)

set(hm_cmp ../../function/cmp/hm_cmp.c)

set(hm_test test.c)

set(hm_func ../hm_test.c)

set(CMAKE_BUILD_TYPE Debug)

# remote is the same as the local
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

add_executable(test_hm_map ${hm_test} ${hm_map} ${hm_cmp} ${hm_hash} ${hm_func})

add_test(NAME test_hm_map COMMAND test_hm_map)