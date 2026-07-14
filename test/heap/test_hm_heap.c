#include "../../include/hm_heap.h"
#include "../hm_test_tool.h"
#include <stdint.h>
#include <stdlib.h>
#include "../../function/cmp/hm_cmp.h"
#include <string.h>

// This variable can record the total number of failures and it can be used as a return value to check whether the test passed
int all_failure_num = 0;

// use a macro to replace the repetitive code 
#define HM_TEST_COUNTER \
    all_failure_num += fail_cnt;


// every test function ...

void test_heap_integrity(hm_heap* heap, int* fail_cnt, size_t size, bool dynamic_grow, size_t capacity, hm_free free, hm_cmp cmp) {

    check_res(heap->size == size, "TEST OF INTEGRITY: the heap's size(heap.size) is wrong", fail_cnt);
    check_res(heap->dynamic_grow == dynamic_grow, "TEST OF INTEGRITY: heap's dynamic signal is unexpected", fail_cnt);
    if (!dynamic_grow) {
        check_res(heap->capacity == capacity, "TEST OF INTEGRITY: the heap's capacity is wrrong", fail_cnt);
    }
    check_res(heap->size <= heap->capacity, "TEST OF INTEGRITY: `capacity` should greater than `size`", fail_cnt);

    check_res(!(heap->capacity == 0 && heap->vals != NULL), "TEST OF INTEGRITY: heap's capacity is 0, but vals have memory", fail_cnt);
    check_res(!(heap->capacity != 0 && heap->vals == NULL), "TEST OF INTEGRITY: heap's capacity isn't 0, but vals is NULL", fail_cnt);

    check_res(heap->free == free, "TEST OF INTEGRITY: heap's free function is wrong", fail_cnt);
    check_res(heap->cmp == cmp, "TEST OF INTEGRITY: heap's cmp function is wrong", fail_cnt);

    // verify logic of heap

    int fail = 0;
    for (size_t parent = ((heap->size - 1) - 1) / 2; 2 * parent + 1 < heap->size; parent--) {
        size_t l = 2 * parent + 1;
        size_t r = l + 1;
        size_t min;
        if (r < heap->size) {
            min = heap->cmp(heap->vals[l], heap->vals[r]) < 0 ? l : r;
        } else {
            min = l;
        }
        if (heap->cmp(heap->vals[parent], heap->vals[min]) > 0) {
            fail++;
        }
    }
    check_res(fail == 0, "TEST OF INTEGRITY: the structure logic of heap is wrong", fail_cnt);


}

void test_heap_fixed_init() {
    int fail_cnt = 0;
    print_run("HEAP(FIXED) | FUNC | INIT | CAPAITY: 64");
    hm_heap heap;
    int capacity = 64;
    // pass in `free` for heap
    hm_heap_init(&heap, capacity, free, cmp_int_up);
    
    test_heap_integrity(&heap, &fail_cnt, 0, false, capacity, free, cmp_int_up);
    check_res(heap.capacity == capacity, "the heap's capaity isn't equal to expected capacity", &fail_cnt);
    check_res(heap.cmp == cmp_int_up, "the heap's cmp function should be `cmp_int_up`", &fail_cnt);
    check_res(heap.dynamic_grow == false, "the queue dynamic signal should be false", &fail_cnt);
    check_res(heap.free == free, "the heap's free function should be free", &fail_cnt);
    check_res(heap.size == 0, "the heap's size should be 0", &fail_cnt);

    hm_heap_free(&heap);
    
    // pass in NULL for queue
    hm_heap_init(&heap, capacity, NULL, cmp_int_up);
    test_heap_integrity(&heap, &fail_cnt, 0, false, capacity, NULL, cmp_int_up);
    check_res(heap.free == NULL, "the heap's free function should be NULL", &fail_cnt);

    hm_heap_free(&heap);
    print_end("HEAP(FIXED) | FUNC | INIT | CAPAITY: 64", fail_cnt);
    HM_TEST_COUNTER
}

