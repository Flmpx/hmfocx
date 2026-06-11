cmake_minimum_required(VERSION 3.28.3)

project(test_hm_map)

set(map ../../src/hm_map.c)

set(hash ../../function/hash/hm_hash.c)

set(cmp ../../function/cmp/hm_cmp.c)

set(test test.c)

set(func ../hm_test.c)

set(CMAKE_BUILD_TYPE Debug)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

add_executable(test ${test} ${map} ${cmp} ${hash} ${func})