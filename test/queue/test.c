/*
 * Copyright (c) 2026 Flmpx
 * Licensed under MIT (see LICENSE).
 */


#include "../../function/cmp/hm_cmp.h"
#include "../hm_test_tool.h"
#include <stdlib.h>
#include "../../include/hm_queue.h"

// This variable can record the total number of failures and it can be used as a return value to check whether the test passed
int all_failure_num = 0;

// use a macro to replace the repetitive code 
#define HM_TEST_COUNTER \
    all_failure_num += fail_cnt;


// every test function ...

void test_queue_integrity(hm_queue* queue, int* fail_cnt, size_t size, bool dynamic_grow, size_t capacity, hm_free free) {
    // because the limitation of queue, the integrity test only test these

    check_res(queue->size == size, "TEST OF INTEGRITY: the queue's size(queue.size) is wrong", fail_cnt);
    size_t real_size;
    if (queue->front <= queue->rear && queue->capacity != queue->size) {
        real_size = queue->rear - queue->front;
    } else {
        real_size = queue->rear + queue->capacity - queue->front;
    }
    check_res(real_size == size, "TEST OF INTEGRITY: the queue.size(get by front and rear) is wrong", fail_cnt);
    check_res(real_size <= queue->capacity, "TEST OF INTEGRITY: `capacity` should greater than `size`", fail_cnt);

    check_res(queue->dynamic_grow == dynamic_grow, "TEST OF INTEGRITY: queue's dynamic signal is unexpected", fail_cnt);
    if (!dynamic_grow) {
        check_res(queue->capacity == capacity, "TEST OF INTEGRITY: queue's capacity is wrong", fail_cnt);
    }
    if (queue->capacity) {
        check_res(queue->front < queue->capacity, "TEST OF INTEGRITY: `capacity` should greater than `front`", fail_cnt);
        check_res(queue->rear < queue->capacity, "TEST OF INTEGRITY: `capacity` should greater than `rear`", fail_cnt);
    } else {
        check_res(queue->front == 0, "TEST OF INTEGRITY: `front` should be 0 when `capacity` is 0", fail_cnt);
        check_res(queue->rear == 0, "TEST OF INTEGRITY: `rear` should be 0 when `capacity` is 0", fail_cnt);
    }

    check_res(!(queue->capacity == 0 && queue->vals != NULL), "TEST OF INTEGRITY: queue's capacity is 0, but vals have memory", fail_cnt);
    check_res(!(queue->capacity != 0 && queue->vals == NULL), "TEST OF INTEGRITY: queue's capacity isn't 0, but vals is NULL", fail_cnt);

    check_res(queue->free == free, "TEST OF INTEGRITY: queue's free function is wrong", fail_cnt);

}



void test_queue_fixed_init() {
    int fail_cnt = 0;
    print_run("QUEUE(FIXED) | FUNC | INIT | CAPACITY: 64");
    hm_queue queue;
    int capacity = 64;    
    // pass in `free` for queue
    hm_queue_init(&queue, capacity, free);
    test_queue_integrity(&queue, &fail_cnt, 0, false, capacity, free);
    
    check_res(queue.capacity == capacity, "the queue's capacity isn't equal to expected capacity", &fail_cnt);
    check_res(queue.dynamic_grow == false, "the queue's dynamic-gorwth should is `false`", &fail_cnt);
    check_res(queue.free == free, "the queue's free should be `free` when pass in `free` to queue", &fail_cnt);
    check_res(queue.front == 0, "the queue's front should be 0", &fail_cnt);
    check_res(queue.rear == 0, "the queue's rear should be 0", &fail_cnt);
    
    hm_queue_free(&queue);
    
    // pass in NULL for queue
    hm_queue_init(&queue, capacity, NULL);
    check_res(queue.free == NULL, "the queue's free should be `NULL` when pass in `NULL` to queue", &fail_cnt);
    test_queue_integrity(&queue, &fail_cnt, 0, false, capacity, NULL);

    hm_queue_free(&queue);
    print_end("QUEUE(FIXED) | FUNC | INIT | CAPACITY: 64", fail_cnt);
    HM_TEST_COUNTER
}

