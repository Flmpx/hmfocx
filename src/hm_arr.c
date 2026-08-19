/*
 * Copyright (c) 2026 Flmpx
 * Licensed under MIT (see LICENSE).
 */


#include "../include/hm_arr.h"
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

size_t hm_arr_size(hm_arr* arr) {
    assert(arr != NULL);

    return arr->size;
}

size_t hm_arr_capacity(hm_arr* arr) {
    assert(arr != NULL);

    return arr->capacity;
}

/**
 * Initialize the arr(fixed-size arr)
 * 
 * @note - Use the parameter `capacity` to set the capacity for this arr
 * @note - If you do `NOT` want the arr to free its values, set the `free_val` function pointer to `NULL`
 * 
 * @return - Return `hm_arr_ret_error` when initialize failure
 * @return - Return `hm_arr_ret_suc` when initialize success
 */
hm_arr_ret hm_arr_init(hm_arr* arr, size_t capacity, hm_free free_val) {
    assert(arr != NULL);

    if (capacity) {
        // prevent overflow
        if (capacity > SIZE_MAX / sizeof(void*)) {
            return hm_arr_ret_error;
        }
        arr->vals = (void**)malloc(capacity * sizeof(void*));
        if (arr->vals == NULL) {
            return hm_arr_ret_error;
        }
    } else {
        arr->vals = NULL;
    }
    arr->dynamic_grow = false;
    arr->capacity = capacity;
    arr->free_val = free_val;
    
    arr->size = 0;
    return hm_arr_ret_suc;    

}

/**
 * Initialize the arr(dynamic-grow arr)
 * 
 * @note - Use the parameter `start_capacity` to set the start capacity for this arr
 * @note - If you do `NOT` want the arr to free its values, set the `free_val` function pointer to `NULL`
 * 
 * @return - Return `hm_arr_ret_error` when initialize failure
 * @return - Return `hm_arr_ret_suc` when initialize success
 */
hm_arr_ret hm_arr_init_dynamic_grow(hm_arr* arr, size_t start_capacity, hm_free free_val) {
    assert(arr != NULL);

    hm_arr_ret ret = hm_arr_init(arr, start_capacity, free_val);
    if (ret == hm_arr_ret_suc) {
        arr->dynamic_grow = true;
    }
    return ret;
}


/**
 * Check if the arr is full
 */
bool hm_arr_is_full(hm_arr* arr) {
    assert(arr != NULL);

    return !(arr->dynamic_grow) && arr->size >= arr->capacity;
}

/**
 * Check if the arr is empty
 */
bool hm_arr_is_empty(hm_arr* arr) {
    assert(arr != NULL);

    return arr->size == 0;
}

/**
 * Fresh arr with the new capapcity
 * 
 * @note - This function is used to change the capacity of arr
 * 
 * @return - Return `hm_arr_ret_warn` when `size of arr` > `new_capacity`
 * @return - Return `hm_arr_ret_error` when malloc failure
 * @return - Return `hm_arr_ret_suc` when fresh arr success
 */
static hm_arr_ret hm_arr_fresh(hm_arr* arr, size_t new_capacity) {
    assert(arr != NULL);

    if (arr->size > new_capacity) {
        return hm_arr_ret_warn;
    }

    // prevent overflow
    if (new_capacity > SIZE_MAX / sizeof(void*)) {
        return hm_arr_ret_error;
    }

    void** new_vals = (void**)realloc(arr->vals, new_capacity * sizeof(void*));
    if (new_vals == NULL) {
        return hm_arr_ret_error;
    }
    arr->vals = new_vals;
    arr->capacity = new_capacity;

    return hm_arr_ret_suc;
}



/**
 * Insert a value at the given `index`
 * 
 * @note - `Index` must be >= `0`, and <= `the size of arr`
 * 
 * @return - Return `hm_arr_ret_warn` when the `index` is out of bounds
 * @return - Return `hm_arr_ret_full` when arr is full
 * @return - Return `hm_arr_ret_suc` when insert success
 * @return - Return `hm_arr_ret_error` when arr is `dynamic-grow` and expand failure
 */
hm_arr_ret hm_arr_insert_index(hm_arr* arr, void* val, size_t index) {
    assert(arr != NULL);

    if (index > arr->size) {
        return hm_arr_ret_warn;
    }

    if (hm_arr_is_full(arr)) {
        return hm_arr_ret_full;
    }

    /**
     * Check if need relloc
     */
    // is the condition is true, indicate the arr is dynamic growth
    if (arr->size == arr->capacity) {
        size_t new_capacity = 0;
        if (arr->capacity) {
            if (arr->capacity > SIZE_MAX / 2) {
                return hm_arr_ret_error;
            }
            // expand to twice the origin size
            new_capacity = arr->capacity * 2;
        } else {
            new_capacity = 1;
        }

        if (hm_arr_fresh(arr, new_capacity) != hm_arr_ret_suc) {
            return hm_arr_ret_error;
        }
        
    }

    for (size_t i = arr->size; i > index; i--) {
        arr->vals[i] = arr->vals[i - 1];
    }
    arr->vals[index] = val;
    arr->size++;
    return hm_arr_ret_suc;
}

/**
 * Insert a value at the head of the arr
 * 
 * @return - Return `hm_arr_ret_full` when arr is full
 * @return - Return `hm_arr_ret_suc` when insert success
 * @return - Return `hm_arr_ret_error` when arr is `dynamic-grow` and expand failure
 */
