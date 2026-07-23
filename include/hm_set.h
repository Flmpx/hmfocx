/*
 * Copyright (c) 2026 Flmpx
 * Licensed under MIT (see LICENSE).
 */

/*********************************************************************
 * Latest API Description see https://github.com/Flmpx/hmfocx/blob/main/docs/hm_set.md
 * 
 ********************************************************************/

#ifndef HM_SET_H
#define HM_SET_H
#include <stdbool.h>
#include "hm_base.h"

/**
 * The return signal of function in set
 */
typedef enum hm_set_ret {
    hm_set_ret_error = 0x16,     // Malloc failed
    hm_set_ret_warn,            // The pass parameter is incorrect
    hm_set_ret_none,            // Operation invalid, like del entry by key , but key is not existed in set
    hm_set_ret_existed,         // Insert entry in set, but key is existed , you should free the key if the key that you given is alloced
    hm_set_ret_suc              // Operation successful
} hm_set_ret;



/**
 * The status of entry in set 
 */
typedef enum hm_set_entry_status {
    hm_exist_in_set,
    hm_del_in_set,
    hm_none_in_set
} hm_set_entry_status;



/**
 * Entry of set
 */
typedef struct hm_set_entry {
    void* key;
} hm_set_entry;


/**
 * The set of using open addressing 
 */
typedef struct hm_set {
    hm_set_entry* buckets;
    hm_set_entry_status* buckets_status;

    size_t size;
    size_t len;

    hm_cmp cmp;
    hm_hash hash;
    hm_free free_key;
} hm_set;


/**
 * The iterator of set
 */
typedef struct hm_set_iter {
    hm_set_entry* buckets;
    hm_set_entry_status* buckets_status;
    size_t len;
    size_t index;
} hm_set_iter;


/**
 * Some simple functions
 */

extern size_t hm_set_size(hm_set* set);
extern size_t hm_set_len(hm_set* set);
extern double hm_set_get_load_factor(hm_set* set);


/**
 * Some functin of set , like `insert`, `get` , `del` and `free`
 */

extern void hm_set_init(hm_set* set, hm_hash hash_key, hm_cmp cmp_key, hm_free free_key);
extern hm_set_ret hm_set_init_reserve(hm_set* set, hm_hash hash_key, hm_cmp cmp_key, hm_free free_key, size_t len);
extern hm_set_ret hm_set_insert(hm_set* set, void* key);
extern hm_set_entry* hm_set_get(hm_set* set, void* key);
extern hm_set_ret hm_set_del(hm_set* set, void* key);
extern hm_set_ret hm_set_shrink(hm_set* set);
extern void hm_set_clear(hm_set* set);
extern void hm_set_free(hm_set* set);


/**
 * Some funtion of iterator of set , like `next` and `has next`
 */

extern void hm_set_iter_init(hm_set_iter* iter, hm_set* set);
extern bool hm_set_iter_has_next(hm_set_iter* iter);
extern hm_set_entry* hm_set_iter_next(hm_set_iter* iter);



#endif