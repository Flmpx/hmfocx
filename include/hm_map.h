/*
 * Copyright (c) 2026 Flmpx
 * Licensed under MIT (see LICENSE).
 */

/*********************************************************************
 * Latest API Description see https://github.com/Flmpx/hmfocx/blob/main/docs/hm_map.md
 * 
 ********************************************************************/


#ifndef HM_MAP_H
#define HM_MAP_H
#include <stdbool.h>
#include "hm_base.h"

/**
 * The return signal of function in map
 */
typedef enum hm_map_ret {
    hm_map_ret_error = 0x4,     // Malloc failed
    hm_map_ret_warn,            // The pass parameter is incorrect
    hm_map_ret_none,            // Operation invalid, like del entry by key , but key is not existed in map
    hm_map_ret_existed,         // Insert entry in map, but key is existed , you should free the key if the key that you given is alloced
    hm_map_ret_suc              // Operation successful
} hm_map_ret;


/**
 * The status of entry in map 
 */
typedef enum hm_map_entry_status {
    hm_exist_in_map,
    hm_del_in_map,
    hm_none_in_map
} hm_map_entry_status;


/**
 * Entry of map
 */
typedef struct hm_map_entry {
    void* key;
    void* val;
} hm_map_entry;



/**
 * The map of using open addressing 
 */
typedef struct hm_map {
    hm_map_entry* buckets;
    hm_map_entry_status* buckets_status;

    size_t size;
    size_t len;

    hm_cmp cmp;
    hm_hash hash;
    hm_free free_key;
    hm_free free_val;
} hm_map;

/**
 * The iterator of map
 */
typedef struct hm_map_iter {
    hm_map_entry* buckets;
    hm_map_entry_status* buckets_status;
    size_t len;
    size_t index;
} hm_map_iter;


/**
 * Some simple functions
 */

extern size_t hm_map_size(hm_map* map);
extern size_t hm_map_len(hm_map* map);
extern double hm_map_get_load_factor(hm_map* map);

/**
 * Some functin of map , like `insert`, `get` , `del` and `free`
 */

extern void hm_map_init(hm_map* map, hm_hash hash_key, hm_cmp cmp_key, hm_free free_key, hm_free free_val);
extern hm_map_ret hm_map_init_reserve(hm_map* map, hm_hash hash_key, hm_cmp cmp_key, hm_free free_key, hm_free free_val, size_t len);
extern hm_map_ret hm_map_insert(hm_map* map, void* key, void* val);
extern hm_map_entry* hm_map_get(hm_map* map, void* key);
extern hm_map_ret hm_map_del(hm_map* map, void* key);
extern hm_map_ret hm_map_shrink(hm_map* map);
extern void hm_map_clear(hm_map* map);
extern void hm_map_free(hm_map* map);


/**
 * Some funtion of iterator of map , like `next` and `has next`
 */

extern void hm_map_iter_init(hm_map_iter* iter, hm_map* map);
extern bool hm_map_iter_has_next(hm_map_iter* iter);
extern hm_map_entry* hm_map_iter_next(hm_map_iter* iter);

#endif