hm_arr_ret hm_arr_insert_head(hm_arr* arr, void* val) {
    assert(arr != NULL);

    if (hm_arr_is_full(arr)) {
        return hm_arr_ret_full;
    }
    return hm_arr_insert_index(arr, val, 0);
}


/**
 * Insert a value at the tail of the arr
 * 
 * @return - Return `hm_arr_ret_full` when arr is full
 * @return - Return `hm_arr_ret_suc` when insert success
 * @return - Return `hm_arr_ret_error` when arr is `dynamic-grow` and expand failure
 */
hm_arr_ret hm_arr_insert_tail(hm_arr* arr, void* val) {
    assert(arr != NULL);

    if (hm_arr_is_full(arr)) {
        return hm_arr_ret_full;
    }
    return hm_arr_insert_index(arr, val, arr->size);
}



/**
 * Delete the val at the specified `index`
 * 
 * @note - `Index` must be >= `0`, and < `the size of arr`
 * 
 * @return - Return `hm_arr_ret_none` when the `index` is out of bounds or arr is empty
 * @return - Return `hm_arr_ret_suc` when delete success
 */
hm_arr_ret hm_arr_del_index(hm_arr* arr, size_t index) {
    assert(arr != NULL);

    if (index >= arr->size || hm_arr_is_empty(arr)) {
        return hm_arr_ret_none;
    }
    arr->free_val(arr->vals[index]);
    for (size_t i = index; i < arr->size - 1; i++) {
        arr->vals[i] = arr->vals[i + 1];
    }
    arr->size--;
    return hm_arr_ret_suc;
}

/**
 * Delete the val at the head of the arr
 * 
 * @return - Return `hm_arr_ret_none` when arr is empty
 * @return - Return `hm_arr_ret_suc` when delete success
 */
hm_arr_ret hm_arr_del_head(hm_arr* arr) {
    assert(arr != NULL);

    if (hm_arr_is_empty(arr)) {
        return hm_arr_ret_none;
    }
    return hm_arr_del_index(arr, 0);
}



/**
 * Delete the val at the tail of the arr
 * 
 * @return - Return `hm_arr_ret_none` when arr is empty
 * @return - Return `hm_arr_ret_suc` when delete success
 */
hm_arr_ret hm_arr_del_tail(hm_arr* arr) {
    assert(arr != NULL);

    if (hm_arr_is_empty(arr)) {
        return hm_arr_ret_none;
    }
    return hm_arr_del_index(arr, arr->size - 1);
}

/**
 * Get a pointer to the pointer of value at the specified `index`
 * 
 * @note - Use the pointer can change `the pointer to the value`
 * @note - `Index` must be >= `0`, and < `the size of arr`
 * 
 * @return - Return `NULL` when the `index` is out of bounds
 */
void** hm_arr_get_pointer(hm_arr* arr, size_t index) {
    assert(arr != NULL);

    if (index >= arr->size || hm_arr_is_empty(arr)) {
        return NULL;
    }
    return arr->vals + index;
}

/**
 * Get a pointer to the value at the specified `index`
 * 
 * @note - `Index` must be >= `0`, and < `the size of arr`
 * 
 * @return - Return `NULL` when the `index` is out of bounds
 */
void* hm_arr_get(hm_arr* arr, size_t index) {
    assert(arr != NULL);

    if (index >= arr->size || hm_arr_is_empty(arr)) {
        return NULL;
    }
    return arr->vals[index];
}

/**
 * Pop the val at the specified `index`
 * 
 * @note - The val will be removed but not free its memory(Memory Ownership Transfer)
 * @note - `Index` must be >= `0`, and < `the size of arr`
 * 
 * @return - Return `NULL` when the `index` is out of bounds
 */
void* hm_arr_pop(hm_arr* arr, size_t index) {
    assert(arr != NULL);

    if (index >= arr->size || hm_arr_is_empty(arr)) {
        return NULL;
    }
    void* ret_val = arr->vals[index];
    for (size_t i = index; i < arr->size - 1; i++) {
        arr->vals[i] = arr->vals[i + 1];
    }
    arr->size--;
    return ret_val;
}

/**
 * Shrink the capacity of arr if possible
 * 
 * @note - Only dynamic-grow arr have a chance to shrink
 * 
 * @return - Return `hm_arr_ret_suc` when shrink success
 * @return - Return `hm_arr_ret_none` when the arr can't be shrunk
 * @return - Return `hm_arr_ret_error` when shrink failure
 */
hm_arr_ret hm_arr_shrink(hm_arr* arr) {
    assert(arr != NULL);

    if (!arr->dynamic_grow || arr->size >= arr->capacity / 2) {
        return hm_arr_ret_none;
    }
    size_t new_capacity = arr->capacity / 2;

    return hm_arr_fresh(arr, new_capacity);

}


/**
 * Clear the arr 
 * 
 * @note - Only free the values(if possible),  but keep the vals array existed
 */
void hm_arr_clear(hm_arr* arr) {
    assert(arr != NULL);
    
    if (arr->free_val) {
        size_t total = arr->size;
        void** vals = arr->vals;
        for (size_t i = 0; i < total; i++) {
            arr->free_val(vals[i]);
        }
    }
    arr->size = 0;
}

/**
 * Free all contents of the arr
 * 
 * @note - The arr can be reused when it is `dynamic-grow` but `fixed-size` cannot
 */
void hm_arr_free(hm_arr* arr) {
    if (arr == NULL) return;
    hm_arr_clear(arr);
    free(arr->vals);
    arr->vals = NULL;
    arr->capacity = 0;
}