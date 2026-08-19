/*
 * Copyright (c) 2026 Flmpx
 * Licensed under MIT (see LICENSE).
 */



#include "../include/hm_str.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

/**
 * `min_capacity` represents the minimal capacity of str
 */
static const size_t min_capacity = 17;



size_t hm_str_len(hm_str* str) {
    assert(str != NULL);

    return str->len;
}

size_t hm_str_capacity(hm_str* str) {
    assert(str != NULL);

    return str->capacity;
}

/**
 * Initialize str
 */
void hm_str_init(hm_str* str) {
    assert(str != NULL);

    *str = (hm_str){
        .capacity = 0,
        .len = 0,
        .val = NULL
    };
}


/**
 * Fresh str with the new capacity
 * 
 * @note - This function is used to change the capacity of str
 * 
 * @return - Return `hm_str_ret_warn` when `len of str` > `new_capacity`
 * @return - Return `hm_str_ret_error` when malloc failure
 * @return - Return `hm_str_ret_suc` when fresh str success
 */
static hm_str_ret hm_str_fresh(hm_str* str, size_t new_capacity) {
    assert(str != NULL);

    if (str->len > new_capacity) {
        return hm_str_ret_warn;
    }
    if (new_capacity == SIZE_MAX) {
        return hm_str_ret_error;
    }
    char* new_val = (char*)realloc(str->val, new_capacity + 1);
    if (new_val == NULL) {
        return hm_str_ret_error;
    }
    str->val = new_val;
    str->capacity = new_capacity;
    return hm_str_ret_suc;
}


/**
 * Reserve capacity and initialize str
 * 
 * @note - Use the parameter `capacity` to set the start capacity of this str, the `min_capacity` is 17, `capacity` will be `min_capacity` if `capacity` < `min_capacity`
 * 
 * @return - Return `hm_str_ret_suc` when initialize success
 * @return - Return `hm_str_ret_error` when initialize failure
 */
hm_str_ret hm_str_init_reserve(hm_str* str, size_t capacity) {
    assert(str != NULL);

    hm_str_init(str);
    if (hm_str_fresh(str, (capacity > min_capacity) ? capacity : min_capacity) != hm_str_ret_suc) {
        return hm_str_ret_error;
    }
    str->val[0] = '\0';
    return hm_str_ret_suc;
}



/**
 * Append string in str
 * 
 * @return - Return `hm_str_ret_error` when append fail
 * @return - Return `hm_str_ret_suc` when append success
 * 
 * @warning - Append itself is prohibited
 * @warning - The parameter `sub_str` shouldn't be `NULL`
 */
hm_str_ret hm_str_append(hm_str* str, const char* sub_str) {
    assert(str != NULL);
    assert(sub_str != NULL);

    if (str->capacity == 0) {
        if (hm_str_init_reserve(str, min_capacity) != hm_str_ret_suc) {
            return hm_str_ret_error;
        }
    }
    size_t sub_len = strlen(sub_str);
    // prevent overflow
    if (str->len > SIZE_MAX - sub_len) {
        return hm_str_ret_error;
    }
    size_t need = str->len + sub_len;
    if (need > str->capacity) {
        // prevent overflow
        if (str->capacity > SIZE_MAX / 2) {
            return hm_str_ret_error;
        }
        size_t new_capacity = str->capacity * 2;
        if (need > str->capacity * 2) {
            new_capacity = need;
        }
        if (hm_str_fresh(str, new_capacity) != hm_str_ret_suc) {
            return hm_str_ret_error;
        }
    }
    // prevent sub_str is in str.val
    strcat(str->val + str->len, sub_str);
    str->len = need;
    return hm_str_ret_suc;
}

/**
 * Get the str at the specified index
 * 
 * @note - `Index`  must be >= `0`, and <= `the len of str`
 * 
 * @return - Return `NULL` when the `capacity` of str is `0`
 * @return - Return `NULL` when `index` is out of bounds
 * 
 * @warning - Change the string is prohibited
 */
const char* hm_str_get(hm_str* str, size_t index) {
    assert(str != NULL);

    if (index > str->len || str->val == NULL) {
        // index is invalid or capacity == 0(str->val == NULL)
        return NULL;
    }
    return (const char*)(str->val + index);
}


/**
 * Pop the string from str
 * 
 * @note - The string will be removed but not free it(Memory Ownership Transfer)
 * @note - Please free this string after use
 */
char* hm_str_pop(hm_str* str) {
    assert(str != NULL);

    char* ret_val = str->val;
    hm_str_init(str);
    return ret_val;
}

/**
 * Clear the str
 */
void hm_str_clear(hm_str* str) {
    assert(str != NULL);

    str->len = 0;
    if (str->val) {
        str->val[0] = '\0';
    }
}

/**
 * Free the str
 */
void hm_str_free(hm_str* str) {
    if (str == NULL) return;
    free(str->val);
    hm_str_init(str);
}

/**
 * Shrink the capacity of str if possible
 * 
 * @return - Return `hm_str_ret_suc` when shrink success
 * @return - Return `hm_str_ret_none` when the str can't be shrunk
 * @return - Return `hm_str_ret_error` when shrink failure
 */
hm_str_ret hm_str_shrink(hm_str* str) {
    assert(str != NULL);

    size_t l = str->len, c = str->capacity;
    if (c < min_capacity * 2 || l >= c / 2) {
        return hm_str_ret_none;
    }
    return hm_str_fresh(str, c / 2);
}