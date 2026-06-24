/*
 * Copyright (c) 2026 Flmpx
 * Licensed under MIT (see LICENSE).
 */


#include "../include/hm_stack.h"
#include <stdlib.h>


hm_stack_ret hm_stack_init(hm_stack* stack, size_t capacity, hm_free free) {
    stack->dynamic_grow = false;

}

hm_stack_ret hm_stack_init_dynamic_grow(hm_stack* stack, size_t start_capacity, hm_free) {
    stack->dynamic_grow = true;
}


bool hm_stack_is_full(hm_stack* stack) {
    return !(stack->dynamic_grow) && stack->capacity == stack->top;
}

bool hm_stack_is_empty(hm_stack* stack) {
    return stack->top == 0;
}

hm_stack_ret hm_stack_push(hm_stack* stack, void* val) {

}

void* hm_stack_peek(hm_stack* stack) {
    
}

void* hm_stack_pop(hm_stack* stack) {

}

void hm_stack_clear(hm_stack* stack) {

}

void hm_stack_free(hm_stack* stack) {

}
