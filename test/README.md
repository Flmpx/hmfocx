# Test Code Format
## Introduce 
- This `README.md` will record how to write test for every container

## **`CMakeLists.txt` Content**

- Location: `test/{container}/`
```cmake
cmake_minimum_required(VERSION 3.28.3)  

project(test_hm_list)       # project name format: `test_hm_{container}`

set(hm_list ../../src/hm_list.c)       # The C file that need to test

set(hm_test test.c)                # test file -- name: must be `test.c`

set(hm_func ../hm_test.c)          # some basic funciton for every test part, like print test information

set(hm_cmp ../../function/cmp/hm_cmp.c)        # some file is needed for this container's test


# The output path of executable in remote and loacal test is the same -- `bin/`
# And you should add commane `-DBUILD_TESTS=ON` in file `.github/workflows/cmake-single-platform.yml` to ensure this test can be run rightly
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)


# The following code is normal build process

set(CMAKE_BUILD_TYPE Debug)

add_executable(test_hm_list ${hm_list} ${hm_test} ${hm_func} ${hm_cmp})

add_test(NAME test_hm_list COMMAND test_hm_list) # register test, name format: `test_hm_{container}`

```
- And you should write a line for `CMakeLists.txt` of root directory, see following code
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

- Location: `test/{container}/`

- **This is a example** with `hm_list` test

### File Structure

- Test include `functional`, `stressful` and `boundary` test. 


| Part    |  Name Format                             |
| ------- | ---------------------------------- |
| `functional test` | `test_{container}_{main content of test}` |
| `boundary test` | `test_{main content of test}`  |
| `stressful test` | `test_{container}_{main content of test}_stress` | 

```c
#include "../../include/hm_list.h"
#include "../../function/cmp/hm_cmp.h"
#include "../hm_test.h"

// This variable can record the all number of failure and it can be used as a return value to check if this test is passed
int all_failure_num = 0;

// use macro to replace the code 
#define HM_TEST_COUNTER \
    all_failure_num += fail_cnt;


// every test function ...

void function_test() {
    test_list_init();                               printf("\n");
    // You should add function of `printf("\n");` after every test done
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
    // Make a rough classification of every test
    function_test();
    boundary_test();
    stress_test();
    return all_failure_num;
}


```

### Every Test Function(`test.c`)

- The format of `HEAD INFO` -- `{Container} | {TYPE OF TEST} | {TEST CONTENT} | {OTHER INFO}
`

- The following code is a example
```c
void test_list_insert_head() {
    
    int fail_cnt = 0;   // record the number of failure of this part
    
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
- You should use `print_run_time` to print infomation in `stress test`
```c

print_run_time("INSERT", start, end, nums[i], nums[i]);

```


- Some brief information of important function, some parameter see [test/hm_test.h](hm_test.h) and [test/hm_test.c](hm_test.c)


| Function | Brief Introduce|
| --------- | ------- |
| `print_run` | Print information of this test part at start|  
| `check_res`| Check if result is true <br> It will `print information` and `change the value of fail_cnt` if `res == false` | 
| `print_end` | Print information of this test  according to the number of `fail_cnt` at end | 
| `print_run_time` | print `cost time` and `speed` according the parameter that pass in |

## Tips
