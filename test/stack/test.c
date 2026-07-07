/*
 * Copyright (c) 2026 Flmpx
 * Licensed under MIT (see LICENSE).
 */

#include "../../include/hm_stack.h"
#include "../hm_test.h"
#include <stdlib.h>
#include <stdint.h>

// This variable can record the total number of failures and it can be used as a return value to check whether the test passed
int all_failure_num = 0;

// use a macro to replace the repetitive code 
#define HM_TEST_COUNTER \
    all_failure_num += fail_cnt;


// every test function ...

void test_stack_integrity(hm_stack* stack, int* fail_cnt, size_t top, bool dynamic_gorwth, size_t capacity) {
    // because the limitation of stack, the integrity test only test these
    check_res(stack->top == top, "TEST OF INTEGRITY: stack's top isn't the expected size", fail_cnt);
    check_res(stack->dynamic_grow == dynamic_gorwth, "TEST OF INTEGRITY: stack'dynamic signal is unexpected", fail_cnt);
    if (!dynamic_gorwth) {
        check_res(stack->capacity == capacity, "TEST OF INTEGRITY: stack's capacity isn't the expected size", fail_cnt);
    }
    check_res(stack->capacity >= stack->top, "TEST OF INTEGRITY: `capacity` should greater than `top`", fail_cnt);
    check_res(!(stack->capacity == 0 && stack->vals != NULL), "stack's capacity is 0, but vals have memory", fail_cnt);
    check_res(!(stack->capacity != 0 && stack->vals == NULL), "stack's capacity isn't 0, but vals is NULL", fail_cnt);
} 


void test_stack_fixed_init() {
    // fixed-size
    int fail_cnt = 0;
    print_run("STACK(FIXED) | FUNC | INIT | CAPACITY: 64");
    hm_stack stack;
    size_t capacity = 64;
    // pass in `free` for stack
    hm_stack_init(&stack, capacity, free);
    test_stack_integrity(&stack, &fail_cnt, 0, false, capacity);

    check_res(stack.capacity == capacity, "the stack's capacity isn't equal to expected capacity", &fail_cnt);
    check_res(stack.dynamic_grow == false, "the stack's dynamic-gorwth should is `false`", &fail_cnt);
    check_res(stack.free == free, "the stack's free should be `free` when pass in `free` to stack", &fail_cnt);
    check_res(stack.top == 0, "the stack's top should be 0", &fail_cnt);
    check_res(stack.vals != NULL, "the stack's vals shouldn't be NULL with a small capacity", &fail_cnt);
    
    hm_stack_free(&stack);
    
    // pass in NULL for stack
    
    hm_stack_init(&stack, capacity, NULL);
    check_res(stack.free == NULL, "the stack's free should be `NULL` when pass in `NULL` to stack", &fail_cnt);
    test_stack_integrity(&stack, &fail_cnt, 0, false, capacity);

    hm_stack_free(&stack);

    print_end("STACK(FIXED) | FUNC | INIT | CAPACITY: 64", fail_cnt);

    HM_TEST_COUNTER

}

void test_stack_dynamic_init() {
    // dynamic-growth
    int fail_cnt = 0;
    print_run("STACK(DYNAMIC) | FUNC | INIT | CAPACITY: 64");
    hm_stack stack;
    size_t capacity = 64;
    // pass in `free` for stack
    hm_stack_init_dynamic_grow(&stack, capacity, free);
    test_stack_integrity(&stack, &fail_cnt, 0, true, capacity);

    check_res(stack.capacity == capacity, "the stack's capacity isn't equal to expected capacity", &fail_cnt);
    check_res(stack.dynamic_grow == true, "the stack's dynamic-gorwth should is `true`", &fail_cnt);
    check_res(stack.free == free, "the stack's free should be `free` when pass in `free` to stack", &fail_cnt);
    check_res(stack.top == 0, "the stack's top should be 0", &fail_cnt);
    check_res(stack.vals != NULL, "the stack's vals shouldn't be NULL with a small capacity", &fail_cnt);
    
    hm_stack_free(&stack);
    
    // pass in NULL for stack
    
    hm_stack_init_dynamic_grow(&stack, capacity, NULL);
    check_res(stack.free == NULL, "the stack's free should be `NULL` when pass in `NULL` to stack", &fail_cnt);
    test_stack_integrity(&stack, &fail_cnt, 0, true, capacity);

    hm_stack_free(&stack);

    print_end("STACK(DYNAMIC) | FUNC | INIT | CAPACITY: 64", fail_cnt);

    HM_TEST_COUNTER
}