void test_heap_dynamic_init() {
    int fail_cnt = 0;
    print_run("HEAP(DYNAMIC) | FUNC | INIT | CAPAITY: 64");
    hm_heap heap;
    int capacity = 64;
    // pass in `free` for heap
    hm_heap_init_dynamic_grow(&heap, capacity, free, cmp_int_up);
    
    test_heap_integrity(&heap, &fail_cnt, 0, true, capacity, free, cmp_int_up);
    check_res(heap.capacity == capacity, "the heap's capaity isn't equal to expected capacity", &fail_cnt);
    check_res(heap.cmp == cmp_int_up, "the heap's cmp function should be `cmp_int_up`", &fail_cnt);
    check_res(heap.dynamic_grow == true, "the queue dynamic signal should be false", &fail_cnt);
    check_res(heap.free == free, "the heap's free function should be free", &fail_cnt);
    check_res(heap.size == 0, "the heap's size should be 0", &fail_cnt);
    
    hm_heap_free(&heap);
    
    // pass in NULL for queue
    hm_heap_init_dynamic_grow(&heap, capacity, NULL, cmp_int_up);
    test_heap_integrity(&heap, &fail_cnt, 0, true, capacity, NULL, cmp_int_up);
    check_res(heap.free == NULL, "the heap's free function should be NULL", &fail_cnt);
    
    hm_heap_free(&heap);
    
    print_end("HEAP(DYNAMIC) | FUNC | INIT | CAPAITY: 64", fail_cnt);
    
}


void test_heap_fixed_insert() {
    int fail_cnt = 0;
    print_run("HEAP(FIXED) | FUNC | INSERT | CAPAITY: 64");
    
    
    hm_heap heap;
    int capacity = 64;
    
    int mod = 1000;     // mod shouldn't too big
    int arr_real[mod];
    int arr_tmp[mod];

    int seeds[] = {100, 200, 520};
    int cnt = sizeof(seeds) / sizeof(int);
    for (int i = 0; i < cnt; i++) {
        hm_heap_init(&heap, capacity, free, cmp_int_up);

        srand(seeds[i]);
        memset(arr_real, 0, sizeof(arr_real));
        memset(arr_tmp, 0, sizeof(arr_tmp));
        
        // insert | reasonable number
        int fail = 0;
        for (int j = 0; j < capacity; j++) {
            int val = rand() % mod;
            arr_real[val]++;
            int* v = (int*)malloc(sizeof(int));
            *v = val;
            if (hm_heap_insert(&heap, v) != hm_heap_ret_suc) {
                fail++;
            }
        }
        // verify
        check_res(fail == 0, "the insert function should return full when insert with reasonable number", &fail_cnt);
        test_heap_integrity(&heap, &fail_cnt, capacity, false, capacity, free, cmp_int_up);
        void** vals = heap.vals;
        for (int j = 0; j < capacity; j++) {
            arr_tmp[*(int*)vals[j]]++;
        }
        int fail_diff = 0;
        for (int j = 0; j < mod; j++) {
            if (arr_tmp[j] != arr_real[j]) {
                fail_diff++;
            }
        }
        check_res(fail_diff == 0, "some vals doesn't exist in heap's vals array", &fail_cnt);


        // insert | beyond the capacity
        fail = 0;
        for (int i = 0; i < capacity; i++) {
            int val = rand() % mod;
            int* v = (int*)malloc(sizeof(int));
            *v = val;
            if (hm_heap_insert(&heap, v) != hm_heap_ret_full) {
                fail++;
            } else {
                free(v);
            }
        }
        check_res(fail == 0, "the insert function should return full when insert with beyong the capacity's number", &fail_cnt);
        
        hm_heap_free(&heap);
    }
    
    
    print_end("HEAP(FIXED) | FUNC | INSERT | CAPAITY: 64", fail_cnt);
    HM_TEST_COUNTER
    
}

