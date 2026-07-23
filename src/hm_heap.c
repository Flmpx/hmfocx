/*
 * Copyright (c) 2026 Flmpx
 * Licensed under MIT (see LICENSE).
 */
#include "../include/hm_heap.h"
#include <stdlib.h>
#include <stdint.h>

size_t hm_heap_size(hm_heap* heap) {
    return heap->size;
}

size_t hm_heap_capacity(hm_heap* heap) {
    return heap->capacity;
}


/**
 * Initialize the heap(fixed-size heap)
 * @note - Use the parameter `capacity` to set the capacity for this heap
 * @note - Return `hm_heap_ret_error` when initialization fails
 * @note - Return `hm_heap_ret_suc` when initialization succeeds
 * @note - If you do `NOT` want the heap to free its values,
 * set the `hm_free` function pointer to `NULL`
 * @note - The `hm_cmp` function pointer `must not be NULL`
 */
hm_heap_ret hm_heap_init(hm_heap* heap, size_t capacity, hm_free free, hm_cmp cmp) {
    if (capacity) {
        // prevent overflow
        if (capacity > SIZE_MAX / sizeof(void*)) {
            return hm_heap_ret_error;
        }
        heap->vals = (void**)malloc(capacity * sizeof(void*));
        if (heap->vals == NULL) {
            return hm_heap_ret_error;
        }
    } else {
        heap->vals = NULL;
    }

    heap->dynamic_grow = false;
    heap->capacity = capacity;
    
    heap->free = free;
    heap->cmp = cmp;

    heap->size = 0;
    
    return hm_heap_ret_suc;
}


/**
 * Initialize the heap(dynamic-growth heap)
 * @note - Use the parameter `capacity` to set the capacity for this heap
 * @note - Return `hm_heap_ret_error` when initialization fails
 * @note - Return `hm_heap_ret_suc` when initialization succeeds
 * @note - If you do `NOT` want the heap to free its values,
 * set the `hm_free` function pointer to `NULL`
 * @note - The `hm_cmp` function pointer `must not be NULL`
 */
hm_heap_ret hm_heap_init_dynamic_grow(hm_heap* heap, size_t start_capacity, hm_free free, hm_cmp cmp) {
    hm_heap_ret ret = hm_heap_init(heap, start_capacity, free, cmp);
    if (ret == hm_heap_ret_suc) {
        heap->dynamic_grow = true;
    }
    return ret;
}


/**
 * Check if the heap is full
 */
bool hm_heap_is_full(hm_heap* heap) {
    return !(heap->dynamic_grow) && heap->size >= heap->capacity;
}

/**
 * Check if the heap is empty
 */
bool hm_heap_is_empty(hm_heap* heap) {
    return heap->size == 0;
}


/**
 * Swap two values
 */
static void hm_swap(void** a, void** b) {
    void* tmp = *a;
    *a = *b;
    *b = tmp;
}

/**
 * Heap Sift Up
 * @note - Return `hm_heap_ret_warn` if kid is out of bounds
 */
static hm_heap_ret hm_heap_sift_up(hm_heap* heap, size_t kid) {
    size_t s = heap->size;
    if (kid >= s) {
        return hm_heap_ret_warn;
    }
    void** vals = heap->vals;
    while (kid > 0) {
        size_t parent = (kid - 1) / 2;
        if (heap->cmp(vals[parent], vals[kid]) <= 0) {
            break;
        } else {
            hm_swap(&vals[parent], &vals[kid]);
            kid = parent;
        }
    }
    return hm_heap_ret_suc;
}



/**
 * Heap Sift Down
 * @note - Return `hm_heap_ret_warn` if parent is out of bounds
 */
static hm_heap_ret hm_heap_sift_down(hm_heap* heap, size_t parent) {
    size_t s = heap->size;
    if (parent >= s) {
        return hm_heap_ret_warn;
    }
    void** vals = heap->vals;
    // parent should have kids
    while (2 * parent + 1 < s) {
        size_t l = 2 * parent + 1;
        size_t r = l + 1;
        size_t min;
        if (r < s) {
            min = heap->cmp(vals[l], vals[r]) < 0 ? l : r;
        } else {
            min = l;
        }
        if (heap->cmp(vals[parent], vals[min]) <= 0) {
            break;
        } else {
            hm_swap(&vals[parent], &vals[min]);
            parent = min;
        }
    }
    return hm_heap_ret_suc;
}


/**
 * Fresh heap with the new capapcity
 * @note - This function return `hm_heap_ret_warn` when size of heap is greater than the `new_capacity`
 */
static hm_heap_ret hm_heap_fresh(hm_heap* heap, size_t new_capacity) {
    if (heap->size > new_capacity) {
        return hm_heap_ret_warn;
    }

    // prevent overflow
    if (new_capacity > SIZE_MAX / sizeof(void*)) {
        return hm_heap_ret_error;
    }

    void** new_vals = (void**)realloc(heap->vals, new_capacity * sizeof(void*));
    if (new_vals == NULL) {
        return hm_heap_ret_error;
    }
    heap->vals = new_vals;
    heap->capacity = new_capacity;

    return hm_heap_ret_suc;
}


