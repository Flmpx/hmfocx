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




void test_arr_fixed_insert_tail() {
    int fail_cnt = 0;
    int tag = 0;
    print_run("ARR(FIXED) | FUNC | INSERT TAIL | CAPACITY: 64");

    int capacity = 64;
    hm_arr arr;
    hm_arr_init(&arr, capacity, free);

    // insert tail
    int fail = 0;
    for (int i = 0; i < capacity; i++) {
        int* v = (int*)malloc(sizeof(int));
        *v = i * 100;
        if (hm_arr_insert_tail(&arr, v) != hm_arr_ret_suc) {
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
        } else if (*v != i * 100) {
            fail_diff++;
        }
    }
    check_res(fail_NULL == 0, "the pointer of val in arr shouldn't be NULL", &fail_cnt, tag++);
    check_res(fail_diff == 0, "val in arr is wrong", &fail_cnt, tag++);

    hm_arr_free(&arr);

    print_end("ARR(FIXED) | FUNC | INSERT TAIL | CAPACITY: 64", fail_cnt);
    HM_TEST_COUNTER

}


void test_arr_dynamic_insert_tail() {
    int fail_cnt = 0;
    int tag = 0;
    print_run("ARR(DYNAMIC) | FUNC | INSERT TAIL | CAPACITY: 64");

    int start_capacity = 64;
    hm_arr arr;
    hm_arr_init_dynamic_grow(&arr, start_capacity, free);

    // insert tail
    int fail = 0;
    for (int i = 0; i < start_capacity * 2; i++) {
        int* v = (int*)malloc(sizeof(int));
        *v = i * 100;
        if (hm_arr_insert_tail(&arr, v) != hm_arr_ret_suc) {
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
        } else if (*v != i * 100) {
            fail_diff++;
        }
    }
    check_res(fail_NULL == 0, "the pointer of val in arr shouldn't be NULL", &fail_cnt, tag++);
    check_res(fail_diff == 0, "val in arr is wrong", &fail_cnt, tag++);

    hm_arr_free(&arr);

    print_end("ARR(DYNAMIC) | FUNC | INSERT TAIL | CAPACITY: 64", fail_cnt);
    HM_TEST_COUNTER
}

void test_arr_fixed_insert_index() {
    int fail_cnt = 0;
    int tag = 0;
    print_run("ARR(FIXED) | FUNC | INSERT INDEX | CAPACITY: 8");

    int capacity = 8;
    hm_arr arr;
    hm_arr_init(&arr, capacity, free);

    size_t indexs[] = {2, 3, 1, 0, 1, 2, 3, 4, 5, 5, 6, 6, 6, 7, 12, 12, 24};
    int cnt = sizeof(indexs) / sizeof(size_t);

    int fail_full = 0;
    int fail_invalid_index = 0;
    int fail_diff = 0;
    int fail_normal = 0;

    int real_size = 0;
    for (int i = 0; i < cnt; i++) {
        int* v = (int*)malloc(sizeof(int));
        // this can prevent arr existed same number
        *v = i;
        
        size_t prev_s = hm_arr_size(&arr);

        hm_arr_ret ret = hm_arr_insert_index(&arr, v, indexs[i]);

    
        
        if (indexs[i] <= prev_s) {

            if (prev_s < capacity) {
                // normal situation but return tag wrong
                if (ret != hm_arr_ret_suc) {
                    fail_normal++;
                }
                int** vals = (int**)arr.vals;
                int* val = vals[indexs[i]];
                if (val != v) {
                    fail_diff++;
                }
                real_size++;
                test_arr_integrity(&arr, &fail_cnt, tag++, real_size, false, capacity, free);

            } else {
                // arr full
                if (ret != hm_arr_ret_full) {
                    fail_full++;
                } else {
                    free(v);
                }
                test_arr_integrity(&arr, &fail_cnt, tag++, real_size, false, capacity, free);
            }
            
        } else {
            // index invalid
            if (ret != hm_arr_ret_warn) {
                fail_invalid_index++;
            } else {
                free(v);
            }
            test_arr_integrity(&arr, &fail_cnt, tag++, real_size, false, capacity, free);
        }

    }
    

    check_res(fail_invalid_index == 0, "the function should return warn when index is out of bound", &fail_cnt, tag++);
    check_res(fail_full == 0, "the function should return full when arr is full and index is right", &fail_cnt, tag++);
    check_res(fail_normal == 0, "the function shoudld return suc when insert suc", &fail_cnt, tag++);
    check_res(fail_diff == 0, "the val in arr is wrong", &fail_cnt, tag++);

    hm_arr_free(&arr);

    print_end("ARR(FIXED) | FUNC | INSERT INDEX | CAPACITY: 8", fail_cnt);
    HM_TEST_COUNTER
}