void test_heap_dynamic_insert() {
    int fail_cnt = 0;
    print_run("HEAP(DYNAMIC) | FUNC | INSERT | CAPAITY: 64");
    hm_heap heap;
    int start_capacity = 64;
    // mod shouldn't too big
    int mod = 1000;
    int arr_real[mod];
    int arr_tmp[mod];
    
    int seeds[] = {100, 200, 520};
    int cnt = sizeof(seeds) / sizeof(int);
    for (int i = 0; i < cnt; i++) {
        hm_heap_init_dynamic_grow(&heap, start_capacity, free, cmp_int_up);
    
        srand(seeds[i]);
        memset(arr_real, 0, sizeof(arr_real));
        memset(arr_tmp, 0, sizeof(arr_tmp));
        
        // insert
        int fail = 0;
        for (int j = 0; j < start_capacity * 2; j++) {
            int val = rand() % mod;
            arr_real[val]++;
            int* v = (int*)malloc(sizeof(int));
            *v = val;
            if (hm_heap_insert(&heap, v) != hm_heap_ret_suc) {
                fail++;
            }
        }
        // verify
        check_res(fail == 0, "the insert function should return full when insert with reasonable number", &fail_cnt);
        test_heap_integrity(&heap, &fail_cnt, start_capacity * 2, true, start_capacity, free, cmp_int_up);
        void** vals = heap.vals;
        for (int j = 0; j < start_capacity * 2; j++) {
            arr_tmp[*(int*)vals[j]]++;
        }
        int fail_diff = 0;
        for (int j = 0; j < mod; j++) {
            if (arr_tmp[j] != arr_real[j]) {
                fail_diff++;
            }
        }
        check_res(fail_diff == 0, "some vals doesn't exist in heap's vals array", &fail_cnt);
        hm_heap_free(&heap);
    }
    
    print_end("HEAP(DYNAMIC) | FUNC | INSERT | CAPAITY: 64", fail_cnt);
    
}

void test_heap_fixed_extract() {
    int fail_cnt = 0;
    print_run("HEAP(FIXED) | FUNC | EXTRACT | CAPAITY: 64");
    
    hm_heap heap;
    int capacity = 64;
    int arr[capacity];
    
    int seeds[] = {100, 200, 520, 23333333};
    int cnt = sizeof(seeds) / sizeof(int);
    for (int i = 0; i < cnt; i++) {
        hm_heap_init(&heap, capacity, free, cmp_int_up);
    
        srand(seeds[i]);
        // insert
        int fail = 0;
        for (int j = 0; j < capacity; j++) {
            int val = rand();
            arr[j] = val;
            int* v = (int*)malloc(sizeof(int));
            *v = val;
            hm_heap_insert(&heap, v);
        }
        qsort(arr, capacity, sizeof(int), cmp_int_up);
        int fail_null = 0;
        int fail_diff = 0;
        for (int j = 0; j < capacity; j++) {
            int* v = hm_heap_extract(&heap);
            if (v == NULL) {
                fail_null++;
            } else if (*v != arr[j]) {
                fail_diff++;
            }
            free(v);
        }
        test_heap_integrity(&heap, &fail_cnt, 0, false, capacity, free, cmp_int_up);
        check_res(fail_diff == 0, "the extract val is wrong", &fail_cnt);
        check_res(fail_null == 0, "the extract val shouldn't be NULL", &fail_cnt);

        hm_heap_free(&heap);
    }
    
    print_end("HEAP(FIXED) | FUNC | EXTRACT | CAPAITY: 64", fail_cnt);
    HM_TEST_COUNTER
    
}

