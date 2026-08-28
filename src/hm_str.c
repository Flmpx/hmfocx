/*
 * Copyright (c) 2026 Flmpx
 * Licensed under MIT (see LICENSE).
 */



#include "../include/hm_str.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>



size_t hm_str_len(hm_str* str) {
    assert(str != NULL);

    return str->len;
}

size_t hm_str_capacity(hm_str* str) {
    assert(str != NULL);

    return str->capacity;
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
 * @note - Use the parameter `capacity` to set the start capacity of this str
 * 
 * @return - Return `hm_str_ret_suc` when initialize success
 * @return - Return `hm_str_ret_error` when initialize failure
 */
hm_str_ret hm_str_init_reserve(hm_str* str, size_t capacity) {
    assert(str != NULL);

    memset(str, 0, sizeof(hm_str));

    if (hm_str_fresh(str, capacity) != hm_str_ret_suc) {
        return hm_str_ret_error;
    }
    str->val[0] = '\0';
    return hm_str_ret_suc;
}


/**
 * Initialize str
 */
hm_str_ret hm_str_init(hm_str* str) {
    assert(str != NULL);

    return hm_str_init_reserve(str, 0);
}


/**
 * Append string in str
 * 
 * @note - Use the parameter to set the length of sub_str
 * 
 * @return - Return `hm_str_ret_error` when append fail
 * @return - Return `hm_str_ret_suc` when append success
 * 
 * @warning - Append itself is prohibited
 * @warning - The parameter `sub_str` shouldn't be `NULL`
 */
hm_str_ret hm_str_append(hm_str* str, const char* sub_str, size_t sub_str_len) {
    assert(str != NULL);
    assert(sub_str != NULL);

    // prevent overflow
    if (str->len > SIZE_MAX - sub_str_len) {
        return hm_str_ret_error;
    }

    size_t need = str->len + sub_str_len;
    if (need > str->capacity) {         // the all code block is invalid when need is `0`, so `need` > 0 when the code block is valid

        // prevent overflow
        if (str->capacity > SIZE_MAX / 2) {
            return hm_str_ret_error;
        }

        size_t new_capacity = str->capacity * 2;
        if (need > str->capacity * 2) {         // the `new_cpapcity` isn't be 0 even if the `str->cpacity` is 0
            new_capacity = need;                
        }
        if (hm_str_fresh(str, new_capacity) != hm_str_ret_suc) {
            return hm_str_ret_error;
        }
    }
    memmove(str->val + str->len, sub_str, sub_str_len);
    str->len = need;
    str->val[str->len] = '\0';
    return hm_str_ret_suc;
}


/**
 * Append character in str
 * 
 * @return - Return `hm_str_ret_error` when append fail
 * @return - Return `hm_str_ret_suc` when append success
 */
hm_str_ret hm_str_append_ch(hm_str* str, char ch) {
    assert(str != NULL);

    if (str->len == str->capacity) {
        size_t new_capacity = 0;

        if (str->capacity) {
            // prevent overflow
            if (str->capacity > SIZE_MAX / 2) {
                return hm_str_ret_error;
            }
            new_capacity = str->capacity * 2;
        } else {
            new_capacity = 1;
        }
        
        if (hm_str_fresh(str, new_capacity) != hm_str_ret_suc) {
            return hm_str_ret_error;
        }

    }

    str->val[str->len] = ch;
    str->len++;
    str->val[str->len] = '\0';

    return hm_str_ret_suc;

}

/**
 * Get the str at the specified index
 * 
 * @note - `Index`  must be >= `0`, and <= `the len of str`
 * 
 * @return - Return `NULL` when `index` is out of bounds
 * 
 * @warning - Change the string is prohibited
 */
const char* hm_str_get(hm_str* str, size_t index) {
    assert(str != NULL);

    if (index > str->len) {
        return NULL;
    }

    return (const char*)(str->val + index);
}


/**
 * Pop the string from str
 * 
 * @note - The string will be removed but not free it(Memory Ownership Transfer)
 * @note - Please free this return string after use
 * 
 * @warning - The str can't be used after call this function because the lifetime of str is over
 */
char* hm_str_pop(hm_str* str) {
    assert(str != NULL);

    char* ret_val = str->val;
    memset(str, 0, sizeof(hm_str));

    return ret_val;
}

/**
 * Clear the str
 */
void hm_str_clear(hm_str* str) {
    assert(str != NULL);

    str->len = 0;
    str->val[0] = '\0';
}

/**
 * Free the str
 * 
 * @warning - The str can't be used after call this function because the lifetime of str is over
 */
void hm_str_free(hm_str* str) {
    if (str == NULL) return;

    free(str->val);
    memset(str, 0, sizeof(hm_str));
}

/**
 * Shrink the capacity of str if possible
 * 
 * @note - Shrink to the half of original capacity every call this function
 * 
 * @return - Return `hm_str_ret_suc` when shrink success
 * @return - Return `hm_str_ret_none` when the str can't be shrunk
 * @return - Return `hm_str_ret_error` when shrink failure
 */
hm_str_ret hm_str_shrink(hm_str* str) {
    assert(str != NULL);

    size_t l = str->len, c = str->capacity;
    if (l >= c / 2) {
        return hm_str_ret_none;
    }
    return hm_str_fresh(str, c / 2);
}