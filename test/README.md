# Test Code Formatting
<p align = "center">
    English | <a href = "./README.zh-CN.md">简体中文</a>
</p>

## Introduction 
- This `README.md` will describe how to write tests for each container

## **`CMakeLists.txt` Content**

- File Location: `test/{container}/`
```cmake
cmake_minimum_required(VERSION 3.28.3)  

project(test_hm_list)       # project name format: `test_hm_{container}`

set(hm_list ../../src/hm_list.c)       # The C file that needs to be tested

set(hm_test test.c)                # test file -- name: must be `test.c`

set(hm_func ../hm_test.c)          # some basic function for each test part, such as printing test information

set(hm_cmp ../../function/cmp/hm_cmp.c)        # some files are needed for testing this container


# The output path of executable in remote and local test is the same -- `bin/`
# And you should add command `-DBUILD_TESTS=ON` in file `.github/workflows/cmake-single-platform.yml` to ensure this test can be run correctly
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)


# The following code is normal build process

set(CMAKE_BUILD_TYPE Debug)

add_executable(test_hm_list ${hm_list} ${hm_test} ${hm_func} ${hm_cmp})

add_test(NAME test_hm_list COMMAND test_hm_list) # register test, name format: `test_hm_{container}`

```
- And you should add a line to the root `CMakeLists.txt`, see following code
```cmake
if (BUILD_TESTS)
    enable_testing()
    # This is new test
    add_subdirectory(test/list) 

    # other test
    add_subdirectory(test/map)
endif()
```



## `test.c` Content

- File Location: `test/{container}/`

- **This is an example** using the `hm_list` test

### File Structure

- Tests include `functional`, `stress` and `boundary` tests. 


| Part    |  Name Format                             |
| ------- | ---------------------------------- |
| `functional test` | `test_{container}_{main content of test}` |
| `boundary test` | `test_{main content of test}`  |
| `stress test` | `test_{container}_{main content of test}_stress` | 

```c
#include "../../include/hm_list.h"
#include "../../function/cmp/hm_cmp.h"
#include "../hm_test.h"

// This variable can record the total number of failures and it can be used as a return value to check whether the test passed
int all_failure_num = 0;

// use a macro to replace the repetitive code 
#define HM_TEST_COUNTER \
    all_failure_num += fail_cnt;


// every test function ...

void function_test() {
    test_list_init();                               printf("\n");
    // You should add `printf("\n");` after each test function call
    // and more ...
}

void boundary_test() {
    test_empty_list_oper();                         printf("\n");
    // and more ...
}

void stress_test() {
    test_list_insert_tail_stress();                 printf("\n");
    // and more ...
}

int main()
{
    // Group the test roughly
    function_test();
    boundary_test();
    stress_test();
    return all_failure_num;
}


```

### Every Test Function(`test.c`)

- The format of `HEAD INFO` -- `{Container} | {TYPE OF TEST} | {TEST CONTENT} | {OTHER INFO}
`

- The following code is an example
```c
void test_list_insert_head() {
    
    int fail_cnt = 0;   // record the total number of failures of this part
    
    // **START**
    print_run("LIST | FUNC | INSERT HEAD | TYPE: [INT]");
    
    int fail = 0;
    
    // some test ...
    
    // **CHECK**
    check_res(fail == 0, "some deail information of faillure", &fail_cnt);
    
    
    // **END**
    print_end("LIST | FUNC | INSERT HEAD | TYPE: [INT]", fail_cnt);
    // recode the all failure number
    HM_TEST_COUNTER
    
}
```
- You should use `print_run_time` to print information in `stress test`
```c

print_run_time("INSERT", start, end, nums[i], nums[i]);

```


- Some brief information about important functions, some parameters in [test/hm_test.h](hm_test.h) and [test/hm_test.c](hm_test.c)


| Function | Brief Desctiption|
| --------- | ------- |
| `print_run` | Print information at the start of a test part|  
| `check_res`| Check if the result is true <br> It will `print information` and `change the value of fail_cnt` if `res == false` | 
| `print_end` | Print information of this test  according to the number of `fail_cnt` at end | 
| `print_run_time` | prints `cost time` and `speed` according to the passed-in parameters |

## Tips