void test_heap_dynamic_extract() {
    int fail_cnt = 0;
    print_run("HEAP(DYNAMIC) | FUNC | EXTRACT | CAPAITY: 64");

    hm_heap heap;
    int start_capacity = 64;
    int arr[start_capacity * 2];
    
    int seeds[] = {100, 200, 520, 1000000, 5201314};
    int cnt = sizeof(seeds) / sizeof(int);
    for (int i = 0; i < cnt; i++) {
        hm_heap_init_dynamic_grow(&heap, start_capacity, free, cmp_int_up);
    
        srand(seeds[i]);
        // insert
        int fail = 0;
        for (int j = 0; j < start_capacity * 2; j++) {
            int val = rand();
            arr[j] = val;
            int* v = (int*)malloc(sizeof(int));
            *v = val;
            hm_heap_insert(&heap, v);
        }
        qsort(arr, start_capacity * 2, sizeof(int), cmp_int_up);
        int fail_null = 0;
        int fail_diff = 0;
        for (int j = 0; j < start_capacity * 2; j++) {
            int* v = hm_heap_extract(&heap);
            if (v == NULL) {
                fail_null++;
            } else if (*v != arr[j]) {
                fail_diff++;
            }
            free(v);
        }
        test_heap_integrity(&heap, &fail_cnt, 0, true, start_capacity, free, cmp_int_up);
        check_res(fail_diff == 0, "the extract val is wrong", &fail_cnt);
        check_res(fail_null == 0, "the extract val shouldn't be NULL", &fail_cnt);
    
        hm_heap_free(&heap);
    }
    
    print_end("HEAP(DYNAMIC) | FUNC | EXTRACT | CAPAITY: 64", fail_cnt);
    HM_TEST_COUNTER
}


void test_heap_fixed_peek() {
    int fail_cnt = 0;
    print_run("HEAP(FIXED) | FUNC | PEEK | CAPAITY: 64");

    hm_heap heap;
    int capacity = 64;
    int arr[capacity];
    
    int seeds[] = {100, 200, 520, 23333333, 7777};
    int cnt = sizeof(seeds) / sizeof(int);
    for (int i = 0; i < cnt; i++) {
        hm_heap_init(&heap, capacity, free, cmp_int_up);
    
        srand(seeds[i]);
        // insert
        int fail = 0;
        for (int j = 0; j < capacity; j++) {
            int val = rand();
            arr[j] = val;
            int* v = (int*)malloc(sizeof(int));
            *v = val;
            hm_heap_insert(&heap, v);
        }

        qsort(arr, capacity, sizeof(int), cmp_int_up);
        int* v = hm_heap_peek(&heap);

        check_res(*v == arr[0], "the peek val is wrong", &fail_cnt);
        test_heap_integrity(&heap, &fail_cnt, capacity, false, capacity, free, cmp_int_up);

        hm_heap_free(&heap);
    }

    print_end("HEAP(FIXED) | FUNC | PEEK | CAPAITY: 64", fail_cnt);
    HM_TEST_COUNTER
    
}

void test_heap_dynamic_peek() {
    int fail_cnt = 0;
    print_run("HEAP(DYNAMIC) | FUNC | PEEK | CAPAITY: 64");
    
    hm_heap heap;
    int start_capacity = 64;
    int arr[start_capacity * 2];
    
    int seeds[] = {100, 200, 520, 23333333, 9420};
    int cnt = sizeof(seeds) / sizeof(int);
    for (int i = 0; i < cnt; i++) {
        hm_heap_init_dynamic_grow(&heap, start_capacity, free, cmp_int_up);
    
        srand(seeds[i]);
        // insert
        int fail = 0;
        for (int j = 0; j < start_capacity * 2; j++) {
            int val = rand();
            arr[j] = val;
            int* v = (int*)malloc(sizeof(int));
            *v = val;
            hm_heap_insert(&heap, v);
        }
    
        qsort(arr, start_capacity * 2, sizeof(int), cmp_int_up);
        int* v = hm_heap_peek(&heap);
    
        check_res(*v == arr[0], "the peek val is wrong", &fail_cnt);
        test_heap_integrity(&heap, &fail_cnt, start_capacity * 2, true, start_capacity, free, cmp_int_up);
    
        hm_heap_free(&heap);
    }
    
    print_end("HEAP(DYNAMIC) | FUNC | PEEK | CAPAITY: 64", fail_cnt);
    HM_TEST_COUNTER
    
}