void test_queue_dynamic_init() {
    int fail_cnt = 0;
    print_run("QUEUE(DYNMAIC) | FUNC | INIT | CAPACITY: 64");
    hm_queue queue;
    int start_capacity = 64;    
    // pass in `free` for queue
    hm_queue_init_dynamic_grow(&queue, start_capacity, free);
    test_queue_integrity(&queue, &fail_cnt, 0, true, start_capacity, free);
    
    check_res(queue.capacity == start_capacity, "the queue's capacity isn't equal to expected capacity", &fail_cnt);
    check_res(queue.dynamic_grow == true, "the queue's dynamic-gorwth should is `false`", &fail_cnt);
    check_res(queue.free == free, "the queue's free should be `free` when pass in `free` to queue", &fail_cnt);
    check_res(queue.front == 0, "the queue's front should be 0", &fail_cnt);
    check_res(queue.rear == 0, "the queue's rear should be 0", &fail_cnt);
    
    hm_queue_free(&queue);
    
    // pass in NULL for queue
    hm_queue_init_dynamic_grow(&queue, start_capacity, NULL);
    check_res(queue.free == NULL, "the queue's free should be `NULL` when pass in `NULL` to queue", &fail_cnt);
    test_queue_integrity(&queue, &fail_cnt, 0, true, start_capacity, NULL);

    hm_queue_free(&queue);
    
    print_end("QUEUE(DYNAMIC) | FUNC | INIT | CAPACITY: 64", fail_cnt);
    HM_TEST_COUNTER
    
}

void test_queue_fixed_enq() {
    int fail_cnt = 0;
    print_run("QUEUE(FIXED) | FUNC | ENQUEUE | CAPACITY: 64 TYPE: [INT]");
    hm_queue queue;
    int capacity = 64;    
    hm_queue_init(&queue, capacity, free);

    int fail = 0;
    // enqueue vals , the amount is same as capacity
    for (int i = 0; i < capacity; i++) {
        int* v = (int*)malloc(sizeof(int));
        *v = i * 10;
        if (hm_queue_enq(&queue, v) != hm_queue_ret_suc) {
            fail++;
            free(v);
        }
    }
    check_res(fail == 0, "enqueue vals should return suc with a reasonable amount", &fail_cnt);
    test_queue_integrity(&queue, &fail_cnt, capacity, false, capacity, free);
    
    // verify
    
    fail = 0;
    int** vals = (int**)queue.vals;
    for (int i = 0; i < capacity; i++) {
        int* v = vals[i];
        if (*v != i * 10) {
            fail++;
        }
    }
    check_res(fail == 0, "the vals is wrong in the queue", &fail_cnt);
    
    // enqueue more vals , let it return `full`
    fail = 0;
    for (int i = capacity; i < 2 * capacity; i++) {
        int* v = (int*)malloc(sizeof(int));
        *v = i * 10;
        if (hm_queue_enq(&queue, v) != hm_queue_ret_full) {
            fail++;
        } else {
            free(v);
        }
    }
    check_res(fail == 0, "the enqueue function should reuturn full when enqueue vals beyond the capacity", &fail_cnt);
    test_queue_integrity(&queue, &fail_cnt, capacity, false, capacity, free);
    

    fail = 0;
    vals = (int**)queue.vals;
    for (int i = 0; i < capacity; i++) {
        int* v = vals[i];
        if (*v != i * 10) {
            fail++;
        }
    }
    check_res(fail == 0, "the vals is wrong in the queue after enqueue vals beyond the capacity", &fail_cnt);
    
    
    hm_queue_free(&queue);
    
    
    print_end("QUEUE(FIXED) | FUNC | ENQUEUE | CAPACITY: 64 TYPE: [INT]", fail_cnt);
    HM_TEST_COUNTER
    
}


