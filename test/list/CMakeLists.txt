cmake_minimum_required(VERSION 3.28.3)

project(test_hm_list)

set(list ../../src/hm_list.c)

set(test test.c)

set(func ../hm_test.c)

set(cmp ../../function/cmp/hm_cmp.c)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/build)

set(CMAKE_BUILD_TYPE Debug)

add_executable(test_hm_list ${list} ${test} ${func} ${cmp})

add_test(NAME test_hm_list COMMAND $<TARGET_FILE:test_hm_list>)