void test_heap_fixed_clear() {
    int fail_cnt = 0;
    print_run("HEAP(FIXED) | FUNC | CLEAR | CAPAITY: 64");
    
    hm_heap heap;
    int capacity = 64;
    
    
    hm_heap_init(&heap, capacity, free, cmp_int_up);
    // insert
    for (int i = 0; i < capacity; i++) {
        int val = rand();
        int* v = (int*)malloc(sizeof(int));
        *v = val;
        hm_heap_insert(&heap, v);
    }

    hm_heap_clear(&heap);
    test_heap_integrity(&heap, &fail_cnt, 0, false, capacity, free, cmp_int_up);

    hm_heap_free(&heap);
    // use valgrind to check leak memory
        
    print_end("HEAP(FIXED) | FUNC | CLEAR | CAPAITY: 64", fail_cnt);
    HM_TEST_COUNTER
    
}

void test_heap_dynamic_clear() {
    int fail_cnt = 0;
    print_run("HEAP(DYNAMIC) | FUNC | CLEAR | CAPAITY: 64");
    
    hm_heap heap;
    int start_capacity = 64;
    
    
    hm_heap_init_dynamic_grow(&heap, start_capacity, free, cmp_int_up);
    // insert
    for (int i = 0; i < start_capacity * 2; i++) {
        int val = rand();
        int* v = (int*)malloc(sizeof(int));
        *v = val;
        hm_heap_insert(&heap, v);
    }
    
    hm_heap_clear(&heap);
    test_heap_integrity(&heap, &fail_cnt, 0, true, start_capacity, free, cmp_int_up);
    
    hm_heap_free(&heap);
    // use valgrind to check leak memory
    
    print_end("HEAP(DYNAMIC) | FUNC | CLEAR | CAPAITY: 64", fail_cnt);
    HM_TEST_COUNTER
    
}

void test_heap_fixed_free() {
    int fail_cnt = 0;
    print_run("HEAP(FIXED) | FUNC | FREE | CAPAITY: 64");
    
    hm_heap heap;
    int capacity = 64;
    
    
    hm_heap_init(&heap, capacity, free, cmp_int_up);
    // insert
    for (int i = 0; i < capacity; i++) {
        int val = rand();
        int* v = (int*)malloc(sizeof(int));
        *v = val;
        hm_heap_insert(&heap, v);
    }

    hm_heap_free(&heap);
    test_heap_integrity(&heap, &fail_cnt, 0, false, 0, free, cmp_int_up);

    // use valgrind to check leak memory
        
    print_end("HEAP(FIXED) | FUNC | FREE | CAPAITY: 64", fail_cnt);
    HM_TEST_COUNTER
}


void test_heap_dynamic_free() {
    int fail_cnt = 0;
    print_run("HEAP(DYNAMIC) | FUNC | FREE | CAPAITY: 64");
    
    hm_heap heap;
    int start_capacity = 64;
    
    
    hm_heap_init_dynamic_grow(&heap, start_capacity, free, cmp_int_up);
    // insert
    for (int i = 0; i < start_capacity * 2; i++) {
        int val = rand();
        int* v = (int*)malloc(sizeof(int));
        *v = val;
        hm_heap_insert(&heap, v);
    }
    
    hm_heap_free(&heap);

    test_heap_integrity(&heap, &fail_cnt, 0, true, 0, free, cmp_int_up);
    // use valgrind to check leak memory
    
    print_end("HEAP(DYNAMIC) | FUNC | FREE | CAPAITY: 64", fail_cnt);
    HM_TEST_COUNTER
}