void test_arr_dynamic_insert_index() {
    int fail_cnt = 0;
    int tag = 0;
    print_run("ARR(DYNAMIC) | FUNC | INSERT INDEX | CAPACITY: 8");

    int start_capacity = 8;
    hm_arr arr;
    hm_arr_init_dynamic_grow(&arr, start_capacity, free);

    size_t indexs[] = {2, 3, 1, 0, 1, 2, 3, 4, 1111, 520,  5, 5, 6, 6, 1314, 3, 3, 3, 3, 3, 3, 3, 3, 5, 2, 0, 1, 3, 1, 4, 6, 7, 12, 12, 24};
    int cnt = sizeof(indexs) / sizeof(size_t);

    int fail_invalid_index = 0;
    int fail_diff = 0;
    int fail_normal = 0;

    int real_size = 0;
    for (int i = 0; i < cnt; i++) {
        int* v = (int*)malloc(sizeof(int));
        // this can prevent arr existed same number
        *v = i;
        
        size_t prev_s = hm_arr_size(&arr);

        hm_arr_ret ret = hm_arr_insert_index(&arr, v, indexs[i]);

        if (indexs[i] <= prev_s) {

            // normal situation but return tag wrong
            if (ret != hm_arr_ret_suc) {
                fail_normal++;
            }
            int** vals = (int**)arr.vals;
            int* val = vals[indexs[i]];
            if (val != v) {
                fail_diff++;
            }
            real_size++;
            test_arr_integrity(&arr, &fail_cnt, tag++, real_size, true, start_capacity, free);
            
        } else {
            // index invalid
            if (ret != hm_arr_ret_warn) {
                fail_invalid_index++;
            } else {
                free(v);
            }
            test_arr_integrity(&arr, &fail_cnt, tag++, real_size, true, start_capacity, free);
        }

    }

    check_res(fail_invalid_index == 0, "the function should return warn when index is out of bound", &fail_cnt, tag++);
    check_res(fail_normal == 0, "the function shoudld return suc when insert suc", &fail_cnt, tag++);
    check_res(fail_diff == 0, "the val in arr is wrong", &fail_cnt, tag++);

    hm_arr_free(&arr);

    print_end("ARR(DYNAMIC) | FUNC | INSERT INDEX | CAPACITY: 8", fail_cnt);
    HM_TEST_COUNTER
}


void test_arr_fixed_get() {
    int fail_cnt = 0;
    int tag = 0;
    print_run("ARR(FIXED) | FUNC | GET | CAPACITY: 64");

    int capacity = 64;
    hm_arr arr;
    hm_arr_init(&arr, capacity, free);
    // insert
    for (int i = 0; i < capacity; i++) {
        int* v = (int*)malloc(sizeof(int));
        *v = i;
        hm_arr_insert_tail(&arr, v);
    }
    // get and verify[valid]
    int fail_NULL = 0;
    int fail_diff = 0;
    for (int i = 0; i < capacity; i++) {
        int* v = hm_arr_get(&arr, i);
        if (v == NULL) {
            fail_NULL++;
        } else if (*v != i) {
            fail_diff++;
        }
    }
    test_arr_integrity(&arr, &fail_cnt, tag++, capacity, false, capacity, free);
    check_res(fail_NULL == 0, "val got by `get` shouldn't be NULL when index is right", &fail_cnt, tag++);
    check_res(fail_diff == 0, "val got by `get` is wrong", &fail_cnt, tag++);
    
    // get and verfiy[invalid]
    int fail_exist = 0;
    for (int i = capacity; i < capacity * 2; i++) {
        int* v = hm_arr_get(&arr, i);
        if (v) {
            fail_exist++;
        }
    }
    test_arr_integrity(&arr, &fail_cnt, tag++, capacity, false, capacity, free);
    check_res(fail_exist == 0, "val got by `get` should be NULL when index is out of booud", &fail_cnt, tag++);

    hm_arr_free(&arr);

    print_end("ARR(FIXED) | FUNC | GET | CAPACITY: 64", fail_cnt);
    HM_TEST_COUNTER

}