void test_stack_fixed_push() {
    int fail_cnt = 0;
    print_run("STACK(FIXED) | FUNC | PUSH | CAPACITY: 64 TYPE: [INT]");
    int capacity = 64;
    hm_stack stack;
    hm_stack_init(&stack, capacity, free);
    
    int fail = 0;

    // push vals, the amount is same as capacity
    for (int i = 0; i < capacity; i++) {
        int* v = (int*)malloc(sizeof(int));
        *v = i * 10;
        if (hm_stack_push(&stack, v) != hm_stack_ret_suc) {
            fail++;
            free(v);
        }
    }
    check_res(fail == 0, "push vals should return suc with a reasonable amount", &fail_cnt);
    test_stack_integrity(&stack, &fail_cnt, capacity, false, capacity);
    // verify

    fail = 0;
    int** vals = (int**)stack.vals;
    for (int i = 0; i < capacity; i++) {
        int* v = vals[i];
        if (*v != i * 10) {
            fail++;
        }
    }
    check_res(fail == 0, "the vals is wrong in the stack", &fail_cnt);

    // push more vals, let it return `full`
    fail = 0;
    for (int i = capacity; i < 2 * capacity; i++) {
        int* v = (int*)malloc(sizeof(int));
        *v = i * 10;
        if (hm_stack_push(&stack, v) != hm_stack_ret_full) {
            fail++;
        } else {
            free(v);
        }
    }
    check_res(fail == 0, "the push function should reuturn full when push vals beyond the capacity", &fail_cnt);
    test_stack_integrity(&stack, &fail_cnt, capacity, false, capacity);


    fail = 0;
    vals = (int**)stack.vals;
    for (int i = 0; i < capacity; i++) {
        int* v = vals[i];
        if (*v != i * 10) {
            fail++;
        }
    }
    check_res(fail == 0, "the vals is wrong in the stack after push vals beyond the capacity", &fail_cnt);

    hm_stack_free(&stack);

    print_end("STACK(FIXED) | FUNC | PUSH | CAPACITY: 64 TYPE: [INT]", fail_cnt);

    HM_TEST_COUNTER

}

void test_stack_dynamic_push() {
    int fail_cnt = 0;
    print_run("STACK(DYNAMIC) | FUNC | PUSH | CAPACITY: 64 TYPE: [INT]");

    hm_stack stack;
    int start_capacity = 64;
    hm_stack_init_dynamic_grow(&stack, start_capacity, free);

    int fail = 0;
    // push vals, the amount is same as start_capacity
    for (int i = 0; i < start_capacity; i++) {
        int* v = (int*)malloc(sizeof(int));
        *v = i * 10;
        if (hm_stack_push(&stack, v) != hm_stack_ret_suc) {
            fail++;
            free(v);
        }
    }
    check_res(fail == 0, "push vals should return suc with a reasonable amount", &fail_cnt);
    test_stack_integrity(&stack, &fail_cnt, start_capacity, true, start_capacity);


    // verify
    fail = 0;
    int** vals = (int**)stack.vals;
    for (int i = 0; i < start_capacity; i++) {
        int* v = vals[i];
        if (*v != i * 10) {
            fail++;
        }
    }
    check_res(fail == 0, "the vals is wrong in the stack", &fail_cnt);

    // push more vals
    fail = 0;
    for (int i = start_capacity; i < 2 * start_capacity; i++) {
        int* v = (int*)malloc(sizeof(int));
        *v = i * 10;
        if (hm_stack_push(&stack, v) != hm_stack_ret_suc) {
            fail++;
            free(v);
        }
    }
    
    check_res(fail == 0, "push function should return suc when push vals beyond the capacity", &fail_cnt);
    test_stack_integrity(&stack, &fail_cnt, start_capacity * 2, true, start_capacity);


    // verify

    fail = 0;
    vals = (int**)stack.vals;
    for (int i = 0; i < start_capacity * 2; i++) {
        int* v = vals[i];
        if (*v != i * 10) {
            fail++;
        }
    }

    check_res(fail == 0, "the vals is wrong in the stack after push vals beyond the capacity", &fail_cnt);

    hm_stack_free(&stack);

    print_end("STACK(DYNAMIC) | FUNC | PUSH | CAPACITY: 64 TYPE: [INT]", fail_cnt);

    HM_TEST_COUNTER
}

