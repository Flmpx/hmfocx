cmake_minimum_required(VERSION 3.28.3)

project(test_hm_list)

set(hm_list ../../src/hm_list.c)

set(hm_test test.c)

set(hm_func ../hm_test.c)

set(hm_cmp ../../function/cmp/hm_cmp.c)

if (BUILD_TESTS) 
    set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/build)
else()    
    set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
endif()

set(CMAKE_BUILD_TYPE Debug)

add_executable(test_hm_list ${hm_list} ${hm_test} ${hm_func} ${hm_cmp})

add_test(NAME test_hm_list COMMAND test_hm_list)