void test_arr_dynamic_get() {
    int fail_cnt = 0;
    int tag = 0;
    print_run("ARR(DYNAMIC) | FUNC | GET | CAPACITY: 64");

    int start_capacity = 64;
    hm_arr arr;
    hm_arr_init_dynamic_grow(&arr, start_capacity, free);
    // insert
    for (int i = 0; i < start_capacity * 2; i++) {
        int* v = (int*)malloc(sizeof(int));
        *v = i;
        hm_arr_insert_tail(&arr, v);
    }
    // get and verify[valid]
    int fail_NULL = 0;
    int fail_diff = 0;
    for (int i = 0; i < start_capacity * 2; i++) {
        int* v = hm_arr_get(&arr, i);
        if (v == NULL) {
            fail_NULL++;
        } else if (*v != i) {
            fail_diff++;
        }
    }
    test_arr_integrity(&arr, &fail_cnt, tag++, start_capacity * 2, true, start_capacity, free);
    check_res(fail_NULL == 0, "val's pointer got by `get` shouldn't be NULL when index is right", &fail_cnt, tag++);
    check_res(fail_diff == 0, "val got by `get` is wrong", &fail_cnt, tag++);
    
    // get and verfiy[invalid]
    int fail_exist = 0;
    for (int i = start_capacity * 2; i < start_capacity * 4; i++) {
        int* v = hm_arr_get(&arr, i);
        if (v) {
            fail_exist++;
        }
    }
    test_arr_integrity(&arr, &fail_cnt, tag++, start_capacity * 2, true, start_capacity, free);
    check_res(fail_exist == 0, "val't pointer got by `get` should be NULL when index is out of booud", &fail_cnt, tag++);

    hm_arr_free(&arr);

    print_end("ARR(DYNAMIC) | FUNC | GET | CAPACITY: 64", fail_cnt);
    HM_TEST_COUNTER

}


void test_arr_fixed_get_pointer() {
    int fail_cnt = 0;
    int tag = 0;
    print_run("ARR(FIXED) | FUNC | GET POINTER | CAPACITY: 64");

    int capacity = 64;
    hm_arr arr;
    hm_arr_init(&arr, capacity, free);
    // insert
    int* flag[capacity];    // store the pointer to the val
    for (int i = 0; i < capacity; i++) {
        int* v = (int*)malloc(sizeof(int));
        *v = i;
        flag[i] = v;
        hm_arr_insert_tail(&arr, v);
    }
    // get and verify[valid]
    int fail_NULL = 0;
    int fail_diff = 0;
    for (int i = 0; i < capacity; i++) {
        int** v = (int**)hm_arr_get_pointer(&arr, i);
        if (v == NULL) {
            fail_NULL++;
        } else if (*v != flag[i]) {
            fail_diff++;
        }
    }
    test_arr_integrity(&arr, &fail_cnt, tag++, capacity, false, capacity, free);
    check_res(fail_NULL == 0, "the pointer to val's pointer got by `get_pointer` shouldn't be NULL when index is right", &fail_cnt, tag++);
    check_res(fail_diff == 0, "val's pointer got by `get_pointer` is wrong", &fail_cnt, tag++);
    
    // get and verfiy[invalid]
    int fail_exist = 0;
    for (int i = capacity; i < capacity * 2; i++) {
        int** v = (int**)hm_arr_get_pointer(&arr, i);
        if (v) {
            fail_exist++;
        }
    }
    test_arr_integrity(&arr, &fail_cnt, tag++, capacity, false, capacity, free);
    check_res(fail_exist == 0, "the pointer to val's pointer got by `get` should be NULL when index is out of booud", &fail_cnt, tag++);

    hm_arr_free(&arr);

    print_end("ARR(FIXED) | FUNC | GET POINTER | CAPACITY: 64", fail_cnt);
    HM_TEST_COUNTER

}

