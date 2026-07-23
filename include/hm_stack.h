/*
 * Copyright (c) 2026 Flmpx
 * Licensed under MIT (see LICENSE).
 */


/*********************************************************************
 * Latest API Description see https://github.com/Flmpx/hmfocx/blob/main/docs/hm_stack.md
 * 
 ********************************************************************/

#ifndef HM_STACK_H
#define HM_STACK_H

#include "hm_base.h"
#include <stdbool.h>

/**
 * The return signal of function in stack
 */
typedef enum hm_stack_ret {
    hm_stack_ret_error = 0x9,       // Malloc fail when expand the capacity of stack or init the stack
    hm_stack_ret_full,              // stack is full now
    hm_stack_ret_empty,             // stack is empty now
    hm_stack_ret_suc,               // Operation successful, like push and init successful
    hm_stack_ret_none,              // Operation invalid, like shrink capacity of a fixed-size stack
    hm_stack_ret_warn               // The pass parameter is incorrect
} hm_stack_ret;


/**
 * Stack
 */
typedef struct hm_stack {
    void** vals;
    size_t top;
    size_t capacity;
    hm_free free;
    bool dynamic_grow;
} hm_stack;



/**
 * Simple functions
 */

extern size_t hm_stack_size(hm_stack* stack);
extern size_t hm_stack_capacity(hm_stack* stack);


/**
 * Initialize
 */

extern hm_stack_ret hm_stack_init(hm_stack* stack, size_t capacity, hm_free free);
extern hm_stack_ret hm_stack_init_dynamic_grow(hm_stack* stack, size_t start_capacity, hm_free free);



/**
 * Judge
 */

extern bool hm_stack_is_full(hm_stack* stack);
extern bool hm_stack_is_empty(hm_stack* stack);


/**
 * Basic Operation on stack
 */

extern hm_stack_ret hm_stack_push(hm_stack* stack, void* val);
extern void* hm_stack_peek(hm_stack* stack);
extern void* hm_stack_pop(hm_stack* stack);
extern hm_stack_ret hm_stack_shrink(hm_stack* stack);


/**
 * Free and clear
 */

extern void hm_stack_clear(hm_stack* stack);
extern void hm_stack_free(hm_stack* stack);


#endif