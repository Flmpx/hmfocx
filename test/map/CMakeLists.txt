cmake_minimum_required(VERSION 3.28.3)

set(HM_TARGET hm_map)

project(test_${HM_TARGET})

set(hm_src ../../src/${HM_TARGET}.c)

set(hm_test test_${HM_TARGET}.c)

set(hm_test_tool ../hm_test_tool.c)

set(hm_functions ../../function/cmp/hm_cmp.c
                 ../../function/hash/hm_hash.c
)


# remote is the same as the local
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)


set(CMAKE_BUILD_TYPE Debug)

add_executable(test_${HM_TARGET} ${hm_src} ${hm_test} ${hm_test_tool} ${hm_functions})

add_test(NAME test_${HM_TARGET} COMMAND test_${HM_TARGET})
