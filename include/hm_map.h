#ifndef HM_MAP_H
#define HM_MAP_H
#include <stdbool.h>
#include "hm_base.h"

/**
 * the return signal of function in map
 */
typedef enum hm_map_ret {
    hm_map_ret_error = 0x4,     // malloc filed
    hm_map_ret_warn,            // the pass parameter is incorrect
    hm_map_ret_none,            // operation invalid, like del entry by key , but key is not existed in map
    hm_map_ret_existed,         // insert entry in map, but key is existed , you should free the key if the key that you given is alloced
    hm_map_ret_suc              // operation successful
} hm_map_ret;


/**
 * the status of entry in map 
 */
typedef enum hm_entry_status {
    hm_exist_in_map,
    hm_del_in_map,
    hm_none_in_map
} hm_entry_status;


/**
 * entry of map
 */
typedef struct hm_entry {
    void* key;
    void* val;
} hm_entry;



/**
 * the map of using open addressing 
 */
typedef struct hm_map {
    hm_entry* buckets;
    hm_entry_status* buckets_status;

    size_t size;
    size_t len;

    hm_cmp cmp;
    hm_hash hash;
    hm_free free_key;
    hm_free free_val;
} hm_map;

/**
 * the iterator of map
 */
typedef struct hm_iter_map {
    hm_entry* buckets;
    hm_entry_status* buckets_status;
    size_t len;
    size_t index;
} hm_iter_map;


/**
 * some simple functions
 */

#define hm_map_size(l) ((l)->size)
#define hm_map_len(l) ((l)->len)

/**
 * some functin of map , like `insert`, `get` , `del` and `free`
 */

extern void hm_map_init(hm_map* map, hm_hash hash_key, hm_cmp cmp_key, hm_free free_key, hm_free free_val);
extern hm_map_ret hm_map_insert(hm_map* map, void* key, void* val);
extern hm_entry* hm_map_get(hm_map* map, void* key);
extern hm_map_ret hm_map_del(hm_map* map, void* key);
extern hm_map_ret hm_map_shrink(hm_map* map);
extern void hm_map_clear(hm_map* map);
extern void hm_map_free(hm_map* map);


/**
 * some funtion of iterator of map , like `next` and `has next`
 */

extern void hm_iter_map_init(hm_iter_map* iter, hm_map* map);
extern bool hm_iter_map_has_next(hm_iter_map* iter);
extern hm_entry* hm_iter_map_next(hm_iter_map* iter);

#endif