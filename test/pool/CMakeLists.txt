cmake_minimum_required(VERSION 3.28.3)

# Set the target container name that will be test, format: hm_{container}
# And use this variable to reduce repetitive container's name 
set(HM_TARGET hm_pool)                  

project(test_${HM_TARGET})              # project name, use the HM_TARGET to replace

set(hm_src ../../src/${HM_TARGET}.c)                # The C file that needs to be tested

set(hm_test test_${HM_TARGET}.c)                     # test file

set(hm_test_tool ../hm_test_tool.c)              # some useful tools for each test part, such as printing test information

# set(hm_functions ../../function/cmp/hm_cmp.c)           # some functions are needed for testing this container


# The output path of executable in remote and local test is the same -- `bin/`
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# The following code is normal build process

set(CMAKE_BUILD_TYPE Debug)

add_executable(test_${HM_TARGET} ${hm_src} ${hm_test} ${hm_test_tool} ${hm_functions})

add_test(NAME test_${HM_TARGET} COMMAND test_${HM_TARGET})