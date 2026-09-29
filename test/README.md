# Test Code Formatting

## Introduction 
This `README.md` will describe how to write tests for each container  

## **`CMakeLists.txt` Content**

File Location: `test/{container}/`  
```cmake

cmake_minimum_required(VERSION 3.28.3)

# Set the target container name that will be test, format: hm_{container}
# And use this variable to reduce repetitive container's name 
set(HM_TARGET hm_list)                  

project(test_${HM_TARGET})              # project name, use the HM_TARGET to replace

set(hm_test test_${HM_TARGET}.c)                     # test file

set(hm_test_tool ../hm_test_tool.c)              # some useful tools for each test part, such as printing test information, cmp and hash

# The output path of executable in remote and local test is the same -- `bin/`
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# The following code is normal build process

set(CMAKE_BUILD_TYPE Debug)

# build use test file and tool file
add_executable(test_${HM_TARGET} ${hm_test} ${hm_test_tool})

target_link_libraries(test_${HM_TARGET} PRIVATE hmfocx)     # use hmfocx library directly

add_test(NAME test_${HM_TARGET} COMMAND test_${HM_TARGET})

```
And you should add a container's name in the root `CMakeLists.txt`, see following code  
Also, you should add it in a file for remote test -- `.github/workflows/cmake-single-platform.yml`  
```cmake
# `MAP` is other container's test 
# `LIST` is the new test
set(
CONTAINERS 
"MAP" "LIST"
)
```



## `test_${container}.c` Content

File Location: `test/{container}/`  

**This is an example** using the `hm_list` test  

### Test Category

Tests include `functional`, `stress` and `boundary` tests  


| Part    |  Name Format                             |
| ------- | ---------------------------------- |
| `functional test` | `test_{container}_{main content of test}` |
| `boundary test` | `test_{main content of test}`  |
| `stress test` | `test_{container}_{main content of test}_stress` | 

```c
#include <hm_list.h>
#include "../hm_test_tool.h"

/* This variable can record the total number of failures and it can be used as a return value to check whether the test passed */
int all_failure_num = 0;

/* use a macro to replace the repetitive code  */
#define HM_TEST_COUNTER \
    all_failure_num += fail_cnt;


/* every test function ... */

void function_test() {
    test_list_init();                               printf("\n");
    /* You should add `printf("\n");` after each test function call */
    /* and more ... */
}

void boundary_test() {
    test_empty_list_oper();                         printf("\n");
    /* and more ... */
}

void stress_test() {
    test_list_insert_tail_stress();                 printf("\n");
    /* and more ... */
}

int main()
{
    /* Group the test roughly */
    function_test();
    boundary_test();
    stress_test();
    
    return all_failure_num;
}


```

### The Content Of Every Test Function(`test_${container}.c`)

The format of `HEAD INFO` -- `{Container} | {TYPE OF TEST} | {TEST CONTENT} | {OTHER INFO}`    

The following code is an example  
```c
void test_list_insert_head() {
    int fail_cnt = 0;   // record the total number of failures of this part
    int tag = 0;
    print_run("LIST | FUNC | INSERT HEAD | TYPE: [INT]");           // **START Block**
    
    
    int fail = 0;
    
    /* some test ... */
    
    /* **CHECK** */
    check_res(fail == 0, "some deail information of faillure", &fail_cnt, tag++);
    
    
    
    print_end("LIST | FUNC | INSERT HEAD | TYPE: [INT]", fail_cnt);     // **END Block**
    HM_TEST_COUNTER         // recode the all failure number
    
}
```
You should use `print_run_time()` to print information in `stress test`, or use `print_speed_vs()` to print the speed comparsion result with other container in the equal scale  
```c

void print_run_time("INSERT", start, end, nums[i], nums[i]);
void print_speed_vs(const char* info_a, clock_t start_a, clock_t end_a,
                    const char* info_b, clock_t start_b, clock_t end_b,
                    size_t scale, size_t oper_cnt);
```


Some brief information about important and auxiliary functions, some parameters in [test/hm_test_tool.h](hm_test_tool.h) and [test/hm_test_tool.c](hm_test_tool.c)  


| Function | Brief Desctiption|
| --------- | ------- |
| `print_run()` | Print information at the start of a test part|  
| `check_res()`| Check if the result is true <br> It will `print information with tag` and `change the value of fail_cnt` if `res == false` | 
| `print_end()` | Print information of this test  according to the number of `fail_cnt` at end | 
| `print_run_time()` | prints `cost time` and `speed` according to the passed-in parameters |
| `print_speed_vs()` | prints `cost time` and `speed`  for each set of passed-in parameters <br> Also, `compare them` |

## How to run test

Go back to root dirctory of this project  
You should to kown these option(**HMFOCX_{CONTAINER}_TEST**) for building use cmake  


| Option | For |
| --- | --- |
| `HMFOCX_LIST_TEST` | hm_list |
| `HMFOCX_MAP_TEST` | hm_map |
| `HMFOCX_POOL_TEST` | hm_pool |
| `HMFOCX_STACK_TEST` | hm_stack |
| `HMFOCX_QUEUE_TEST` | hm_queue |
| `HMFOCX_HEAP_TEST` | hm_heap |
| `HMFOCX_SET_TEST` | hm_set |
| `HMFOCX_ARR_TEST` | hm_arr |
| `HMFOCX_STR_TEST` | hm_str |

And `HMFOCX_BUILD_TEST` is the main option to start test build   

For example, run next command when run list test  

```shell
mkdir build
cd build
cmake -DHMFOCX_BUILD_TEST=ON -DHMFOCX_LIST_TEST=ON ..
make
ctest -V
```


## Tips

>  [!Tip]
>  - You can delete some test group if you think some test is unnecessary, such as `hm_stack` doesn't rellay need stress test
>  - You can add some test group if you think some test is needed
>  - The number of blank lines below of **End Block** (`int fail_cnt = 0;` & `int tag = 0;` & `print_run()` ) and above of **Start Block** ( `print_end()` & `HM_TEST_COUNTER` ) must be greater than `0`