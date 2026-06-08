cmake_minimum_required(VERSION 3.28.3)

project(test_hm_map)

set(map ../../src/hm_map.c)

set(hash ../../function/hash/hm_hash.c)

set(cmp ../../function/cmp/hm_cmp.c)

set(test test.c)

add_executable(test ${test} ${map} ${cmp} ${hash})