void test_stack_fixed_peek() {
    int fail_cnt = 0;
    print_run("STACK(FIXED) | FUNC | PEEK | CAPACITY: 32 TYPE: [INT]");

    int capacity = 32;
    int num = 0;        // This variable record the real number of stack
    hm_stack stack;
    hm_stack_init(&stack, capacity, free);

    int fail_wrong = 0, fail_null = 0;
    // push reasonable number of vals
    for (int i = 0; i < capacity; i++) {
        int* v = (int*)malloc(sizeof(int));
        *v = i * 10;
        hm_stack_push(&stack, v);

        // verify
        num++;

        int* p = hm_stack_peek(&stack);
        test_stack_integrity(&stack, &fail_cnt, num, false, capacity);
        if (p == NULL) {
            fail_null++;
        } else if (*p != (num - 1) * 10) {
            fail_wrong++;
        }
    }
    check_res(fail_null == 0, "the val peeked by the peek function shouldn't be NULL after push some vals to stack", &fail_cnt);
    check_res(fail_wrong == 0, "the val peeked by the peek function is wrong", &fail_cnt);
    
    // push beyond the capacity
    fail_null = fail_wrong = 0;
    for (int i = capacity; i < capacity * 2; i++) {
        int v = i * 10;
        hm_stack_push(&stack, &v);

        // according the logic, the val shouldn't push to fixed-size and full stack
        int* p = hm_stack_peek(&stack);
        test_stack_integrity(&stack, &fail_cnt, num, false, capacity);
        if (p == NULL) {
            fail_null++;
        } else if (*p != (num - 1) * 10) {
            fail_wrong++;
        }
    }
    check_res(fail_null == 0, "the val peeked by the peek function shouldn't be NULL after push more vals than the capacity of stack", &fail_cnt);
    check_res(fail_wrong == 0, "the val peeked by the peek function is wrong after push more vals than the capacity of stack", &fail_cnt);
    
    hm_stack_free(&stack);
    print_end("STACK(FIXED) | FUNC | PEEK | CAPACITY: 32 TYPE: [INT]", fail_cnt);
    HM_TEST_COUNTER

}

void test_stack_dynamic_peek() {
    int fail_cnt = 0;

    print_run("STACK(DYNAMIC) | FUNC | PEEK | CAPACITY: 32 TYPE: [INT]");

    int start_capacity = 32;
    int num = 0;        // This variable record the real number of stack
    hm_stack stack;
    hm_stack_init_dynamic_grow(&stack, start_capacity, free);

    
    int fail_wrong = 0, fail_null = 0;
    // push vals(include beyond the capacity)
    for (int i = 0; i < start_capacity * 2; i++) {
        int* v = (int*)malloc(sizeof(int));
        *v = i * 10;
        hm_stack_push(&stack, v);

        // verify
        num++;

        int* p = hm_stack_peek(&stack);
        test_stack_integrity(&stack, &fail_cnt, num, true, start_capacity);
        if (p == NULL) {
            fail_null++;
        } else if (*p != (num - 1) * 10) {
            fail_wrong++;
        }
    }
    check_res(fail_null == 0, "the val peeked by the peek function shouldn't be NULL after push some vals to stack", &fail_cnt);
    check_res(fail_wrong == 0, "the val peeked by the peek function is wrong", &fail_cnt);


    hm_stack_free(&stack);


    print_end("STACK(DYNAMIC) | FUNC | PEEK | CAPACITY: 32 TYPE: [INT]", fail_cnt);
    HM_TEST_COUNTER

}


