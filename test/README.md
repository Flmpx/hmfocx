# Test Code Format

## **File `CMakeLists.txt` Content**

- Location: `test/{container}/`
```cmake
cmake_minimum_required(VERSION 3.28.3)  

project(test_hm_list)       # project name format: `test_hm_{container}`

set(list ../../src/hm_list.c)       # The C file that need to test

set(test test.c)                # test file -- name: must be `test.c`

set(func ../hm_test.c)          # some basic funciton for every test part, like print test information

set(cmp ../../function/cmp/hm_cmp.c)        # some file is needed for this container's test


# In GitHub Action, the path of output `build`. In local, it is `bin`
# And you should add commane `-DBUILD_TESTS=ON` in file `.github/workflows/cmake-single-platform.yml` to ensure this test can be run rightly
if (BUILD_TESTS) 
    set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/build)
else()    
    set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
endif()


# The following code is normal build process

set(CMAKE_BUILD_TYPE Debug)

add_executable(test_hm_list ${list} ${test} ${func} ${cmp})

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



## File `test.c` Content

- Location: `test/{container}/`

### Main Test(`test.c`)


**This is a example** with `hm_list` test
```c
int main()
{
    function_test();

    boundary_test();

    stress_test();

    return all_failure_num;
}
```
1. **Function Test**
- The format of every test funciton's name is `test_{container}_{main content of test}`

```c
void function_test() {
    test_list_init();                               printf("\n");
    // You should add function of `printf("\n");` after every test done
    
    test_list_insert_head();                        printf("\n");
    test_list_insert_tail();                        printf("\n");
    test_list_insert_index();                       printf("\n");
    
    test_iter_list();                               printf("\n");
    
    // and more ...

}
```

2. **Boundary Test**

- The format of every test funciton's name is `test_{main content of test}`

```c
void boundary_test() {
    test_empty_list_oper();                         printf("\n");

    test_freed_list_oper();                         printf("\n");

    test_single_listnode_oper();                    printf("\n");

}
```

3. **Stress Test**
- The format of every test funciton's name is `test_{container}_{main content of test}_stress`
```c
void stress_test() {

    test_list_insert_tail_stress();                 printf("\n");
    
    test_list_insert_head_stress();                 printf("\n");

    test_list_insert_index_stress();                printf("\n");
    
    // and more ...

}
```
