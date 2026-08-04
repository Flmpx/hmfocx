/*
 * Copyright (c) 2026 Flmpx
 * Licensed under MIT (see LICENSE).
 */

/*********************************************************************
 * Latest API Description see https://github.com/Flmpx/hmfocx/blob/main/docs/hm_arr.md
 * 
 ********************************************************************/


#ifndef HM_ARR_H
#define HM_ARR_H

#include "hm_base.h"
#include <stdbool.h>

/**
 * The return signal of function in arr
 */
typedef enum hm_arr_ret {
    hm_arr_ret_error = 0x7000,       // Malloc fail when expand the capacity of arr or init the arr
    hm_arr_ret_full,              // arr is full now
    hm_arr_ret_empty,             // arr is empty now
    hm_arr_ret_suc,               // Operation successful, like push and init successful
    hm_arr_ret_none,              // Operation invalid, like shrink capacity of a fixed-size arr
    hm_arr_ret_warn               // The pass parameter is incorrect
} hm_arr_ret;


/**
 * Arr
 */
typedef struct hm_arr {
    void** vals;
    size_t size;
    size_t capacity;
    hm_free free_val;
    bool dynamic_grow;
} hm_arr;


/**
 * Small Functions
 */

extern size_t hm_arr_size(hm_arr* arr);
extern size_t hm_arr_capacity(hm_arr* arr);


/**
 * Initialize
 */

extern hm_arr_ret hm_arr_init(hm_arr* arr, size_t capacity, hm_free free_val);
extern hm_arr_ret hm_arr_init_dynamic_grow(hm_arr* arr, size_t start_capacity, hm_free free_val);


/**
 * Judge
 */

extern bool hm_arr_is_full(hm_arr* arr);
extern bool hm_arr_is_empty(hm_arr* arr);

/**
 * Insert
 */

extern hm_arr_ret hm_arr_insert_head(hm_arr* arr, void* val);
extern hm_arr_ret hm_arr_insert_tail(hm_arr* arr, void* val);
extern hm_arr_ret hm_arr_insert_index(hm_arr* arr, void* val, size_t index);

/**
 * Delete
 */

extern hm_arr_ret hm_arr_del_head(hm_arr* arr);
extern hm_arr_ret hm_arr_del_tail(hm_arr* arr);
extern hm_arr_ret hm_arr_del_index(hm_arr* arr, size_t index);


/**
 * Get
 */

extern void* hm_arr_get(hm_arr* arr, size_t index);
extern void** hm_arr_get_pointer(hm_arr* arr, size_t index);


/**
 * Pop
 */

extern void* hm_arr_pop(hm_arr* arr, size_t index);


/**
 * Shrink
 */

extern hm_arr_ret hm_arr_shrink(hm_arr* arr);


/**
 * Clear And Free
 */

extern void hm_arr_clear(hm_arr* arr);
extern void hm_arr_free(hm_arr* arr);


#endif