void test_stack_fixed_pop() {
    int fail_cnt = 0;
    print_run("STACK(FIXED) | FUNC | POP | CAPACITY: 8 TYPE: [INT]");

    hm_stack stack;
    int capacity = 8;
    hm_stack_init(&stack, capacity, free);
    // push reasonable number of vals
    int fail_null = 0, fail_wrong = 0;
    for (int i = 0; i < capacity; i++) {
        // push
        for (int j = 0; j <= i; j++) {
            int* v = (int*)malloc(sizeof(int));
            *v = j * 10;
            hm_stack_push(&stack, v);
        }

        // pop and verify
        int fail = 0;
        for (int j = i; j >= 0; j--) {
            test_stack_integrity(&stack, &fail_cnt, j + 1, false, capacity);
            int* v = hm_stack_pop(&stack);
            if (v == NULL) {
                fail_null++;
            } else if (*v != j * 10) {
                fail_wrong++;
            }
            free(v);
        }
    }
    
    check_res(fail_null == 0, "the pop val shouldn't be NULL", &fail_cnt);
    check_res(fail_wrong == 0, "the pop val is wrong when run pop function", &fail_cnt);

    // push beyond the capacity of stack
    fail_null = fail_wrong = 0;
    for (int i = capacity; i < capacity * 2; i++) {
        // push
        for (int j = 0; j <= i; j++) {
            int* v = (int*)malloc(sizeof(int));
            *v = j * 10;
            if (hm_stack_push(&stack, v) == hm_stack_ret_full) {
                free(v);
            }
        }

        // pop and verify

        for (int j = capacity - 1; j >= 0; j--) {
            test_stack_integrity(&stack, &fail_cnt, j + 1, false, capacity);
            int* v = hm_stack_pop(&stack);
            if (v == NULL) {
                fail_null++;
            } else if (*v != j * 10) {
                fail_wrong++;
            }
            free(v);
        }
    }
    check_res(fail_null == 0, "the pop val shouldn't be NULL after push more vals than the capacity of stack", &fail_cnt);
    check_res(fail_wrong == 0, "the pop val is wrong when run pop function after push more vals than the capacity of stack", &fail_cnt);
    

    hm_stack_free(&stack);
    print_end("STACK(FIXED) | FUNC | POP | CAPACITY: 8 TYPE: [INT]", fail_cnt);
    HM_TEST_COUNTER
}

void test_stack_dynamic_pop() {
    int fail_cnt = 0;
    print_run("STACK(DYNAMIC) | FUNC | POP | CAPACITY: 8 TYPE: [INT]");

    hm_stack stack;
    int start_capacity = 8;
    hm_stack_init_dynamic_grow(&stack, start_capacity, free);
    // push reasonable number of vals
    int fail_null = 0, fail_wrong = 0;
    for (int i = 0; i < start_capacity * 2; i++) {
        // push
        for (int j = 0; j <= i; j++) {
            int* v = (int*)malloc(sizeof(int));
            *v = j * 10;
            hm_stack_push(&stack, v);
        }

        // pop and verify
        int fail = 0;
        for (int j = i; j >= 0; j--) {
            test_stack_integrity(&stack, &fail_cnt, j + 1, true, start_capacity);
            int* v = hm_stack_pop(&stack);
            if (v == NULL) {
                fail_null++;
            } else if (*v != j * 10) {
                fail_wrong++;
            }
            free(v);
        }
    }
    
    check_res(fail_null == 0, "the pop val shouldn't be NULL", &fail_cnt);
    check_res(fail_wrong == 0, "the pop val is wrong when run pop function", &fail_cnt);


    hm_stack_free(&stack);
    print_end("STACK(DYNAMIC) | FUNC | POP | CAPACITY: 8 TYPE: [INT]", fail_cnt);
    HM_TEST_COUNTER
}

void test_stack_fixed_clear() {
    int fail_cnt = 0;
    print_run("STACK(FIXED) | FUNC | CLEAR | CAPACITY: 64 TYPE: [INT]");
    hm_stack stack;
    int capacity = 64;
    hm_stack_init(&stack, capacity, free);

    // push
    for (int i = 0; i < capacity * 2; i++) {
        int* v = (int*)malloc(sizeof(int));
        *v = i * 10;
        if (hm_stack_push(&stack, v) == hm_stack_ret_full) {
            free(v);
        }
    }

    test_stack_integrity(&stack, &fail_cnt, capacity, false, capacity);

    hm_stack_clear(&stack);

    test_stack_integrity(&stack, &fail_cnt, 0, false, capacity);

    int* val = hm_stack_peek(&stack);
    check_res(val == NULL, "the peek top should be NULL after clear the stack", &fail_cnt);
    
    val = hm_stack_pop(&stack);
    check_res(val == NULL, "the pop top should be NULL after clear the stack", &fail_cnt);
    
    hm_stack_free(&stack);
    print_end("STACK(FIXED) | FUNC | CLEAR | CAPACITY: 64 TYPE: [INT]", fail_cnt);

    HM_TEST_COUNTER
}

