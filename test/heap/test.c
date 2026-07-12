#include "../../include/hm_heap.h"
#include "../hm_test_tool.h"
#include <stdint.h>
#include <stdlib.h>
#include "../../function/cmp/hm_cmp.h"

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




void test_heap_fixed_func() {
    test_heap_fixed_init();                                                                     printf("\n");    
}

void test_heap_dynamic_func() {
    test_heap_dynamic_init();                                                                   printf("\n");
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

