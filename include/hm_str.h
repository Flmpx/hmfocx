/*
 * Copyright (c) 2026 Flmpx
 * Licensed under MIT (see LICENSE).
 */

/*********************************************************************
 * Latest API Description see https://github.com/Flmpx/hmfocx/blob/main/docs/hm_str.md
 * 
 ********************************************************************/


#ifndef HM_STR_H
#define HM_STR_H

#include "hm_base.h"

/**
 * The return signal of function in str
 */
typedef enum hm_str_ret {
    hm_str_ret_error = 0x8000,      // Malloc fail when expand the capacity of str or init the str
    hm_str_ret_suc,                 // Operation successful, like push and init successful
    hm_str_ret_none,                // Operation invalid, like shrink capacity of a fixed-size str
    hm_str_ret_warn                 // The pass parameter is incorrect
} hm_str_ret;


/**
 * Str
 */
typedef struct hm_str {
    char* val;
    size_t len;
    size_t capacity;
} hm_str;




/**
 * Small Functions
 */

extern size_t hm_str_len(hm_str* str);
extern size_t hm_str_capacity(hm_str* str);


/**
 * Init
 */

extern hm_str_ret hm_str_init(hm_str* str);
extern hm_str_ret hm_str_init_reserve(hm_str* str, size_t capacity);


/**
 * Append
 */

extern hm_str_ret hm_str_append(hm_str* str, const char* sub_str, size_t sub_str_len);
extern hm_str_ret hm_str_append_ch(hm_str* str, char ch);

/**
 * Get
 */

extern const char* hm_str_get(hm_str* str, size_t index);



/**
 * Shrink
 */

extern hm_str_ret hm_str_shrink(hm_str* str);

/**
 * Pop
 */

extern char* hm_str_pop(hm_str* str);

/**
 * Clear And Free
 */

extern void hm_str_clear(hm_str* str);
extern void hm_str_free(hm_str* str);

#endif