void test_arr_dynamic_get_pointer() {
    int fail_cnt = 0;
    int tag = 0;
    print_run("ARR(DYNAMIC) | FUNC | GET POINTER| CAPACITY: 64");

    int start_capacity = 64;
    hm_arr arr;
    hm_arr_init_dynamic_grow(&arr, start_capacity, free);
    // insert
    int* flag[start_capacity * 2];    // store the pointer to the val
    for (int i = 0; i < start_capacity * 2; i++) {
        int* v = (int*)malloc(sizeof(int));
        *v = i;
        flag[i] = v;
        hm_arr_insert_tail(&arr, v);
    }
    // get and verify[valid]
    int fail_NULL = 0;
    int fail_diff = 0;
    for (int i = 0; i < start_capacity * 2; i++) {
        int** v = (int**)hm_arr_get_pointer(&arr, i);
        if (v == NULL) {
            fail_NULL++;
        } else if (*v != flag[i]) {
            fail_diff++;
        }
    }
    test_arr_integrity(&arr, &fail_cnt, tag++, start_capacity * 2, true, start_capacity, free);
    check_res(fail_NULL == 0, "the pointer to val's pointer got by `get_pointer` shouldn't be NULL when index is right", &fail_cnt, tag++);
    check_res(fail_diff == 0, "val's pointer got by `get_pointer` is wrong", &fail_cnt, tag++);
    
    // get and verfiy[invalid]
    int fail_exist = 0;
    for (int i = start_capacity * 2; i < start_capacity * 4; i++) {
        int** v = (int**)hm_arr_get_pointer(&arr, i);
        if (v) {
            fail_exist++;
        }
    }
    test_arr_integrity(&arr, &fail_cnt, tag++, start_capacity * 2, true, start_capacity, free);
    check_res(fail_exist == 0, "the pointer to val's pointer got by `get` should be NULL when index is out of booud", &fail_cnt, tag++);

    hm_arr_free(&arr);

    print_end("ARR(DYNAMIC) | FUNC | GET POINTER | CAPACITY: 64", fail_cnt);
    HM_TEST_COUNTER

}

void test_arr_fixed_change() {
    int fail_cnt = 0;
    int tag = 0;
    print_run("ARR(FIXED) | FUNC | CHANGE | CAPACITY: 64");

    int capacity = 64;
    hm_arr arr;
    hm_arr_init(&arr, capacity, free);

    // insert
    for (int i = 0; i < capacity; i++) {
        int* v = (int*)malloc(sizeof(int));
        *v = i;
        hm_arr_insert_tail(&arr, v);
    }
    
    //  change [use `hm_arr_get`]
    int diff = 100;
    for (int i = 0; i < capacity; i++) {
        int* v = hm_arr_get(&arr, i);
        *v += diff;
    }
    
    // verify
    int fail_diff = 0; 
    for (int i = 0; i < capacity; i++) {
        int* v = hm_arr_get(&arr, i);
        if (*v != i + diff) {
            fail_diff++;
        }
    }
    check_res(fail_diff == 0, "the val change by `get` is wrong", &fail_cnt, tag++);
    test_arr_integrity(&arr, &fail_cnt, tag++, capacity, false, capacity, free);

    hm_arr_free(&arr);


    hm_arr_init(&arr, capacity, NULL);
    int flag[capacity];         

    // insert
    for (int i = 0; i < capacity; i++) {
        hm_arr_insert_tail(&arr, NULL);
    }

    // change the pointer
    for (int i = 0; i < capacity; i++) {
        int** v_p = (int**)hm_arr_get_pointer(&arr, i);
        *v_p = &flag[i];
    }

    // verify
    fail_diff = 0;
    for (int i = 0; i < capacity; i++) {
        int* v = hm_arr_get(&arr, i);
        if (v != &flag[i]) {
            fail_diff++;
        }
    }
    check_res(fail_diff == 0, "the pointer to the val change by `get_pointer` is wrong", &fail_cnt, tag++);
    test_arr_integrity(&arr, &fail_cnt, tag++, capacity, false, capacity, NULL);

    hm_arr_free(&arr);

    print_end("ARR(FIXED) | FUNC | CHANGE | CAPACITY: 64", fail_cnt);
    HM_TEST_COUNTER
}


