/*
 * Copyright (c) 2026 Flmpx
 * Licensed under MIT (see LICENSE).
 */


#include "../../include/hm_arr.h"
#include "../hm_test_tool.h"
#include <stdlib.h>

// This variable can record the total number of failures and it can be used as a return value to check whether the test passed
int all_failure_num = 0;

// use a macro to replace the repetitive code 
#define HM_TEST_COUNTER \
    all_failure_num += fail_cnt;


// every test function ...


void test_arr_integrity(hm_arr* arr, int* fail_cnt, int tag, size_t size, bool dynamic_grow, size_t capacity, hm_free free_val) {
    check_res(arr->size == size, "TEST OF INTEGRITY: arr's size isn't the expected size", fail_cnt, tag);
    check_res(arr->dynamic_grow == dynamic_grow, "TEST OF INTEGRITY: arr's dynamic signal is unexpected", fail_cnt, tag);
    check_res(arr->free_val == free_val, "TEST OF INTEGRITY: arr's free_val is unexpected", fail_cnt, tag);
    if (!dynamic_grow) {
        check_res(arr->capacity == capacity, "TEST OF INTEGRITY: arr's capacity isn't the expected size", fail_cnt, tag);
    }
    check_res(arr->capacity >= arr->size, "TEST OF INTEGRITY: `capacity` should greater than `size`", fail_cnt, tag);
    check_res(!(arr->capacity == 0 && arr->vals != NULL), "TEST OF INTEGRITY: arr's capacity is 0, but vals have memory", fail_cnt, tag);
    check_res(!(arr->capacity != 0 && arr->vals == NULL), "TEST OF INTEGRITY: arr's capacity isn't 0, but vals is NULL", fail_cnt, tag);
    
}

void test_arr_fixed_init() {
    int fail_cnt = 0;
    int tag = 0;
    print_run("ARR(FIXED) | FUNC | INIT | CAPACITY: 64");
    
    hm_arr arr;
    size_t capacity = 64;
    
    // pass in `free` for arr
    hm_arr_init(&arr, capacity, free);
    test_arr_integrity(&arr, &fail_cnt, tag++, 0, false, capacity, free);

    check_res(arr.capacity == capacity, "the arr's capacity isn't to expected capacity", &fail_cnt, tag++);
    check_res(arr.dynamic_grow == false, "the arr's dynamic-grow signal is false", &fail_cnt, tag++);
    check_res(arr.free_val == free, "the arr's free_val should be `free` when pass in `free` to arr", &fail_cnt, tag++);
    check_res(arr.size == 0, "the arr's size should be 0", &fail_cnt, tag++);
    check_res(arr.vals != NULL, "the arr's vals shouldn't be NULL with a small capacity", &fail_cnt, tag++);

    hm_arr_free(&arr);

    // pass in NULL for arr

    hm_arr_init(&arr, capacity, NULL);
    test_arr_integrity(&arr, &fail_cnt, tag++, 0, false, capacity, NULL);
    check_res(arr.free_val == NULL, "the arr's free_val should be `NULL` when pass in `NULL` to arr", &fail_cnt, tag++);

    hm_arr_free(&arr);

    print_end("ARR(FIXED) | FUNC | INIT | CAPACITY: 64", fail_cnt);
    HM_TEST_COUNTER
    
}

void test_arr_dynamic_init() {
    int fail_cnt = 0;
    int tag = 0;
    print_run("ARR(DYNAMIC) | FUNC | INIT | CAPACITY: 64");
    
    hm_arr arr;
    size_t capacity = 64;
    
    // pass in `free` for arr
    hm_arr_init_dynamic_grow(&arr, capacity, free);
    test_arr_integrity(&arr, &fail_cnt, tag++, 0, true, capacity, free);
    
    check_res(arr.capacity == capacity, "the arr's capacity isn't to expected capacity", &fail_cnt, tag++);
    check_res(arr.dynamic_grow == true, "the arr's dynamic-grow signal is true", &fail_cnt, tag++);
    check_res(arr.free_val == free, "the arr's free_val should be `free` when pass in `free` to arr", &fail_cnt, tag++);
    check_res(arr.size == 0, "the arr's size should be 0", &fail_cnt, tag++);
    check_res(arr.vals != NULL, "the arr's vals shouldn't be NULL with a small capacity", &fail_cnt, tag++);
    
    hm_arr_free(&arr);
    
    // pass in NULL for arr
    
    hm_arr_init_dynamic_grow(&arr, capacity, NULL);
    test_arr_integrity(&arr, &fail_cnt, tag++, 0, true, capacity, NULL);
    check_res(arr.free_val == NULL, "the arr's free_val should be `NULL` when pass in `NULL` to arr", &fail_cnt, tag++);
    
    hm_arr_free(&arr);
    
    print_end("ARR(DYNAMIC) | FUNC | INIT | CAPACITY: 64", fail_cnt);
    HM_TEST_COUNTER
    
}


