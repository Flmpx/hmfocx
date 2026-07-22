/*
 * Copyright (c) 2026 Flmpx
 * Licensed under MIT (see LICENSE).
 */
#include "../include/hm_map.h"
#include <stdlib.h>
#include <stdint.h>
/**
 * `hm_map_get_index()` returns `invalid_index(SIZE_MAX)` to represents that the key was not found
 */
static const size_t invalid_index = SIZE_MAX;


/**
 * `min_len` represents the minimal length of map
 */
static const size_t min_len = 17;

/**
 * `hm_same` represents that two pieces of key are equal
 */
static const int hm_same = 0;


/**
 * Max load factor in map
 */
static const double max_load_factor = 0.75;


/**
 * Min load factor in map (only used in shrink function)
 */
static const double min_load_factor = 0.25;

/**
 * Determine whether a number is a prime number
 */
static bool is_prime(size_t n) {
    if (n < 2) return false;
    if (n == 2 || n == 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (size_t i = 5; i <= n / i; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) {
            return false;
        }
    }
    return true;
}

/**
 * Get the smallest prime number greater than the given number
 * @note - SIZE_MAX is an invalid number 
 */
static size_t max_prime(size_t n) {
    size_t i;
    for (i = n; i < SIZE_MAX; i++) {
        if (is_prime(i)){
            return i;
        }
    }
    return SIZE_MAX;
}

/**
 * Initialize hm_map
 * @note - This function requires not only `free` function for key and value, 
 * but also `hash` and `cmp` functions for keys
 * @note - Like `list`, the `free_key` and `free_val` parameters are optional (can be NULL), 
 * but `hash_key` and `cmp_key` must not be NULL
 */
void hm_map_init(hm_map* map, hm_hash hash_key, hm_cmp cmp_key, hm_free free_key, hm_free free_val) {
    *map = (hm_map){.buckets = NULL,
                    .buckets_status = NULL,
                    .cmp = cmp_key,
                    .free_key = free_key,
                    .free_val = free_val,
                    .hash = hash_key,
                    .len = 0,
                    .size = 0};
}

/**
 * Add and entry to the map
 * @warning - This function is used by `hm_map_insert()`
 * 
 * @warning - Ensure there is enough space to insert before calling this function
 * 
 * @note - If the key already exists, the old entry(include key and val) remains in the map. 
 * Therefore, you should handle this special situation
 */
static hm_map_ret hm_map_addfunc(hm_map* map, void* key, void* val) {
    size_t l = map->len;

    size_t index = map->hash(key) % l;

    bool flag_find_del = false;
    size_t first_del_index = invalid_index;

    
    for (size_t i = 0; i < l; i++) {
        if (map->buckets_status[index] == hm_none_in_map) {
            break;
        }
        if (!flag_find_del && map->buckets_status[index] == hm_del_in_map) {
            flag_find_del = true;
            first_del_index = index;
        }

        if (map->buckets_status[index] == hm_exist_in_map && map->cmp(map->buckets[index].key, key) == hm_same) {
            /*keep the same and old entry(including key and val) */
            return hm_map_ret_existed;
        }

        index = (index + 1) % l;
    }
    if (flag_find_del) {
        index = first_del_index;
    }

    map->buckets[index] = (hm_map_entry){key, val};
    map->buckets_status[index] = hm_exist_in_map;

    map->size++;

    return hm_map_ret_suc;

    

}


/**
 * Add an entry in map
 * @warning - This function is used by `hm_map_fresh()`, because the buckets are empty when the map is freshly created, so this function handles fewer cases
 */
static hm_map_ret hm_map_addfunc_fresh(hm_map* map, void* key, void* val) {
    size_t l = map->len;
    size_t index = map->hash(key) % l;
    while (map->buckets_status[index] != hm_none_in_map) {
        index = (index + 1) % l;
    }

    map->buckets[index] = (hm_map_entry){key, val};
    map->buckets_status[index] = hm_exist_in_map;
    map->size++;

    return hm_map_ret_suc;
}

/**
 * Fresh map with the new length
 * 
 * @note - This function returns `hm_map_ret_warn` when the size of map is greater than the `new_len` 
 */
static hm_map_ret hm_map_fresh(hm_map* map, size_t new_len) {
    size_t old_l = map->len, old_s = map->size;
    if (old_s > new_len) {
        return hm_map_ret_warn;
    }

    hm_map new_map;

    hm_map_init(&new_map, map->hash, map->cmp, map->free_key, map->free_val);
    new_map.len = new_len;

    if (new_len > SIZE_MAX / sizeof(hm_map_entry) || new_len > SIZE_MAX / sizeof(hm_map_entry_status)) {
        return hm_map_ret_error;
    }
    new_map.buckets = (hm_map_entry*)malloc(new_len * sizeof(hm_map_entry));
    if (new_map.buckets == NULL) {
        return hm_map_ret_error;
    }
    new_map.buckets_status = (hm_map_entry_status*)malloc(new_len * sizeof(hm_map_entry_status));
    if (new_map.buckets_status == NULL) {
        free(new_map.buckets);
        return hm_map_ret_error;
    }
    for (size_t i = 0; i < new_len; i++) {
        new_map.buckets_status[i] = hm_none_in_map;
    }

    hm_map_entry e;
    for (size_t i = 0; i < old_l; i++) {
        if (map->buckets_status[i] == hm_exist_in_map) {
            e = map->buckets[i];
            hm_map_addfunc_fresh(&new_map, e.key, e.val);
        }
    }

    if (map->buckets) {
        free(map->buckets);
    }
    if (map->buckets_status) {
        free(map->buckets_status);
    }

    *map = new_map;

    return hm_map_ret_suc;
    

}