void test_arr_dynamic_change() {
    int fail_cnt = 0;
    int tag = 0;
    print_run("ARR(DYNAMIC) | FUNC | CHANGE | CAPACITY: 64");

    int start_capacity = 64;
    hm_arr arr;
    hm_arr_init_dynamic_grow(&arr, start_capacity, free);

    // insert
    for (int i = 0; i < start_capacity * 2; i++) {
        int* v = (int*)malloc(sizeof(int));
        *v = i;
        hm_arr_insert_tail(&arr, v);
    }
    
    //  change [use `hm_arr_get`]
    int diff = 100;
    for (int i = 0; i < start_capacity * 2; i++) {
        int* v = hm_arr_get(&arr, i);
        *v += diff;
    }
    
    // verify
    int fail_diff = 0; 
    for (int i = 0; i < start_capacity * 2; i++) {
        int* v = hm_arr_get(&arr, i);
        if (*v != i + diff) {
            fail_diff++;
        }
    }
    check_res(fail_diff == 0, "the val change by `get` is wrong", &fail_cnt, tag++);
    test_arr_integrity(&arr, &fail_cnt, tag++, start_capacity * 2, true, start_capacity, free);

    hm_arr_free(&arr);


    hm_arr_init_dynamic_grow(&arr, start_capacity, NULL);
    int flag[start_capacity * 2];         

    // insert
    for (int i = 0; i < start_capacity * 2; i++) {
        hm_arr_insert_tail(&arr, NULL);
    }

    // change the pointer
    for (int i = 0; i < start_capacity * 2; i++) {
        int** v_p = (int**)hm_arr_get_pointer(&arr, i);
        *v_p = &flag[i];
    }

    // verify
    fail_diff = 0;
    for (int i = 0; i < start_capacity *2; i++) {
        int* v = hm_arr_get(&arr, i);
        if (v != &flag[i]) {
            fail_diff++;
        }
    }
    check_res(fail_diff == 0, "the pointer to the val change by `get_pointer` is wrong", &fail_cnt, tag++);
    test_arr_integrity(&arr, &fail_cnt, tag++, start_capacity * 2, true, start_capacity, NULL);

    hm_arr_free(&arr);

    print_end("ARR(DYNAMIC) | FUNC | CHANGE | CAPACITY: 64", fail_cnt);
    HM_TEST_COUNTER
}

void test_arr_fixed_pop() {
    int fail_cnt = 0;
    int tag = 0;
    print_run("ARR(FIXED) | FUNC | POP | CAPACITY: 64");

    int capacity = 64;
    hm_arr arr;
    hm_arr_init(&arr, capacity, free);

    // insert
    for (int i = 0; i < capacity; i++) {
        int* v = (int*)malloc(sizeof(int));
        *v = i;
        hm_arr_insert_tail(&arr, v);
    }
    // pop
    size_t pop_indexs[] = {2, 1, 65, 99, 0, 45, 2, 4 * capacity, 4, 10 * capacity, 4, 23, 200, 420, 520, 9, 1314};
    int num = sizeof(pop_indexs) / sizeof(size_t);
    int* pop_v[num];


    int cnt = 0;
    int fail_invalid_index = 0;
    int fail_valid_index = 0;
    for (int i = 0; i < num; i++) {
        size_t s = hm_arr_size(&arr);
        int* v = hm_arr_pop(&arr, pop_indexs[i]);
        if (pop_indexs[i] < s) {
            if (v == NULL) {
                // valid index but pop invalid val
                fail_valid_index++;
            } else {
                pop_v[cnt++] = v;
            }
        } else if (v != NULL) {
            // invalid index but pop valid val
            fail_invalid_index++;
        }
        test_arr_integrity(&arr, &fail_cnt, tag++, capacity - cnt, false, capacity, free);
    }
    check_res(fail_invalid_index == 0, "pop at invalid index should return NULL", &fail_cnt, tag++);
    check_res(fail_valid_index == 0, "pop at valid shouldn't return NULL", &fail_cnt, tag++);


    // verify
    int fail = 0;
    int s = hm_arr_size(&arr);
    for (int i = 0; i < cnt; i++) {
        // verify valid every pointer get by pop
        for (int j = 0; j < s; j++) {
            int* v = hm_arr_get(&arr, j);
            if (v == pop_v[i]) {
                fail++;
            }
        }
    }
    check_res(fail == 0, "the pop val should be not existed in arr", &fail_cnt, tag++);

    hm_arr_free(&arr);
    for (int i = 0; i < cnt; i++) {
        free(pop_v[i]);
    }

    print_end("ARR(FIXED) | FUNC | POP | CAPACITY: 64", fail_cnt);
    HM_TEST_COUNTER
}

