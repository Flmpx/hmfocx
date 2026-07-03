#include "../../include/hm_stack.h"
#include "../hm_test.h"
#include <stdlib.h>

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

void test_stack_fixed() {
    test_stack_fixed_init();                                        printf("\n");

    test_stack_fixed_push();                                        printf("\n");

    test_stack_fixed_peek();                                        printf("\n");
}


void test_stack_dynamic() {
    test_stack_dynamic_init();                                      printf("\n");

    test_stack_dynamic_push();                                      printf("\n");

    test_stack_dynamic_peek();                                      printf("\n");
}

void function_test() {
    test_stack_fixed();                                             

    test_stack_dynamic();      
}

void boundary_test() {
    
}

void stress_test() {
    
}

int main()
{
    // Group the test roughly
    function_test();
    boundary_test();
    stress_test();
    return all_failure_num;
}

