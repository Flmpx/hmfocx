cmake_minimum_required(VERSION 3.28.3)

project(test_hm_list)

set(list ../../src/hm_list.c)

set(test test.c)

set(func ../hm_test.c)

set(cmp ../../function/cmp/hm_cmp.c)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

set(CMAKE_BUILD_TYPE Debug)

add_executable(test ${list} ${test} ${func} ${cmp})
