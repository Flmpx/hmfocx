#include "../../include/hm_stack.h"
#include "../hm_test.h"
#include <stdlib.h>

// This variable can record the total number of failures and it can be used as a return value to check whether the test passed
int all_failure_num = 0;

// use a macro to replace the repetitive code 
#define HM_TEST_COUNTER \
    all_failure_num += fail_cnt;


// every test function ...

void test_stack_integrity(hm_stack* stack, int* fail_cnt) {
    // because the limitation of stack, the integrity test only test these
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
    test_stack_integrity(&stack, &fail_cnt);

    check_res(stack.capacity == capacity, "the stack's capacity isn't equal to expected capacity", &fail_cnt);
    check_res(stack.dynamic_grow == false, "the stack's dynamic-gorwth should is `false`", &fail_cnt);
    check_res(stack.free == free, "the stack's free should be `free` when pass in `free` to stack", &fail_cnt);
    check_res(stack.top == 0, "the stack's top should be 0", &fail_cnt);
    check_res(stack.vals != NULL, "the stack's vals shouldn't be NULL with a small capacity", &fail_cnt);
    
    hm_stack_free(&stack);
    
    // pass in NULL for stack
    
    hm_stack_init(&stack, capacity, NULL);
    check_res(stack.free == NULL, "the stack's free should be `NULL` when pass in `NULL` to stack", &fail_cnt);
    
    hm_stack_free(&stack);

    print_end("STACK(FIXED) | FUNC | INIT | CAPACITY: 64", fail_cnt);

    HM_TEST_COUNTER

}

void test_stack_dynamic_init() {
    // fixed-size
    int fail_cnt = 0;
    print_run("STACK(DYNAMIC) | FUNC | INIT | CAPACITY: 64");
    hm_stack stack;
    size_t capacity = 64;
    // pass in `free` for stack
    hm_stack_init_dynamic_grow(&stack, capacity, free);
    test_stack_integrity(&stack, &fail_cnt);

    check_res(stack.capacity == capacity, "the stack's capacity isn't equal to expected capacity", &fail_cnt);
    check_res(stack.dynamic_grow == true, "the stack's dynamic-gorwth should is `true`", &fail_cnt);
    check_res(stack.free == free, "the stack's free should be `free` when pass in `free` to stack", &fail_cnt);
    check_res(stack.top == 0, "the stack's top should be 0", &fail_cnt);
    check_res(stack.vals != NULL, "the stack's vals shouldn't be NULL with a small capacity", &fail_cnt);
    
    hm_stack_free(&stack);
    
    // pass in NULL for stack
    
    hm_stack_init_dynamic_grow(&stack, capacity, NULL);
    check_res(stack.free == NULL, "the stack's free should be `NULL` when pass in `NULL` to stack", &fail_cnt);
    
    hm_stack_free(&stack);

    print_end("STACK(DYNAMIC) | FUNC | INIT | CAPACITY: 64", fail_cnt);

    HM_TEST_COUNTER
}

void test_stack_fixed() {
    test_stack_fixed_init();                                        printf("\n");
}


void test_stack_dynamic() {
    test_stack_dynamic_init();                                      printf("\n");
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

