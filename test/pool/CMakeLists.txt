cmake_minimum_required(VERSION 3.28.3)  

project(test_hm_pool)       # project name format: `test_hm_{container}`

set(hm_pool ../../src/hm_pool.c)       # The C file that need to test

set(hm_test test.c)                # test file -- name: must be `test.c`

set(hm_func ../hm_test.c)          # some basic funciton for every test part, like print test information



# The output path of executable in remote and loacal test is the same -- `bin/`
# And you should add commane `-DBUILD_TESTS=ON` in file `.github/workflows/cmake-single-platform.yml` to ensure this test can be run rightly
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)


# The following code is normal build process

set(CMAKE_BUILD_TYPE Debug)

add_executable(test_hm_pool ${hm_pool} ${hm_test} ${hm_func})

add_test(NAME test_hm_pool COMMAND test_hm_pool) # register test, name format: `test_hm_{container}`