void test_arr_dynamic_pop() {
    int fail_cnt = 0;
    int tag = 0;
    print_run("ARR(DYNAMIC) | FUNC | POP | CAPACITY: 64");

    int start_capacity = 64;
    hm_arr arr;
    hm_arr_init_dynamic_grow(&arr, start_capacity, free);

    // insert
    for (int i = 0; i < start_capacity * 2; i++) {
        int* v = (int*)malloc(sizeof(int));
        *v = i;
        hm_arr_insert_tail(&arr, v);
    }
    
    // pop
    size_t pop_indexs[] = {2, 1, 65, 99, 0, 45, 2, 4 * start_capacity, 4, 10 * start_capacity, 4, 23, 200, 420, 520, 9, 1314};
    int num = sizeof(pop_indexs) / sizeof(size_t);
    int* pop_v[num];

    int cnt = 0;
    int fail_invalid_index = 0;
    int fail_valid_index = 0;
    for (int i = 0; i < num; i++) {
        size_t s = hm_arr_size(&arr);
        int* v = hm_arr_pop(&arr, pop_indexs[i]);
        if (pop_indexs[i] < s) {
            if (v == NULL) {
                // valid index but pop invalid val
                fail_valid_index++;
            } else {
                pop_v[cnt++] = v;
            }
        } else if (v != NULL) {
            // invalid index but pop valid val
            fail_invalid_index++;
        }
        test_arr_integrity(&arr, &fail_cnt, tag++, start_capacity * 2 - cnt, true, start_capacity, free);
    }
    check_res(fail_invalid_index == 0, "pop at invalid index should return NULL", &fail_cnt, tag++);
    check_res(fail_valid_index == 0, "pop at valid shouldn't return NULL", &fail_cnt, tag++);


    // verify
    int fail = 0;
    int s = hm_arr_size(&arr);
    for (int i = 0; i < cnt; i++) {
        // verify valid every pointer get by pop
        for (int j = 0; j < s; j++) {
            int* v = hm_arr_get(&arr, j);
            if (v == pop_v[i]) {
                fail++;
            }
        }
    }
    check_res(fail == 0, "the pop val should be not existed in arr", &fail_cnt, tag++);

    hm_arr_free(&arr);
    for (int i = 0; i < cnt; i++) {
        free(pop_v[i]);
    }

    print_end("ARR(DYNAMIC) | FUNC | POP | CAPACITY: 64", fail_cnt);
    HM_TEST_COUNTER
}


void test_arr_fixed_func() {
    test_arr_fixed_init();                                                              printf("\n");

    test_arr_fixed_insert_head();                                                       printf("\n");
    test_arr_fixed_insert_tail();                                                       printf("\n");
    test_arr_fixed_insert_index();                                                      printf("\n");

    test_arr_fixed_get();                                                               printf("\n");
    test_arr_fixed_get_pointer();                                                       printf("\n");

    test_arr_fixed_change();                                                            printf("\n");

    test_arr_fixed_pop();                                                               printf("\n");

}


void test_arr_dynamic_func() {
    test_arr_dynamic_init();                                                            printf("\n");

    test_arr_dynamic_insert_head();                                                     printf("\n");
    test_arr_dynamic_insert_tail();                                                     printf("\n");
    test_arr_dynamic_insert_index();                                                    printf("\n");

    test_arr_dynamic_get();                                                             printf("\n");
    test_arr_dynamic_get_pointer();                                                     printf("\n");

    test_arr_dynamic_change();                                                          printf("\n");

    test_arr_dynamic_pop();                                                             printf("\n");

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