void test_arr_fixed_insert_head() {
    int fail_cnt = 0;
    int tag = 0;
    print_run("ARR(FIXED) | FUNC | INSERT HEAD | CAPACITY: 64");

    int capacity = 64;
    hm_arr arr;
    hm_arr_init(&arr, capacity, free);

    // insert head
    int fail = 0;
    for (int i = 0; i < capacity; i++) {
        int* v = (int*)malloc(sizeof(int));
        *v = i * 100;
        if (hm_arr_insert_head(&arr, v) != hm_arr_ret_suc) {
            fail++;
        }
    }
    check_res(fail == 0, "insert function should return suc", &fail_cnt, tag++);
    test_arr_integrity(&arr, &fail_cnt, tag++, capacity, false, capacity, free);

    // verify
    int fail_NULL = 0;
    int fail_diff = 0;
    int** vals = (int**)arr.vals;
    for (int i = 0; i < capacity; i++) {
        int* v = vals[i];
        if (v == NULL) {
            fail_NULL++;
        } else if (*v != (capacity - i - 1) * 100) {
            fail_diff++;
        }
    }
    check_res(fail_NULL == 0, "the pointer of val in arr shouldn't be NULL", &fail_cnt, tag++);
    check_res(fail_diff == 0, "val in arr is wrong", &fail_cnt, tag++);

    hm_arr_free(&arr);

    print_end("ARR(FIXED) | FUNC | INSERT HEAD | CAPACITY: 64", fail_cnt);
    HM_TEST_COUNTER

}


void test_arr_dynamic_insert_head() {
    int fail_cnt = 0;
    int tag = 0;
    print_run("ARR(DYNAMIC) | FUNC | INSERT HEAD | CAPACITY: 64");

    int start_capacity = 64;
    hm_arr arr;
    hm_arr_init_dynamic_grow(&arr, start_capacity, free);

    // insert head
    int fail = 0;
    for (int i = 0; i < start_capacity * 2; i++) {
        int* v = (int*)malloc(sizeof(int));
        *v = i * 100;
        if (hm_arr_insert_head(&arr, v) != hm_arr_ret_suc) {
            fail++;
        }
    }
    check_res(fail == 0, "insert function should return suc", &fail_cnt, tag++);
    test_arr_integrity(&arr, &fail_cnt, tag++, start_capacity * 2, true, start_capacity, free);

    // verify
    int fail_NULL = 0;
    int fail_diff = 0;
    int** vals = (int**)arr.vals;
    for (int i = 0; i < start_capacity * 2; i++) {
        int* v = vals[i];
        if (v == NULL) {
            fail_NULL++;
        } else if (*v != (start_capacity * 2 - i - 1) * 100) {
            fail_diff++;
        }
    }
    check_res(fail_NULL == 0, "the pointer of val in arr shouldn't be NULL", &fail_cnt, tag++);
    check_res(fail_diff == 0, "val in arr is wrong", &fail_cnt, tag++);

    hm_arr_free(&arr);

    print_end("ARR(DYNAMIC) | FUNC | INSERT HEAD | CAPACITY: 64", fail_cnt);
    HM_TEST_COUNTER
}





void test_arr_fixed_func() {
    test_arr_fixed_init();                                                              printf("\n");

    test_arr_fixed_insert_head();                                                       printf("\n");


}


void test_arr_dynamic_func() {
    test_arr_dynamic_init();                                                            printf("\n");

    test_arr_dynamic_insert_head();                                                     printf("\n");

}

void function_test() {
    test_arr_fixed_func();

    test_arr_dynamic_func();    
}

void boundary_test() {
    
}


int main()
{
    // Group the test roughly
    function_test();
    boundary_test();
    return all_failure_num;
}