void test_heap_fixed_build() {
    int fail_cnt = 0;
    print_run("HEAP(FIXED) | FUNC | BUILD | CAPACITY: 64");

    int capacity = 64;
    int mod = 1000;
    int seeds[] = {20, 1000890, 10086};
    int cnt = sizeof(seeds) / sizeof(int);
    
    for (int i = 0; i < cnt; i++) {
        srand(seeds[i]);
        void** vals = (void**)malloc(sizeof(void*) * capacity);
        int vals_real[capacity];
        
        int arr_real[mod];
        int arr_now[mod];
        
        memset(arr_now, 0, sizeof(arr_now));
        memset(arr_real, 0, sizeof(arr_real));

        for (int j = 0; j < capacity; j++) {
            int val = rand() % mod;
            arr_real[val]++;
            int* v = (int*)malloc(sizeof(int));
            *v = val;
            vals[j] = v;

            vals_real[j] = val;
        }

        hm_heap heap;

        hm_heap_build(&heap, vals, capacity, capacity, free, cmp_int_up);

        // verify
        test_heap_integrity(&heap, &fail_cnt, capacity, false, capacity, free, cmp_int_up);
        
        vals = heap.vals;

        for (int j = 0; j < capacity; j++) {
            arr_now[*(int*)vals[j]]++;
        }
        int fail_diff = 0;
        for (int j = 0; j < mod; j++) {
            if (arr_now[j] != arr_real[j]) {
                fail_diff++;
            }
        }
        check_res(fail_diff == 0, "some vals doesn't exist in the built heap's vals array", &fail_cnt);
        
        int fail_p_null = 0, fail_p_diff = 0;
        int fail_e_null = 0, fail_e_diff = 0;
        qsort(vals_real, capacity, sizeof(int), cmp_int_up);
        for (int j = 0; j < capacity; j++) {
            int* peek_v = hm_heap_peek(&heap);
            if (peek_v == NULL) {
                fail_p_null++;
            } else if (*peek_v != vals_real[j]) {
                fail_p_diff++;
            }
            
            int* extract_v = hm_heap_extract(&heap);

            if (extract_v == NULL) {
                fail_e_null++;
            } else if (*extract_v != vals_real[j]) {
                fail_e_diff++;
            }
            free(extract_v);
        }
        check_res(fail_p_null == 0, "the peek val shouldn't be NULL", &fail_cnt);
        check_res(fail_p_diff == 0, "the peek val is wrong", &fail_cnt);

        check_res(fail_e_null == 0, "the extract val shouldn't be NULL", &fail_cnt);
        check_res(fail_e_diff == 0, "the extract val is wrong", &fail_cnt);

        hm_heap_free(&heap);
    }

    print_end("HEAP(FIXED) | FUNC | BUILD | CAPACITY: 64", fail_cnt);
    HM_TEST_COUNTER
}