void test_stack_dynamic_clear() {
    int fail_cnt = 0;
    print_run("STACK(DYNAMIC) | FUNC | CLEAR | CAPACITY: 64 TYPE: [INT]");

    hm_stack stack;
    int start_capacity = 64;
    hm_stack_init_dynamic_grow(&stack, start_capacity, free);

    // push
    for (int i = 0; i < start_capacity * 2; i++) {
        int* v = (int*)malloc(sizeof(int));
        *v = i * 10;
        hm_stack_push(&stack, v);
    }

    test_stack_integrity(&stack, &fail_cnt, start_capacity * 2, true, start_capacity);

    hm_stack_clear(&stack);

    test_stack_integrity(&stack, &fail_cnt, 0, true, start_capacity);

    int* val = hm_stack_peek(&stack);
    check_res(val == NULL, "the peek top should be NULL after clear the stack", &fail_cnt);
    
    val = hm_stack_pop(&stack);
    check_res(val == NULL, "the pop top should be NULL after clear the stack", &fail_cnt);

    print_end("STACK(DYNAMIC) | FUNC | CLEAR | CAPACITY: 64 TYPE: [INT]", fail_cnt);

    hm_stack_free(&stack);
    HM_TEST_COUNTER
}

void test_stack_fixed_free() {
    int fail_cnt = 0;
    print_run("STACK(FIXED) | FUNC | FREE | CAPACITY: 64 TYPE: [INT]");

    hm_stack stack;
    int capacity = 64;
    hm_stack_init(&stack, capacity, free);

    // push
    for (int i = 0; i < capacity * 2; i++) {
        int* v = (int*)malloc(sizeof(int));
        *v = i * 10;
        if (hm_stack_push(&stack, v) == hm_stack_ret_full) {
            free(v);
        }
    }
    test_stack_integrity(&stack, &fail_cnt, capacity, false, capacity);
    hm_stack_free(&stack);
    test_stack_integrity(&stack, &fail_cnt, 0, false, 0);

    int* val = hm_stack_peek(&stack);
    check_res(val == NULL, "the peek top should be NULL after free the stack", &fail_cnt);
    
    val = hm_stack_pop(&stack);
    check_res(val == NULL, "the pop top should be NULL after free the stack", &fail_cnt);

    print_end("STACK(FIXED) | FUNC | FREE | CAPACITY: 64 TYPE: [INT]", fail_cnt);

    HM_TEST_COUNTER
}

void test_stack_dynamic_free() {
    int fail_cnt = 0;
    print_run("STACK(DYNAMIC) | FUNC | FREE | CAPACITY: 64 TYPE: [INT]");

    hm_stack stack;
    int start_capacity = 64;
    hm_stack_init_dynamic_grow(&stack, start_capacity, free);

    // push
    for (int i = 0; i < start_capacity * 2; i++) {
        int* v = (int*)malloc(sizeof(int));
        *v = i * 10;
        hm_stack_push(&stack, v);
    }

    test_stack_integrity(&stack, &fail_cnt, start_capacity * 2, true, start_capacity);

    hm_stack_free(&stack);

    test_stack_integrity(&stack, &fail_cnt, 0, true, 0);

    int* val = hm_stack_peek(&stack);
    check_res(val == NULL, "the peek top should be NULL after free the stack", &fail_cnt);
    
    val = hm_stack_pop(&stack);
    check_res(val == NULL, "the pop top should be NULL after free the stack", &fail_cnt);

    print_end("STACK(DYNAMIC) | FUNC | FREE | CAPACITY: 64 TYPE: [INT]", fail_cnt);

    HM_TEST_COUNTER
}


void test_empty_fixed_stack_oper() {
    int fail_cnt = 0;
    print_run("STACK(FIXED) | BOUNDARY | OPER EMPTY STACK | CAPACITY: 64 TYPE: [INT]");
    
    hm_stack stack;
    int val = 10;

    // fixed-size
    int capacity = 64;
    hm_stack_init(&stack, capacity, NULL);
    // peek
    void* pointer = hm_stack_peek(&stack);
    test_stack_integrity(&stack, &fail_cnt, 0, false, capacity);
    check_res(pointer == NULL, "the peek top should be NULL when stack is empty", &fail_cnt);
    // pop
    pointer = hm_stack_pop(&stack);
    test_stack_integrity(&stack, &fail_cnt, 0, false, capacity);
    check_res(pointer == NULL, "the peek top should be NULL when stack is empty", &fail_cnt);
    // push
    hm_stack_push(&stack, &val);
    test_stack_integrity(&stack, &fail_cnt, 1, false, capacity);
    check_res(*(int*)(stack.vals[0]) == val, "the val in stack is wrong when push val in a empty stack", &fail_cnt);
    // clear
    hm_stack_clear(&stack);
    test_stack_integrity(&stack, &fail_cnt, 0, false, capacity);
    
    // free
    hm_stack_free(&stack);
    test_stack_integrity(&stack, &fail_cnt, 0, false, 0);

    
    print_end("STACK(FIXED) | BOUNDARY | OPER EMPTY STACK | CAPACITY: 64 TYPE: [INT]", fail_cnt);

    HM_TEST_COUNTER
    
    
}