/**
 * Insert a key-value pair into the map
 * @note - Return `hm_map_ret_error` on failure
 * @note - Return `hm_map_ret_suc` on success
 * @note - If the key already exists, the old entry(include key and val) remains in the map. And return `hm_map_ret_existed`.
 * Therefore, you should handle this special situation
 * @note - Use `hm_map_get()` to change val if you want to change the val or it's pointer
 */
hm_map_ret hm_map_insert(hm_map* map, void* key, void* val) {
    size_t l = map->len, s = map->size;

    bool flag_fresh = false;
    size_t new_len = 0;
    if (l == 0) {
        flag_fresh = true;
        new_len = min_len;
    } else if (((double)(s) / l) > max_load_factor) {

        if (l > SIZE_MAX / 2) {
            return hm_map_ret_error;
        }

        flag_fresh = true;
        new_len = max_prime(2 * l);
        // Check the return number of `max_prime`
        if (new_len == SIZE_MAX) {
            return hm_map_ret_error;
        }
    }

    if (flag_fresh) {
        if (hm_map_fresh(map, new_len) != hm_map_ret_suc) {
            return hm_map_ret_error;
        }
    }
    return hm_map_addfunc(map, key, val);


}

/**
 * Get the index of the key in the map
 * @note - If the key does not exist in the map, this function returns `invalid_index(SIZE_MAX)`
 */
static size_t hm_map_get_index(hm_map* map, void* key) {
    size_t s = map->size, l = map->len;
    if (s == 0 || l == 0) {
        return invalid_index;
    }

    size_t index = map->hash(key) % l;

    hm_map_entry_status status;
    for (size_t i = 0; i < l; i++) {
        status = map->buckets_status[index];
        
        if (status == hm_none_in_map) {
            break;
        }
        if (status == hm_exist_in_map && map->cmp(map->buckets[index].key, key) == hm_same) {
            return index;
        } 
        index = (index + 1) % l;
    }
    return invalid_index;
}
/**
 * Get a pointer to the  entry in the map
 * @note - If the key does not exist, return `NULL`
 */
hm_map_entry* hm_map_get(hm_map* map, void* key) {
    size_t s = map->size, l = map->len;
    if (s == 0 || l == 0) {
        return NULL;
    }
    size_t index = hm_map_get_index(map, key);

    if (index == invalid_index) {
        return NULL;
    } else {
        return &(map->buckets[index]);
    }
}
/**
 * Delete the entry associated with the given key
 * @note - If the key does not exist, returns `hm_map_ret_none`
 */
hm_map_ret hm_map_del(hm_map* map, void* key) {
    size_t s = map->size, l = map->len;
    if (s == 0 || l == 0) {
        return hm_map_ret_none;
    }
    size_t index = hm_map_get_index(map, key);

    if (index == invalid_index) {
        return hm_map_ret_none;
    } else {
        if (map->free_key) map->free_key(map->buckets[index].key);
        if (map->free_val) map->free_val(map->buckets[index].val);

        map->buckets_status[index] = hm_del_in_map;

        map->size--;

        return hm_map_ret_suc;
    }
}
/**
 * Shrink the length of map if possible
 * @note - Returns `hm_map_ret_none` if the map can't be shrunk
 */
hm_map_ret hm_map_shrink(hm_map* map) {
    size_t l = map->len, s = map->size;
    if (l < min_len * 2 || ((double)s / l) > min_load_factor) {
        return hm_map_ret_none;
    }

    size_t new_len = max_prime(l / 2);

    return hm_map_fresh(map, new_len);
}

/**
 * Free the keys and values in map but keeps the buckets and buckets_status array existed
 */
void hm_map_clear(hm_map* map) {
    size_t l = map->len;
    for (size_t i = 0; i < l; i++) {
        if (map->buckets_status[i] == hm_exist_in_map) {
            if (map->free_key) map->free_key(map->buckets[i].key);
            if (map->free_val) map->free_val(map->buckets[i].val);
        }
        map->buckets_status[i] = hm_none_in_map;
    }
    map->size = 0;

}
/**
 * Free all contents of the map
 */
void hm_map_free(hm_map* map) {
    hm_map_clear(map);
    free(map->buckets);
    free(map->buckets_status);
    hm_map_init(map, map->hash, map->cmp, map->free_key, map->free_val);
}


/**
 * Initialize the iterator of map
 */
void hm_map_iter_init(hm_map_iter* iter, hm_map* map) {
    iter->buckets = map->buckets;
    iter->buckets_status = map->buckets_status;
    iter->index = 0;
    iter->len = map->len;
}
/**
 * Check if the iterator has a next entry
 * @note - Return true if the iterator has next
 */
bool hm_map_iter_has_next(hm_map_iter* iter) {
    size_t l = iter->len;
    size_t index = iter->index;
    
    hm_map_entry_status status;
    while (index < l) {
        status = iter->buckets_status[index];
        if (status == hm_exist_in_map) {
            iter->index = index;
            return true;
        }
        index++;
    }
    return false;

}
/**
 * Get next entry of map
 * @note - Use `hm_map_iter_has_next()` to check before calling `hm_map_iter_next()`
 */
hm_map_entry* hm_map_iter_next(hm_map_iter* iter) {
    size_t l = iter->len;
    size_t index = iter->index;

    hm_map_entry_status status;
    while (index < l) {
        status = iter->buckets_status[index];
        if (status == hm_exist_in_map) {
            /*next index is start of next entry*/
            iter->index = index + 1;
            return &(iter->buckets[index]);
        }
        index++;
    }
    iter->index = index;
    return NULL;

}


/**
 * Get the load factor of the map 
 * @note - Return a negative number when the length of the map is 0
 */
double hm_map_get_load_factor(hm_map* map) {
    if (map->len) {
        return (double)map->size / map->len;
    } else {
        return -1;
    }
}