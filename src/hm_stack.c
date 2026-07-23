/*
 * Copyright (c) 2026 Flmpx
 * Licensed under MIT (see LICENSE).
 */


#include "../include/hm_stack.h"
#include <stdlib.h>
#include <stdint.h>

size_t hm_stack_size(hm_stack* stack) {
    return stack->top;
}

size_t hm_stack_capacity(hm_stack* stack) {
    return stack->capacity;
}



/**
 * Initialize the stack(fixed-size stack)
 * @note - Use the parameter `capacity` to set the size for this stack
 * @note - Return `hm_stack_ret_error` when initialization fails
 * @note - Return `hm_stack_ret_suc` when initialization succeeds
 * @note - If you do `NOT` want the stack to free its values,
 * set the `hm_free` function pointer to `NULL`
 */
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


/**
 * Initialize the stack(dynamic-growth stack)
 * @note - Use the parameter `start_capacity` to set the start size for this stack
 * @note - Return `hm_stack_ret_error` when initialization fails
 * @note - Return `hm_stack_ret_suc` when initialization succeeds
 * @note - If you do `NOT` want the stack to free its values,
 * set the `hm_free` function pointer to `NULL`
 */
hm_stack_ret hm_stack_init_dynamic_grow(hm_stack* stack, size_t start_capacity, hm_free free) {
    hm_stack_ret ret = hm_stack_init(stack, start_capacity, free);
    if (ret == hm_stack_ret_suc) {
        stack->dynamic_grow = true;
    }
    return ret;
}


/**
 * Check if the stack is full
 */
bool hm_stack_is_full(hm_stack* stack) {
    return !(stack->dynamic_grow) && stack->top >= stack->capacity;
}

/**
 * Check if the stack is empty
 */
bool hm_stack_is_empty(hm_stack* stack) {
    return stack->top == 0;
}

/**
 * Fresh stack with the new capapcity
 * @note - This function return `hm_stack_ret_warn` when size of stack is greater than the `new_capacity`
 */
static hm_stack_ret hm_stack_fresh(hm_stack* stack, size_t new_capacity) {
    if (stack->top > new_capacity) {
        return hm_stack_ret_warn;
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

    return hm_stack_ret_suc;
}

/**
 * Push a value to the stack
 * @note - Return `hm_stack_ret_full` when stack is full
 * @note - Return `hm_stack_ret_suc` when push succeeds
 * @note - Return `hm_stack_ret_error` when stack is `dynamic-growth` and expansion fails
 */
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

        if (hm_stack_fresh(stack, new_capacity) != hm_stack_ret_suc) {
            return hm_stack_ret_error;
        }
        
    }

    stack->vals[stack->top++] = val;

    return hm_stack_ret_suc;
    
}


/**
 * Peek a value from the stack
 * @note - Return `NULL` when the stack is empty 
 */
void* hm_stack_peek(hm_stack* stack) {
    if (hm_stack_is_empty(stack)) {
        return NULL;
    }
    return stack->vals[stack->top - 1];
}

/**
 * Pop a value from the stack
 * @note - Return `NULL` when the stack is empty
 */
void* hm_stack_pop(hm_stack* stack) {
    if (hm_stack_is_empty(stack)) {
        return NULL;
    } 
    return stack->vals[--(stack->top)];
}


/**
 * Shrink the capacity of stack if possible
 * @note - Only dynamic-grow stack have a chance to shrink
 * @note - Returns `hm_stack_ret_none` if the stack can't be shrunk
 */
hm_stack_ret hm_stack_shrink(hm_stack* stack) {
    if (!stack->dynamic_grow || stack->top >= stack->capacity / 2) {
        return hm_stack_ret_none;
    }
    size_t new_capacity = stack->capacity / 2;

    return hm_stack_fresh(stack, new_capacity);

}


/**
 * Clear the stack 
 * @note - Only free the values(if possible),  but keep the vals array existed
 */
void hm_stack_clear(hm_stack* stack) {
    if (stack->free) {
        size_t total = stack->top;
        void** vals = stack->vals;
        for (size_t i = 0; i < total; i++) {
            stack->free(vals[i]);
        }
    }
    stack->top = 0;
}

/**
 * Free all contents of the stack
 * @note - The stack can be reused when it is `dynamic-growth` but `fixed-size` cannot
 */
void hm_stack_free(hm_stack* stack) {
    hm_stack_clear(stack);
    free(stack->vals);
    stack->vals = NULL;
    stack->capacity = 0;
}