void test_empty_dynamic_stack_oper() {
    int fail_cnt = 0;
    print_run("STACK(DYNAMIC) | BOUNDARY | OPER EMPTY STACK | CAPACITY: 64 TYPE: [INT]");
    
    hm_stack stack;
    int val = 10;
    
    // fixed-size
    int start_capacity = 64;
    hm_stack_init_dynamic_grow(&stack, start_capacity, NULL);
    // peek
    void* pointer = hm_stack_peek(&stack);
    test_stack_integrity(&stack, &fail_cnt, 0, true, start_capacity);
    check_res(pointer == NULL, "the peek top should be NULL when stack is empty", &fail_cnt);
    // pop
    pointer = hm_stack_pop(&stack);
    test_stack_integrity(&stack, &fail_cnt, 0, true, start_capacity);
    check_res(pointer == NULL, "the peek top should be NULL when stack is empty", &fail_cnt);
    // push
    hm_stack_push(&stack, &val);
    test_stack_integrity(&stack, &fail_cnt, 1, true, start_capacity);
    check_res(*(int*)(stack.vals[0]) == val, "the val in stack is wrong when push val in a empty stack", &fail_cnt);
    // clear
    hm_stack_clear(&stack);
    test_stack_integrity(&stack, &fail_cnt, 0, true, start_capacity);
    
    // free
    hm_stack_free(&stack);
    test_stack_integrity(&stack, &fail_cnt, 0, true, start_capacity);
    
    
    print_end("STACK(DYNAMIC) | BOUNDARY | OPER EMPTY STACK | CAPACITY: 64 TYPE: [INT]", fail_cnt);
    
    HM_TEST_COUNTER
    
}


void test_full_fixed_stack_oper() {
    int fail_cnt = 0;
    print_run("STACK(FIXED) | BOUNDARY | OPER FULL STACK | CAPACITY: 64 TYPE: [INT]");

    hm_stack stack;
    int capacity = 64;
    hm_stack_init(&stack, capacity, free);
    // push to full
    for (int i = 0; i < capacity; i++) {
        int* v = (int*)malloc(sizeof(int));
        *v = i * 10;
        hm_stack_push(&stack, v);
    }

    // push 
    int* val = (int*)malloc(sizeof(int));
    *val = -1;
    hm_stack_ret ret = hm_stack_push(&stack, val);
    test_stack_integrity(&stack, &fail_cnt, capacity, false, capacity);
    check_res(ret == hm_stack_ret_full, "push function should return full when fixed-size stack is full", &fail_cnt);

    int* top = hm_stack_peek(&stack);
    check_res(*top != -1, "the peek val is wrong after push a val in a full and fixed-size stack", &fail_cnt);
    
    if (ret == hm_stack_ret_full) {
        free(val);
    }

    hm_stack_free(&stack);
    
    print_end("STACK(FIXED) | BOUNDARY | OPER FULL STACK | CAPACITY: 64 TYPE: [INT]", fail_cnt);
    
    HM_TEST_COUNTER
}

void test_full_dynamic_stack_oper() {
    int fail_cnt = 0;
    print_run("STACK(DYNAMIC) | BOUNDARY | OPER FULL STACK | CAPACITY: 64 TYPE: [INT]");
    
    hm_stack stack;
    int start_capacity = 64;
    hm_stack_init_dynamic_grow(&stack, start_capacity, free);
    // push to full
    for (int i = 0; i < start_capacity; i++) {
        int* v = (int*)malloc(sizeof(int));
        *v = i * 10;
        hm_stack_push(&stack, v);
    }
    
    // push 
    int* val = (int*)malloc(sizeof(int));
    *val = -1;
    hm_stack_ret ret = hm_stack_push(&stack, val);
    test_stack_integrity(&stack, &fail_cnt, start_capacity + 1, true, start_capacity);
    check_res(ret == hm_stack_ret_suc, "push function should return suc when dynamic stack is full", &fail_cnt);
    
    val = hm_stack_peek(&stack);
    check_res(*val == -1, "the peek val is wrong after push a val in a full and dynamic stack", &fail_cnt);
    
    
    hm_stack_free(&stack);
    
    print_end("STACK(DYNAMIC) | BOUNDARY | OPER FULL STACK | CAPACITY: 64 TYPE: [INT]", fail_cnt);
    HM_TEST_COUNTER
}


