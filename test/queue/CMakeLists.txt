cmake_minimum_required(VERSION 3.28.3)  

project(test_hm_queue)       # project name format: `test_hm_{container}`

set(hm_queue ../../src/hm_queue.c)       # The C file that needs to be tested

set(hm_test test.c)                # test file -- name: must be `test.c`

set(hm_func ../hm_test.c)          # some basic function for each test part, such as printing test information



# The output path of executable in remote and local test is the same -- `bin/`
# And you should add command `-DBUILD_TESTS=ON` in file `.github/workflows/cmake-single-platform.yml` to ensure this test can be run correctly
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)


# The following code is normal build process

set(CMAKE_BUILD_TYPE Debug)

add_executable(test_hm_queue ${hm_queue} ${hm_test} ${hm_func})

add_test(NAME test_hm_queue COMMAND test_hm_queue) # register test, name format: `test_hm_{container}`
