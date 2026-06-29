/*
 * Copyright (c) 2026 Flmpx
 * Licensed under MIT (see LICENSE).
 */


#include "../include/hm_stack.h"
#include <stdlib.h>
#include <stdint.h>


hm_stack_ret hm_stack_init(hm_stack* stack, size_t capacity, hm_free free) {
    if (capacity) {
        // prevent overflow
        if (capacity > SIZE_MAX / sizeof(void*)) {
            return hm_stack_ret_error;
        }
        stack->vals = (void**)malloc(capacity * sizeof(void*));
        if (stack->vals == NULL) {
            return hm_stack_ret_error;
        }
    } else {
        stack->vals = NULL;
    }
    stack->dynamic_grow = false;
    stack->capacity = capacity;
    stack->free = free;
    // Because the `size_t` type is a positive number 
    stack->top = 0;
    return hm_stack_ret_suc;    

}

hm_stack_ret hm_stack_init_dynamic_grow(hm_stack* stack, size_t start_capacity, hm_free) {
    hm_stack_ret ret = hm_stack_init(stack, start_capacity, free);
    if (ret == hm_stack_ret_suc) {
        stack->dynamic_grow = true;
    }
    return ret;
}


bool hm_stack_is_full(hm_stack* stack) {
    return !(stack->dynamic_grow) && stack->capacity == stack->top;
}

bool hm_stack_is_empty(hm_stack* stack) {
    return stack->top == 0;
}

hm_stack_ret hm_stack_push(hm_stack* stack, void* val) {
    if (hm_stack_is_full(stack)) {
        return hm_stack_ret_full;
    }

    /**
     * Check if need relloc
     */
    // is the condition is true, indicate the stack is dynamic growth
    if (stack->top == stack->capacity) {
        size_t new_capacity = 0;
        if (stack->capacity) {
            if (stack->capacity > SIZE_MAX / 2) {
                return hm_stack_ret_error;
            }
            // expand to twice the origin size
            new_capacity = stack->capacity * 2;
        } else {
            new_capacity = 1;
        }

        // prevent overflow
        if (new_capacity > SIZE_MAX / sizeof(void*)) {
            return hm_stack_ret_error;
        }

        void** new_vals = (void**)realloc(stack->vals, new_capacity * sizeof(void*));
        if (new_vals == NULL) {
            return hm_stack_ret_error;
        }
        stack->vals = new_vals;
        stack->capacity = new_capacity;
    }

    stack->vals[stack->top++] = val;

    return hm_stack_ret_suc;
    
}

void* hm_stack_peek(hm_stack* stack) {
    if (hm_stack_is_empty(stack)) {
        return NULL;
    }
    return stack->vals[stack->top - 1];
}

void* hm_stack_pop(hm_stack* stack) {
    if (hm_stack_is_empty(stack)) {
        return NULL;
    } 
    return stack->vals[--(stack->top)];
}

void hm_stack_clear(hm_stack* stack) {
    if (stack->free) {
        size_t total = stack->top;
        void** vals = stack->vals;
        for (size_t i = 0; i < stack->top; i++) {
            stack->free(vals[i]);
        }
    }
    stack->top = 0;
}

void hm_stack_free(hm_stack* stack) {
    hm_stack_clear(stack);
    free(stack->vals);
    if (stack->dynamic_grow) {
        hm_stack_init_dynamic_grow(stack, 0, stack->free);
    } else {
        // free no-dynamic growth indicate the stack can't use repetitive
        hm_stack_init(stack, 0, stack->free);
    }
}