void test_queue_dynamic_enq() {
    int fail_cnt = 0;
    print_run("QUEUE(DYNMAIC) | FUNC | ENQUEUE | CAPACITY: 64 TYPE: [INT]");
    
    hm_queue queue;
    int start_capacity = 64;    
    hm_queue_init_dynamic_grow(&queue, start_capacity, free);

    int fail = 0;
    // enqueue vals , the amount is same as capacity
    for (int i = 0; i < start_capacity; i++) {
        int* v = (int*)malloc(sizeof(int));
        *v = i * 10;
        if (hm_queue_enq(&queue, v) != hm_queue_ret_suc) {
            fail++;
            free(v);
        }
    }
    check_res(fail == 0, "enqueue vals should return suc with a reasonable amount", &fail_cnt);
    test_queue_integrity(&queue, &fail_cnt, start_capacity, true, start_capacity, free);
    
    // verify
    
    fail = 0;
    int** vals = (int**)queue.vals;
    for (int i = 0; i < start_capacity; i++) {
        int* v = vals[i];
        if (*v != i * 10) {
            fail++;
        }
    }
    check_res(fail == 0, "the vals is wrong in the queue", &fail_cnt);
    
    // enqueue more vals , let it return `full`
    fail = 0;
    for (int i = start_capacity; i < 2 * start_capacity; i++) {
        int* v = (int*)malloc(sizeof(int));
        *v = i * 10;
        if (hm_queue_enq(&queue, v) != hm_queue_ret_suc) {
            fail++;
            free(v);
        }
    }
    check_res(fail == 0, "the enqueue function should reuturn suc when enqueue vals beyond the capacity", &fail_cnt);
    test_queue_integrity(&queue, &fail_cnt, start_capacity * 2, true, start_capacity, free);
    

    fail = 0;
    vals = (int**)queue.vals;
    for (int i = 0; i < start_capacity * 2; i++) {
        int* v = vals[i];
        if (*v != i * 10) {
            fail++;
        }
    }
    check_res(fail == 0, "the vals is wrong in the queue after enqueue vals beyond the capacity", &fail_cnt);
    
    
    hm_queue_free(&queue);
    
    print_end("QUEUE(DYNAMIC) | FUNC | ENQUEUE | CAPACITY: 64 TYPE: [INT]", fail_cnt);
    HM_TEST_COUNTER
    
}



void test_queue_fixed_deq() {
    int fail_cnt = 0;
    print_run("QUEUE(FIXED) | FUNC | DEQUEUE | CAPACITY: 8 TYPE: [INT]");
    
    hm_queue queue;
    int capacity = 8;    
    hm_queue_init(&queue, capacity, free);
    // push reasonable number of vals
    int fail_null = 0, fail_wrong = 0;
    for (int i = 0; i < capacity; i++) {
        // enqueue
        for (int j = 0; j <= i; j++) {
            int* v = (int*)malloc(sizeof(int));
            *v = j * 10;
            hm_queue_enq(&queue, v);
        }

        // enqueue and verify
        for (int j = 0; j <= i; j++) {
            test_queue_integrity(&queue, &fail_cnt, i + 1 - j, false, capacity, free);
            int* v = hm_queue_deq(&queue);
            if (v == NULL) {
                fail_null++;
            } else if (*v != j * 10) {
                fail_wrong++;
            }
            free(v);
        }
        
    }
    check_res(fail_null == 0, "the dequeue val shouldn't be NULL", &fail_cnt);
    check_res(fail_wrong == 0, "the dequeue val is wrong when run dequeue function", &fail_cnt);
    
    
    // push beyond the capacity of queue
    for (int i = capacity; i < capacity * 2; i++) {
        // enqueue
        for (int j = 0; j <= i; j++) {
            int* v = (int*)malloc(sizeof(int));
            *v = j * 10;
            if (hm_queue_enq(&queue, v) == hm_queue_ret_full) {
                free(v);
            }
        }
        
        // enqueue and verify
        for (int j = 0; j <= capacity - 1; j++) {
            test_queue_integrity(&queue, &fail_cnt, capacity - j, false, capacity, free);
            int* v = hm_queue_deq(&queue);
            if (v == NULL) {
                fail_null++;
            } else if (*v != j * 10) {
                fail_wrong++;
            }
            free(v);
        }
    }
    check_res(fail_null == 0, "the dequeue val shouldn't be NULL", &fail_cnt);
    check_res(fail_wrong == 0, "the dequeue val is wrong when run dequeue function", &fail_cnt);
    
    hm_queue_free(&queue);
    
    print_end("QUEUE(FIXED) | FUNC | DEQUEUE | CAPACITY: 8 TYPE: [INT]", fail_cnt);
    HM_TEST_COUNTER
    
}