void test_no_capacity_fixed_stack() {
    int fail_cnt = 0;
    print_run("STACK(FIEXE) | BOUNDARY | NO CAPACITY STACK OPER | TYPE: [INT]");
    hm_stack stack;
    int capacity = 0;
    hm_stack_init(&stack, capacity, free);
    test_stack_integrity(&stack, &fail_cnt, 0, false, capacity);
    // push
    int* val = (int*)malloc(sizeof(int));
    *val = -1;
    hm_stack_ret ret = hm_stack_push(&stack, val);
    test_stack_integrity(&stack, &fail_cnt, 0, false, capacity);
    
    check_res(ret == hm_stack_ret_full, "it should return full when push a val in 0-capacity and fixed-sizestack", &fail_cnt);
    if (ret == hm_stack_ret_full) {
        free(val);
    }

    // peek
    check_res(hm_stack_peek(&stack) == NULL, "the peek val should be NULL in a 0-capacity and fixed-sizestack", &fail_cnt);
    test_stack_integrity(&stack, &fail_cnt, 0, false, capacity);

    // push
    check_res(hm_stack_pop(&stack) == NULL, "the pop val should be NULL in a 0-capacity and fixed-sizestack", &fail_cnt);
    test_stack_integrity(&stack, &fail_cnt, 0, false, capacity);
    
    // clear
    hm_stack_clear(&stack);
    test_stack_integrity(&stack, &fail_cnt, 0, false, capacity);
    
    // free
    hm_stack_free(&stack);
    test_stack_integrity(&stack, &fail_cnt, 0, false, capacity);
    
    
    print_end("STACK(FIEXE) | BOUNDARY | NO CAPACITY STACK OPER | TYPE: [INT]", fail_cnt);
    HM_TEST_COUNTER
}


void test_no_capacity_dynamic_stack() {
    int fail_cnt = 0;
    print_run("STACK(DYNAMIC) | BOUNDARY | NO CAPACITY STACK OPER | TYPE: [INT]");

    hm_stack stack;
    int capacity = 0;
    hm_stack_init_dynamic_grow(&stack, capacity, free);
    test_stack_integrity(&stack, &fail_cnt, 0, true, capacity);
    // push
    int* val = (int*)malloc(sizeof(int));
    *val = -1;
    hm_stack_ret ret = hm_stack_push(&stack, val);
    test_stack_integrity(&stack, &fail_cnt, 1, true, capacity);
    
    check_res(ret == hm_stack_ret_suc, "it should return suc when push a val in 0-capacity and dynamic-growth stack", &fail_cnt);
    
    // peek
    int* pointer = hm_stack_peek(&stack);
    check_res(pointer == val, "the peek val is wrong after push a val in 0-capacity and dynamic-growth stack", &fail_cnt);
    test_stack_integrity(&stack, &fail_cnt, 1, true, capacity);
    
    // pop
    pointer = hm_stack_pop(&stack);
    check_res(pointer == val, "the pop val is wrong after push a val in 0-capacity and dynamic-growth stack", &fail_cnt);
    test_stack_integrity(&stack, &fail_cnt, 0, true, capacity);
    if (pointer == val) {
        free(val);
    }
    
    // clear
    hm_stack_clear(&stack);
    test_stack_integrity(&stack, &fail_cnt, 0, true, capacity);
    
    // free
    hm_stack_free(&stack);
    test_stack_integrity(&stack, &fail_cnt, 0, true, capacity);
    
    
    print_end("STACK(DYNAMIC) | BOUNDARY | NO CAPACITY STACK OPER | TYPE: [INT]", fail_cnt);
    HM_TEST_COUNTER
    
}

