/*
 * Copyright (c) 2026 Flmpx
 * Licensed under MIT (see LICENSE).
 */

/*********************************************************************
 * Latest API Description see https://github.com/Flmpx/hmfocx/blob/main/docs/hm_queue.md
 * 
 ********************************************************************/

#ifndef HM_QUEUE_H
#define HM_QUEUE_H

#include "hm_base.h"
#include <stdbool.h>


/**
 * The return signal of function in queue
 */
typedef enum hm_queue_ret {
    hm_queue_ret_error = 0xd,       // Malloc fail when expand the capacity of queue or init the queue
    hm_queue_ret_full,              // Queue is full now
    hm_queue_ret_empty,             // Queue is empty now
    hm_queue_ret_suc,               // Operation successful, like push and init successful
    hm_queue_ret_none,              // Operation invalid, like shrink capacity of a fixed-size queue
    hm_queue_ret_warn               // The pass parameter is incorrect
} hm_queue_ret;


/**
 * Queue
 */
typedef struct hm_queue {
    void** vals;
    size_t front;
    size_t rear;
    size_t capacity;
    size_t size;
    hm_free free;
    bool dynamic_grow;
} hm_queue;



/**
 * Simple functions
 */

extern size_t hm_queue_size(hm_queue* queue);
extern size_t hm_queue_capacity(hm_queue* queue);



/**
 * Initialize
 */

extern hm_queue_ret hm_queue_init(hm_queue* queue, size_t capacity, hm_free free);
extern hm_queue_ret hm_queue_init_dynamic_grow(hm_queue* queue, size_t start_capacity, hm_free free);


/**
 * Judge
 */

extern bool hm_queue_is_full(hm_queue* queue);
extern bool hm_queue_is_empty(hm_queue* queue);


/**
 * Basic Operation on queue
 */

extern hm_queue_ret hm_queue_enq(hm_queue* queue, void* val);
extern void* hm_queue_peek(hm_queue* queue);
extern void* hm_queue_deq(hm_queue* queue);
extern hm_queue_ret hm_queue_shrink(hm_queue* queue);

/**
 * Free and clear
 */

extern void hm_queue_clear(hm_queue* queue);
extern void hm_queue_free(hm_queue* queue);





#endif