void test_queue_dynamic_deq() {
    int fail_cnt = 0;
    print_run("QUEUE(DYNMAIC) | FUNC | DEQUEUE | CAPACITY: 8 TYPE: [INT]");
    
    hm_queue queue;
    int start_capacity = 8;    
    hm_queue_init_dynamic_grow(&queue, start_capacity, free);
    // push reasonable number of vals
    int fail_null = 0, fail_wrong = 0;
    for (int i = 0; i < start_capacity * 2; i++) {
        // enqueue
        for (int j = 0; j <= i; j++) {
            int* v = (int*)malloc(sizeof(int));
            *v = j * 10;
            hm_queue_enq(&queue, v);
        }

        // enqueue and verify
        for (int j = 0; j <= i; j++) {
            test_queue_integrity(&queue, &fail_cnt, i + 1 - j, true, start_capacity, free);
            int* v = hm_queue_deq(&queue);
            if (v == NULL) {
                fail_null++;
            } else if (*v != j * 10) {
                fail_wrong++;
            }
            free(v);
        }
        
    }
    check_res(fail_null == 0, "the dequeue val shouldn't be NULL", &fail_cnt);
    check_res(fail_wrong == 0, "the dequeue val is wrong when run dequeue function", &fail_cnt);
    
    hm_queue_free(&queue);
    
    print_end("QUEUE(DYNAMIC) | FUNC | DEQUEUE | CAPACITY: 8 TYPE: [INT]", fail_cnt);
    HM_TEST_COUNTER
    
}



void test_queue_fixed_peek() {
    int fail_cnt = 0;
    print_run("QUEUE(FIXED) | FUNC | PEEK | CAPACITY: 64 TYPE: [INT]");
    hm_queue queue;
    int capacity = 64;    
    int diff = 10;
    hm_queue_init(&queue, capacity, free);
    int fail_null = 0, fail_wrong = 0;
    for (int i = 0; i < capacity * 2; i++) {
        // enqueue and dequeue
        for (int j = 0; j < diff; j++) {
            int* v = (int*)malloc(sizeof(int));
            *v = (j + i) * 10;
            if (hm_queue_enq(&queue, v) == hm_queue_ret_full) {
                free(v);
            }
        }
        int* pointer = hm_queue_peek(&queue);
        test_queue_integrity(&queue, &fail_cnt, (diff > capacity ? capacity : diff), false, capacity, free);
        if (pointer == NULL) {
            fail_null == 0;
        } else if (*pointer != i * 10) {
            fail_wrong++;
        }
        for (int j = 0; j < diff; j++) {
            pointer = hm_queue_deq(&queue);
            free(pointer);
        }
    }

    check_res(fail_null == 0, "the peek value shouldn't be NULL", &fail_cnt);
    check_res(fail_wrong == 0, "the peek value is wrong", &fail_cnt);
    
    hm_queue_free(&queue);
    
    print_end("QUEUE(FIXED) | FUNC | PEEK | CAPACITY: 64 TYPE: [INT]", fail_cnt);
    HM_TEST_COUNTER
    
}