void test_init_big_capacity_fixed_stack() {
    int fail_cnt = 0;
    print_run("STACK(FIEXE) | BOUNDARY | INIT BIG CAPACITY STACK | TYPE: [INT]");
    
    hm_stack stack;
    hm_stack_ret ret;
    size_t capaicty;

    // SIZE_MAX 
    capaicty = SIZE_MAX;
    ret = hm_stack_init(&stack, capaicty, free);
    check_res(ret == hm_stack_ret_error, "init SIZE_MAX-capacity stack should return error", &fail_cnt);
    
    // SIZE_MAX / 2
    capaicty = SIZE_MAX / 2;
    ret = hm_stack_init(&stack, capaicty, free);
    check_res(ret == hm_stack_ret_error, "init SIZE_MAX/2-capacity stack should return error", &fail_cnt);
    
    // SIZE_MAX * ( 2 / 3)
    capaicty = SIZE_MAX / 3 * 2;
    ret = hm_stack_init(&stack, capaicty, free);
    check_res(ret == hm_stack_ret_error, "init SIZE_MAX*(2/3)-capacity stack should return error", &fail_cnt);
    
    // SIZE_MAX / sizeof(void*)
    capaicty = SIZE_MAX / sizeof(void*);
    ret = hm_stack_init(&stack, capaicty, free);
    check_res(ret == hm_stack_ret_error, "init SIZE_MAX/sizeof(void*)-capacity stack should return error", &fail_cnt);


    print_end("STACK(FIEXE) | BOUNDARY | INIT BIG CAPACITY STACK | TYPE: [INT]", fail_cnt);
    HM_TEST_COUNTER
    
}

void test_init_big_capacity_dynamic_stack() {
    int fail_cnt = 0;
    print_run("STACK(DYNAMIC) | BOUNDARY | INIT BIG CAPACITY STACK | TYPE: [INT]");
    
    hm_stack stack;
    hm_stack_ret ret;
    size_t start_capaicty;
    
    // SIZE_MAX 
    start_capaicty = SIZE_MAX;
    ret = hm_stack_init_dynamic_grow(&stack, start_capaicty, free);
    check_res(ret == hm_stack_ret_error, "init SIZE_MAX-capacity stack should return error", &fail_cnt);
    
    // SIZE_MAX / 2
    start_capaicty = SIZE_MAX / 2;
    ret = hm_stack_init_dynamic_grow(&stack, start_capaicty, free);
    check_res(ret == hm_stack_ret_error, "init SIZE_MAX/2-capacity stack should return error", &fail_cnt);
    
    // SIZE_MAX * ( 2 / 3)
    start_capaicty = SIZE_MAX / 3 * 2;
    ret = hm_stack_init_dynamic_grow(&stack, start_capaicty, free);
    check_res(ret == hm_stack_ret_error, "init SIZE_MAX*(2/3)-capacity stack should return error", &fail_cnt);

    // SIZE_MAX / sizeof(void*)
    start_capaicty = SIZE_MAX / sizeof(void*);
    ret = hm_stack_init_dynamic_grow(&stack, start_capaicty, free);
    check_res(ret == hm_stack_ret_error, "init SIZE_MAX/sizeof(void*)-capacity stack should return error", &fail_cnt);
    
    print_end("STACK(DYNAMIC) | BOUNDARY | INIT BIG CAPACITY STACK | TYPE: [INT]", fail_cnt);
    HM_TEST_COUNTER
    
}


void test_stack_fixed_func() {
    test_stack_fixed_init();                                        printf("\n");

    test_stack_fixed_push();                                        printf("\n");

    test_stack_fixed_peek();                                        printf("\n");

    test_stack_fixed_pop();                                         printf("\n");

    test_stack_fixed_clear();                                       printf("\n");

    test_stack_fixed_free();                                        printf("\n");
}


void test_stack_dynamic_func() {
    test_stack_dynamic_init();                                      printf("\n");

    test_stack_dynamic_push();                                      printf("\n");

    test_stack_dynamic_peek();                                      printf("\n");

    test_stack_dynamic_pop();                                       printf("\n");

    test_stack_dynamic_clear();                                     printf("\n");

    test_stack_dynamic_free();                                      printf("\n");
}



void test_stack_fixed_boundary() {
    test_empty_fixed_stack_oper();                                  printf("\n");

    test_full_fixed_stack_oper();                                   printf("\n");

    test_no_capacity_fixed_stack();                                 printf("\n");

    test_init_big_capacity_fixed_stack();                           printf("\n");
}

void test_stack_dynamic_boundary() {
    test_empty_dynamic_stack_oper();                                printf("\n");

    test_full_dynamic_stack_oper();                                 printf("\n");

    test_no_capacity_dynamic_stack();                               printf("\n");

    test_init_big_capacity_dynamic_stack();                           printf("\n");

}

void function_test() {
    test_stack_fixed_func();                                             

    test_stack_dynamic_func();      
}

void boundary_test() {
    test_stack_fixed_boundary();

    test_stack_dynamic_boundary();
}

int main()
{
    // Group the test roughly
    function_test();
    boundary_test();
    return all_failure_num;
}