void test_heap_dynamic_build() {
    int fail_cnt = 0;
    print_run("HEAP(DYNAMIC) | FUNC | BUILD | CAPACITY: 64");
    
    int capacity = 64;
    int mod = 1000;
    int seeds[] = {20, 1000890, 10086, 1001};
    int cnt = sizeof(seeds) / sizeof(int);
    
    for (int i = 0; i < cnt; i++) {
        srand(seeds[i]);
        void** vals = (void**)malloc(sizeof(void*) * capacity);
        int vals_real[capacity];
        
        int arr_real[mod];
        int arr_now[mod];
        
        memset(arr_now, 0, sizeof(arr_now));
        memset(arr_real, 0, sizeof(arr_real));
    
        for (int j = 0; j < capacity; j++) {
            int val = rand() % mod;
            arr_real[val]++;
            int* v = (int*)malloc(sizeof(int));
            *v = val;
            vals[j] = v;
    
            vals_real[j] = val;
        }
    
        hm_heap heap;
    
        hm_heap_build_dynamic_grow(&heap, vals, capacity, capacity, free, cmp_int_up);
    
        // verify
        test_heap_integrity(&heap, &fail_cnt, capacity, true, capacity, free, cmp_int_up);
        
        vals = heap.vals;
    
        for (int j = 0; j < capacity; j++) {
            arr_now[*(int*)vals[j]]++;
        }
        int fail_diff = 0;
        for (int j = 0; j < mod; j++) {
            if (arr_now[j] != arr_real[j]) {
                fail_diff++;
            }
        }
        check_res(fail_diff == 0, "some vals doesn't exist in the built heap's vals array", &fail_cnt);
        
        int fail_p_null = 0, fail_p_diff = 0;
        int fail_e_null = 0, fail_e_diff = 0;
        qsort(vals_real, capacity, sizeof(int), cmp_int_up);
        for (int j = 0; j < capacity; j++) {
            int* peek_v = hm_heap_peek(&heap);
            if (peek_v == NULL) {
                fail_p_null++;
            } else if (*peek_v != vals_real[j]) {
                fail_p_diff++;
            }
            
            int* extract_v = hm_heap_extract(&heap);
    
            if (extract_v == NULL) {
                fail_e_null++;
            } else if (*extract_v != vals_real[j]) {
                fail_e_diff++;
            }
            free(extract_v);
        }
        check_res(fail_p_null == 0, "the peek val shouldn't be NULL", &fail_cnt);
        check_res(fail_p_diff == 0, "the peek val is wrong", &fail_cnt);
    
        check_res(fail_e_null == 0, "the extract val shouldn't be NULL", &fail_cnt);
        check_res(fail_e_diff == 0, "the extract val is wrong", &fail_cnt);
    
        hm_heap_free(&heap);
    }
    
    print_end("HEAP(DYNAMIC) | FUNC | BUILD | CAPACITY: 64", fail_cnt);
    HM_TEST_COUNTER
    
}


void test_heap_fixed_rebuild() {
    int fail_cnt = 0;
    print_run("HEAP(FIXED) | FUNC | REBUILD | CAPACITY: 64");

    int capacity = 64;
    int seeds[] = {20, 1000890, 10086, 1001};
    int cnt = sizeof(seeds) / sizeof(int);
    int arr[capacity];
    for (int i = 0; i < cnt; i++) {
        srand(seeds[i]);
        hm_heap heap;
        // init | down
        hm_heap_init(&heap, capacity, free, cmp_int_down);
        // insert
        for (int j = 0; j < capacity; j++) {
            int val = rand();
            int* v = (int*)malloc(sizeof(int));
            *v = val;
            arr[j] = val;
            hm_heap_insert(&heap, v);
        }
        // rebuild | up
        hm_heap_rebuild(&heap, cmp_int_up);
        qsort(arr, capacity, sizeof(int), cmp_int_up);
        test_heap_integrity(&heap, &fail_cnt, capacity, false, capacity, free, cmp_int_up);

        int fail_p_null = 0, fail_p_diff = 0;
        int fail_e_null = 0, fail_e_diff = 0;
        for (int j = 0; j < capacity; j++) {
            int* peek_v = hm_heap_peek(&heap);
            if (peek_v == NULL) {
                fail_p_null++;
            } else if (*peek_v != arr[j]) {
                fail_p_diff++;
            }
            
            int* extract_v = hm_heap_extract(&heap);
    
            if (extract_v == NULL) {
                fail_e_null++;
            } else if (*extract_v != arr[j]) {
                fail_e_diff++;
            }
            free(extract_v);
        }

        check_res(fail_p_null == 0, "the peek val shouldn't be NULL", &fail_cnt);
        check_res(fail_p_diff == 0, "the peek val is wrong", &fail_cnt);
    
        check_res(fail_e_null == 0, "the extract val shouldn't be NULL", &fail_cnt);
        check_res(fail_e_diff == 0, "the extract val is wrong", &fail_cnt);
    
        hm_heap_free(&heap);
    }
    

    print_end("HEAP(FIXED) | FUNC | REBUILD | CAPACITY: 64", fail_cnt);
    HM_TEST_COUNTER
}

