/*
 * Copyright (c) 2026 Flmpx
 * Licensed under MIT (see LICENSE).
 */

/*********************************************************************
 * Latest API Description see https://github.com/Flmpx/hmfocx/blob/main/docs/hm_heap.md
 * 
 ********************************************************************/


#ifndef HM_HEAP_H
#define HM_HEAP_H

#include "hm_base.h"
#include <stdbool.h>

/**
 * The return signal of function in heap
 */
typedef enum hm_heap_ret {
    hm_heap_ret_error = 0x9,       // Malloc fail when expand the capacity of heap or init the heap
    hm_heap_ret_warn,              // The pass parameter is incorrect
    hm_heap_ret_full,              // Heap is full now
    hm_heap_ret_empty,             // Heap is empty now
    hm_heap_ret_suc                // Operation successful, like insert and init successful
} hm_heap_ret;


/**
 * Heap
 */
typedef struct hm_heap {
    void** vals;
    size_t size;
    size_t capacity;
    hm_free free;
    hm_cmp cmp;
    bool dynamic_grow;
} hm_heap;


/**
 * Initialize
 */

extern hm_heap_ret hm_heap_init(hm_heap* heap, size_t capacity, hm_free free, hm_cmp cmp);
extern hm_heap_ret hm_heap_init_dynamic_grow(hm_heap* heap, size_t start_capacity, hm_free free, hm_cmp cmp);


/**
 * Judge
 */

extern bool hm_heap_is_full(hm_heap* heap);
extern bool hm_heap_is_empty(hm_heap* heap);

/**
 * Basic function
 */

extern hm_heap_ret hm_heap_insert(hm_heap* heap, void* val);
extern void* hm_heap_peek(hm_heap* heap);
extern void* hm_heap_extract(hm_heap* heap);


/**
 * Build
 */

extern hm_heap_ret hm_heap_build(hm_heap* heap, void** vals, size_t size, size_t capacity, hm_free free, hm_cmp cmp);
extern hm_heap_ret hm_heap_build_dynamic_grow(hm_heap* heap, void** vals, size_t size, size_t capacity, hm_free free, hm_cmp cmp);
extern void hm_heap_rebuild(hm_heap* heap, hm_cmp new_cmp);



/**
 * Free and Clear
 */

extern void hm_heap_clear(hm_heap* heap);
extern void hm_heap_free(hm_heap* heap);




#endif