/**
 * Insert a value in the heap
 * @note - Return `hm_heap_ret_full` when heap is full
 * @note - Return `hm_heap_ret_suc` when insert succeeds
 * @note - Return `hm_heap_ret_error` when heap is `dynamic-growth` and expansion fails
 */
hm_heap_ret hm_heap_insert(hm_heap* heap, void* val) {
    if (hm_heap_is_full(heap)) {
        return hm_heap_ret_full;
    }

    /**
     * Check if need relloc
     */
    // is the condition is true, indicate the heap is dynamic growth
    if (heap->size == heap->capacity) {
        size_t new_capacity = 0;
        if (heap->capacity) {
            if (heap->capacity > SIZE_MAX / 2) {
                return hm_heap_ret_error;
            }
            // expand to twice the origin size
            new_capacity = heap->capacity * 2;
        } else {
            new_capacity = 1;
        }

        if (hm_heap_fresh(heap, new_capacity) != hm_heap_ret_suc) {
            return hm_heap_ret_error;
        }
    }

    heap->vals[heap->size++] = val;
    hm_heap_sift_up(heap, heap->size - 1);
    return hm_heap_ret_suc;
}

/**
 * Peek a value
 * @note - Return `NULL` when the heap is empty 
 */
void* hm_heap_peek(hm_heap* heap) {
    if (hm_heap_is_empty(heap)) {
        return NULL;
    }
    return heap->vals[0];
}

/**
 * Extract a value 
 * @note - Return `NULL` when the heap is empty
 */
void* hm_heap_extract(hm_heap* heap) {
    if (hm_heap_is_empty(heap)) {
        return NULL;
    }
    void* val = heap->vals[0];
    heap->vals[0] = heap->vals[--heap->size];
    // it will return warn when size == 0, but it can be ignored
    hm_heap_sift_down(heap, 0);
    return val;
}

/**
 * Build a heap(fixed-size heap) by the pass-in vals and some parameter
 * @warning - vals should be located in `heap memory` of system
 * @note - Return `hm_heap_ret_warn` when `size > capacity`
 */
hm_heap_ret hm_heap_build(hm_heap* heap, void** vals, size_t size, size_t capacity, hm_free free, hm_cmp cmp) {
    if (size > capacity) {
        return hm_heap_ret_warn;
    }

    heap->dynamic_grow = false;

    heap->capacity = capacity;
    heap->size = size;

    heap->cmp = cmp;
    heap->free = free;

    heap->vals = vals;
    
    if (heap->size > 1) {

        for (size_t i = 0; i <= ((heap->size - 1) - 1) / 2; i++) {
            size_t parent = ((heap->size - 1) - 1) / 2 - i;
            hm_heap_sift_down(heap, parent);
        }

    }

    return hm_heap_ret_suc;
}


/**
 * Build a heap(dynamic-growth heap) by the pass-in vals and some parameter
 * @warning - vals should be located in `heap memory` of system
 * @note - Return `hm_heap_ret_warn` when `size > capacity`
 */
hm_heap_ret hm_heap_build_dynamic_grow(hm_heap* heap, void** vals, size_t size, size_t capacity, hm_free free, hm_cmp cmp) {
    
    hm_heap_ret ret = hm_heap_build(heap, vals, size, capacity, free, cmp);
    if (ret == hm_heap_ret_suc) {
        heap->dynamic_grow = true;
    }

    return ret;
}


/**
 * Rebuild heap by the new cmp function
 */
void hm_heap_rebuild(hm_heap* heap, hm_cmp new_cmp) {
    heap->cmp = new_cmp;
    if (heap->size > 1) {
        for (size_t i = 0; i <= ((heap->size - 1) - 1) / 2; i++) {
            size_t parent = ((heap->size - 1) - 1) / 2 - i;
            hm_heap_sift_down(heap, parent);
        }
    }
}

/**
 * Shrink the capacity of heap if possible
 * @note - Only dynamic-grow heap have a chance to shrink
 * @note - Return `hm_heap_ret_none` if the heap can't be shrunk
 */
hm_heap_ret hm_heap_shrink(hm_heap* heap) {
    if (!heap->dynamic_grow || heap->size >= heap->capacity / 2) {
        return hm_heap_ret_none;
    }

    size_t new_capacity = heap->capacity / 2;

    return hm_heap_fresh(heap, new_capacity);
}

/**
 * Clear the heap 
 * @note - Only free the values(if possible),  but keep the vals array existed
 */
void hm_heap_clear(hm_heap* heap) {
    hm_free free = heap->free;
    if (free) {
        size_t total = heap->size;
        void** vals = heap->vals;
        for (size_t i = 0; i < total; i++) {
            free(vals[i]);
        }
    }
    heap->size = 0;
}

/**
 * Free all contents of the heap
 * @note - The heap can be reused when it is `dynamic-growth` but `fixed-size` cannot
 */
void hm_heap_free(hm_heap* heap) {
    hm_heap_clear(heap);
    free(heap->vals);
    heap->vals = NULL;
    heap->capacity = 0;
}