void test_queue_dynamic_peek() {
    int fail_cnt = 0;
    print_run("QUEUE(DYNMAIC) | FUNC | PEEK | CAPACITY: 64 TYPE: [INT]");
    
    hm_queue queue;
    int start_capacity = 64;    
    int diff = 10;
    hm_queue_init_dynamic_grow(&queue, start_capacity, free);
    int fail_null = 0, fail_wrong = 0;
    for (int i = 0; i < start_capacity * 2; i++) {
        // enqueue and dequeue
        for (int j = 0; j < diff; j++) {
            int* v = (int*)malloc(sizeof(int));
            *v = (j + i) * 10;
            hm_queue_enq(&queue, v);
        }
        int* pointer = hm_queue_peek(&queue);
        test_queue_integrity(&queue, &fail_cnt, diff, true, start_capacity, free);
        if (pointer == NULL) {
            fail_null == 0;
        } else if (*pointer != i * 10) {
            fail_wrong++;
        }
        for (int j = 0; j < diff; j++) {
            pointer = hm_queue_deq(&queue);
            free(pointer);
        }
    }

    check_res(fail_null == 0, "the peek value shouldn't be NULL", &fail_cnt);
    check_res(fail_wrong == 0, "the peek value is wrong", &fail_cnt);
    
    hm_queue_free(&queue);
    
    print_end("QUEUE(DYNAMIC) | FUNC | PEEK | CAPACITY: 64 TYPE: [INT]", fail_cnt);
    HM_TEST_COUNTER
    
}





void test_queue_fixed_clear() {
    int fail_cnt = 0;
    print_run("QUEUE(FIXED) | FUNC | CLEAR | CAPACITY: 64 TYPE: [INT]");
    
    hm_queue queue;
    int capacity = 64;    
    hm_queue_init(&queue, capacity, free);

    // enqueue
    for (int i = 0; i < capacity * 2; i++) {
        int* v = (int*)malloc(sizeof(int));
        *v = i * 10;
        if (hm_queue_enq(&queue, v) == hm_queue_ret_full) {
            free(v);
        }
    }
    test_queue_integrity(&queue, &fail_cnt, capacity, false, capacity, free);
    
    hm_queue_clear(&queue);
    
    test_queue_integrity(&queue, &fail_cnt, 0, false, capacity, free);

    int* val = hm_queue_deq(&queue);
    check_res(val == NULL, "the dequeue front val should be NULL after clear the queue", &fail_cnt);

    val = hm_queue_peek(&queue);
    check_res(val == NULL, "the peek front val should be NULL after clear the queue", &fail_cnt);

    hm_queue_free(&queue);
    print_end("QUEUE(FIXED) | FUNC | CLEAR | CAPACITY: 64 TYPE: [INT]", fail_cnt);
    HM_TEST_COUNTER
    
}

void test_queue_dynamic_clear() {
    int fail_cnt = 0;
    print_run("QUEUE(DYNMAIC) | FUNC | CLEAR | CAPACITY: 64 TYPE: [INT]");
    
    hm_queue queue;
    int start_capacity = 64;    
    hm_queue_init_dynamic_grow(&queue, start_capacity, free);

    // enqueue
    for (int i = 0; i < start_capacity * 2; i++) {
        int* v = (int*)malloc(sizeof(int));
        *v = i * 10;
        hm_queue_enq(&queue, v);
    }

    test_queue_integrity(&queue, &fail_cnt, start_capacity * 2, true, start_capacity, free);
    
    hm_queue_clear(&queue);
    
    test_queue_integrity(&queue, &fail_cnt, 0, true, start_capacity, free);

    int* val = hm_queue_deq(&queue);
    check_res(val == NULL, "the dequeue front val should be NULL after clear the queue", &fail_cnt);

    val = hm_queue_peek(&queue);
    check_res(val == NULL, "the peek front val should be NULL after clear the queue", &fail_cnt);

    hm_queue_free(&queue);
    
    print_end("QUEUE(DYNAMIC) | FUNC | CLEAR | CAPACITY: 64 TYPE: [INT]", fail_cnt);
    HM_TEST_COUNTER
    
}

