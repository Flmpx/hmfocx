cmake_minimum_required(VERSION 3.28.3)

project(test_hm_list)

set(list ../../src/hm_list.c)

set(test test.c)

add_executable(test ${list} ${test})