void test_heap_dynamic_rebuild() {
    int fail_cnt = 0;
    print_run("HEAP(DYNAMIC) | FUNC | REBUILD | CAPACITY: 64");
    
    
    int capacity = 64;
    int seeds[] = {20, 1000890, 10086, 1001};
    int cnt = sizeof(seeds) / sizeof(int);
    int arr[capacity];
    for (int i = 0; i < cnt; i++) {
        srand(seeds[i]);
        hm_heap heap;
        // init | down
        hm_heap_init_dynamic_grow(&heap, capacity, free, cmp_int_down);
        // insert
        for (int j = 0; j < capacity; j++) {
            int val = rand();
            int* v = (int*)malloc(sizeof(int));
            *v = val;
            arr[j] = val;
            hm_heap_insert(&heap, v);
        }
        // rebuild | up
        hm_heap_rebuild(&heap, cmp_int_up);
        qsort(arr, capacity, sizeof(int), cmp_int_up);
        test_heap_integrity(&heap, &fail_cnt, capacity, true, capacity, free, cmp_int_up);

        int fail_p_null = 0, fail_p_diff = 0;
        int fail_e_null = 0, fail_e_diff = 0;
        for (int j = 0; j < capacity; j++) {
            int* peek_v = hm_heap_peek(&heap);
            if (peek_v == NULL) {
                fail_p_null++;
            } else if (*peek_v != arr[j]) {
                fail_p_diff++;
            }
            
            int* extract_v = hm_heap_extract(&heap);
    
            if (extract_v == NULL) {
                fail_e_null++;
            } else if (*extract_v != arr[j]) {
                fail_e_diff++;
            }
            free(extract_v);
        }
        
        check_res(fail_p_null == 0, "the peek val shouldn't be NULL", &fail_cnt);
        check_res(fail_p_diff == 0, "the peek val is wrong", &fail_cnt);
    
        check_res(fail_e_null == 0, "the extract val shouldn't be NULL", &fail_cnt);
        check_res(fail_e_diff == 0, "the extract val is wrong", &fail_cnt);
    
        hm_heap_free(&heap);
    }

    print_end("HEAP(DYNAMIC) | FUNC | REBUILD | CAPACITY: 64", fail_cnt);
    HM_TEST_COUNTER
    
}

void test_heap_fixed_func() {
    test_heap_fixed_init();                                                                     printf("\n");    

    test_heap_fixed_insert();                                                                   printf("\n");

    test_heap_fixed_peek();                                                                     printf("\n");

    test_heap_fixed_extract();                                                                  printf("\n");

    test_heap_fixed_clear();                                                                    printf("\n");

    test_heap_fixed_free();                                                                     printf("\n");

    test_heap_fixed_build();                                                                    printf("\n");

    test_heap_fixed_rebuild();                                                                  printf("\n");
}

void test_heap_dynamic_func() {
    test_heap_dynamic_init();                                                                   printf("\n");

    test_heap_dynamic_insert();                                                                 printf("\n");

    test_heap_dynamic_peek();                                                                   printf("\n");
 
    test_heap_dynamic_extract();                                                                printf("\n");

    test_heap_dynamic_clear();                                                                  printf("\n");

    test_heap_dynamic_clear();                                                                  printf("\n");

    test_heap_dynamic_build();                                                                  printf("\n");

    test_heap_dynamic_rebuild();                                                                printf("\n");
}



void function_test() {
    test_heap_fixed_func();

    test_heap_dynamic_func();
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