void test_queue_fixed_free() {
    int fail_cnt = 0;
    print_run("QUEUE(FIXED) | FUNC | FREE | CAPACITY: 64 TYPE: [INT]");
    
    hm_queue queue;
    int capacity = 64;    
    hm_queue_init(&queue, capacity, free);

    // enqueue
    for (int i = 0; i < capacity * 2; i++) {
        int* v = (int*)malloc(sizeof(int));
        *v = i * 10;
        if (hm_queue_enq(&queue, v) == hm_queue_ret_full) {
            free(v);
        }
    }
    test_queue_integrity(&queue, &fail_cnt, capacity, false, capacity, free);
    
    hm_queue_free(&queue);
    
    test_queue_integrity(&queue, &fail_cnt, 0, false, 0, free);

    int* val = hm_queue_deq(&queue);
    check_res(val == NULL, "the dequeue front val should be NULL after free the queue", &fail_cnt);

    val = hm_queue_peek(&queue);
    check_res(val == NULL, "the peek front val should be NULL after free the queue", &fail_cnt);

    hm_queue_free(&queue);
    
    print_end("QUEUE(FIXED) | FUNC | FREE | CAPACITY: 64 TYPE: [INT]", fail_cnt);
    HM_TEST_COUNTER
    
}


void test_queue_dynamic_free() {
    int fail_cnt = 0;
    print_run("QUEUE(DYNMAIC) | FUNC | FREE | CAPACITY: 64 TYPE: [INT]");
    
    hm_queue queue;
    int start_capacity = 64;    
    hm_queue_init_dynamic_grow(&queue, start_capacity, free);

    // enqueue
    for (int i = 0; i < start_capacity * 2; i++) {
        int* v = (int*)malloc(sizeof(int));
        *v = i * 10;
        hm_queue_enq(&queue, v);
    }

    test_queue_integrity(&queue, &fail_cnt, start_capacity * 2, true, start_capacity, free);
    
    hm_queue_free(&queue);
    
    test_queue_integrity(&queue, &fail_cnt, 0, true, 0, free);

    int* val = hm_queue_deq(&queue);
    check_res(val == NULL, "the dequeue front val should be NULL after free the queue", &fail_cnt);

    val = hm_queue_peek(&queue);
    check_res(val == NULL, "the peek front val should be NULL after free the queue", &fail_cnt);

    hm_queue_free(&queue);
    
    print_end("QUEUE(DYNAMIC) | FUNC | FREE | CAPACITY: 64 TYPE: [INT]", fail_cnt);
    HM_TEST_COUNTER
    
}





void test_queue_fixed_func() {
    test_queue_fixed_init();                                    printf("\n");
    
    test_queue_fixed_enq();                                     printf("\n");
    
    test_queue_fixed_deq();                                     printf("\n");
    
    test_queue_fixed_peek();                                    printf("\n");

    test_queue_fixed_clear();                                   printf("\n");

    test_queue_fixed_free();                                    printf("\n");
    
}

void test_queue_dynamic_func() {
    test_queue_dynamic_init();                                  printf("\n");
    
    test_queue_dynamic_enq();                                   printf("\n");
    
    test_queue_dynamic_deq();                                   printf("\n");
    
    test_queue_dynamic_peek();                                  printf("\n");
    
    test_queue_dynamic_clear();                                 printf("\n");
    
    test_queue_dynamic_free();                                  printf("\n");
    
}

void function_test() {
    test_queue_fixed_func();

    test_queue_dynamic_func();
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

