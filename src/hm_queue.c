/*
 * Copyright (c) 2026 Flmpx
 * Licensed under MIT (see LICENSE).
 */

#include "../include/hm_queue.h"
#include <stdlib.h>
#include <stdint.h>

size_t hm_queue_size(hm_queue* queue) {
    return queue->size;
}

size_t hm_queue_capacity(hm_queue* queue) {
    return queue->capacity;
}


/**
 * Initialize the queue(fixed-size queue)
 * @note - Use the parameter `capacity` to set the size for this queue
 * @note - Return `hm_queue_ret_error` when initialization fails
 * @note - Return `hm_queue_ret_suc` when initialization succeeds
 * @note - If you do `NOT` want the queue to free its values,
 * set the `hm_free` function pointer to `NULL`
 */
hm_queue_ret hm_queue_init(hm_queue* queue, size_t capacity, hm_free free) {
    if (capacity) {
        // prevent overflow
        if (capacity > SIZE_MAX / sizeof(void*)) {
            return hm_queue_ret_error;
        }
        queue->vals = (void**)malloc(capacity * sizeof(void*));
        if (queue->vals == NULL) {
            return hm_queue_ret_error;
        }
    } else {
        queue->vals = NULL;
    }

    queue->dynamic_grow = false;
    queue->capacity = capacity;
    queue->free = free;

    queue->front = queue->rear = 0;
    queue->size = 0;

    return hm_queue_ret_suc;
}


/**
 * Initialize the queue(dynamic-growth queue)
 * @note - Use the parameter `start_capacity` to set the start size for this queue
 * @note - Return `hm_queue_ret_error` when initialization fails
 * @note - Return `hm_queue_ret_suc` when initialization succeeds
 * @note - If you do `NOT` want the queue to free its values,
 * set the `hm_free` function pointer to `NULL`
 */
hm_queue_ret hm_queue_init_dynamic_grow(hm_queue* queue, size_t start_capacity, hm_free free) {
    hm_queue_ret ret = hm_queue_init(queue, start_capacity, free);
    if (ret == hm_queue_ret_suc) {
        queue->dynamic_grow = true;
    }
    return ret;
}


/**
 * Check if the queue is full
 */
bool hm_queue_is_full(hm_queue* queue) {
    return !(queue->dynamic_grow) && queue->size >= queue->capacity;
}



/**
 * Check if the queue is empty
 */
bool hm_queue_is_empty(hm_queue* queue) {
    return queue->size == 0;
}


/**
 * Enqueue a value
 * @note - Return `hm_queue_ret_full` when queue is full
 * @note - Return `hm_queue_ret_suc` when enqueue succeeds
 * @note - Return `hm_queue_ret_error` when queue is `dynamic-growth` and expansion fails
 */
hm_queue_ret hm_queue_enq(hm_queue* queue, void* val) {
    if (hm_queue_is_full(queue)) {
        return hm_queue_ret_full;
    }

    /**
     * Check if need relloc
     */
    // is the condition is true, indicate the queue is dynamic growth
    if (queue->size == queue->capacity) {
        size_t new_capacity = 0;
        if (queue->capacity) {
            if (queue->capacity > SIZE_MAX / 2) {
                return hm_queue_ret_error;
            }
            // expand to twice the origin size
            new_capacity = queue->capacity * 2;
        } else {
            new_capacity = 1;
        }

        // prevent overflow
        if (new_capacity > SIZE_MAX / sizeof(void*)) {
            return hm_queue_ret_error;
        }

        void** new_vals = (void**)malloc(sizeof(void*) * new_capacity);

        if (new_vals == NULL) {
            return hm_queue_ret_error;
        }

        for (size_t i = 0; i < queue->size; i++) {
            size_t pos = (i + queue->front) % queue->capacity;
            new_vals[i] = queue->vals[pos];
        }

        free(queue->vals);
        queue->vals = new_vals;

        queue->front = 0;
        queue->rear = queue->size;

        queue->capacity = new_capacity;
        
    }

    queue->vals[queue->rear] = val;
    queue->rear = (queue->rear + 1) % queue->capacity;
    queue->size++;

    return hm_queue_ret_suc;

}

/**
 * Peek a value
 * @note - Return `NULL` when the queue is empty 
 */
void* hm_queue_peek(hm_queue* queue) {
    if (hm_queue_is_empty(queue)) {
        return NULL;
    }
    return queue->vals[queue->front];
}

/**
 * Dequeue a value
 * @note - Return `NULL` when the queue is empty
 */
void* hm_queue_deq(hm_queue* queue) {
    if (hm_queue_is_empty(queue)) {
        return NULL;
    }
    void* val = queue->vals[queue->front];
    queue->front = (queue->front + 1) % queue->capacity;
    queue->size--;

    return val;
}



/**
 * Clear the queue 
 * @note - Only free the values(if possible),  but keep the vals array existed
 */
void hm_queue_clear(hm_queue* queue) {
    if (queue->free) {
        size_t size = queue->size;
        void** vals = queue->vals;
        for (size_t i = 0; i < size; i++) {
            size_t pos = (queue->front + i) % queue->capacity;
            queue->free(vals[pos]);
        }
    }
    queue->front = queue->rear = 0;
    queue->size = 0;
}



/**
 * Free all contents of the queue
 * @note - The queue can be reused when it is `dynamic-growth` but `fixed-size` cannot
 */
void hm_queue_free(hm_queue* queue) {
    hm_queue_clear(queue);
    free(queue->vals);
    queue->vals = NULL;
    queue->capacity = 0;
}

