/*
 * Copyright (c) 2026 Flmpx
 * Licensed under MIT (see LICENSE).
 */
#include "../include/hm_set.h"
#include <stdlib.h>
#include <stdint.h>

size_t hm_set_size(hm_set* set) {
    return set->size;
}

size_t hm_set_len(hm_set* set) {
    return set->len;
}


/**
 * `hm_set_get_index()` returns `invalid_index(SIZE_MAX)` to represents that the key was not found
 */
static const size_t invalid_index = SIZE_MAX;


/**
 * `min_len` represents the minimal length of set
 */
static const size_t min_len = 17;

/**
 * `hm_same` represents that two pieces of key are equal
 */
static const int hm_same = 0;


/**
 * Max load factor in set
 */
static const double max_load_factor = 0.75;


/**
 * Min load factor in set (only used in shrink function)
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
 * Initialize hm_set
 * @note - This function requires not only `free` function for keys, 
 * but also `hash` and `cmp` functions for keys
 * @note - Like `list`, the `free_key` parameters is optional (can be NULL), 
 * but `hash_key` and `cmp_key` must not be NULL
 */
void hm_set_init(hm_set* set, hm_hash hash_key, hm_cmp cmp_key, hm_free free_key) {
    *set = (hm_set){.buckets = NULL,
                    .buckets_status = NULL,
                    .cmp = cmp_key,
                    .free_key = free_key,
                    .hash = hash_key,
                    .len = 0,
                    .size = 0};
}

/**
 * Add and entry to the set
 * @warning - This function is used by `hm_set_insert()`
 * 
 * @warning - Ensure there is enough space to insert before calling this function
 * 
 * @note - If the key already exists, the old entry(key) remains in the map. And return `hm_set_ret_existed`. 
 * Therefore, you should handle this special situation
 */
static hm_set_ret hm_set_addfunc(hm_set* set, void* key) {
    size_t l = set->len;

    size_t index = set->hash(key) % l;

    bool flag_find_del = false;
    size_t first_del_index = invalid_index;

    
    for (size_t i = 0; i < l; i++) {
        if (set->buckets_status[index] == hm_none_in_set) {
            break;
        }
        if (!flag_find_del && set->buckets_status[index] == hm_del_in_set) {
            flag_find_del = true;
            first_del_index = index;
        }

        if (set->buckets_status[index] == hm_exist_in_set && set->cmp(set->buckets[index].key, key) == hm_same) {
            /*keep the same and old entry(key) */
            return hm_set_ret_existed;
        }

        index = (index + 1) % l;
    }
    if (flag_find_del) {
        index = first_del_index;
    }

    set->buckets[index] = (hm_set_entry){key};
    set->buckets_status[index] = hm_exist_in_set;

    set->size++;

    return hm_set_ret_suc;

    

}


/**
 * Add an entry in set
 * @warning - This function is used by `hm_set_fresh()`, because the buckets are empty when the set is freshly created, so this function handles fewer cases
 */
static hm_set_ret hm_set_addfunc_fresh(hm_set* set, void* key) {
    size_t l = set->len;
    size_t index = set->hash(key) % l;
    while (set->buckets_status[index] != hm_none_in_set) {
        index = (index + 1) % l;
    }

    set->buckets[index] = (hm_set_entry){key};
    set->buckets_status[index] = hm_exist_in_set;
    set->size++;

    return hm_set_ret_suc;
}

/**
 * Fresh set with the new length
 * 
 * @note - This function returns `hm_set_ret_warn` when the size of set is greater than the `new_len` 
 */
static hm_set_ret hm_set_fresh(hm_set* set, size_t new_len) {
    size_t old_l = set->len, old_s = set->size;
    if (old_s > new_len) {
        return hm_set_ret_warn;
    }

    hm_set new_set;

    hm_set_init(&new_set, set->hash, set->cmp, set->free_key);
    new_set.len = new_len;

    if (new_len > SIZE_MAX / sizeof(hm_set_entry) || new_len > SIZE_MAX / sizeof(hm_set_entry_status)) {
        return hm_set_ret_error;
    }
    new_set.buckets = (hm_set_entry*)malloc(new_len * sizeof(hm_set_entry));
    if (new_set.buckets == NULL) {
        return hm_set_ret_error;
    }
    new_set.buckets_status = (hm_set_entry_status*)malloc(new_len * sizeof(hm_set_entry_status));
    if (new_set.buckets_status == NULL) {
        free(new_set.buckets);
        return hm_set_ret_error;
    }
    for (size_t i = 0; i < new_len; i++) {
        new_set.buckets_status[i] = hm_none_in_set;
    }

    hm_set_entry e;
    for (size_t i = 0; i < old_l; i++) {
        if (set->buckets_status[i] == hm_exist_in_set) {
            e = set->buckets[i];
            hm_set_addfunc_fresh(&new_set, e.key);
        }
    }

    if (set->buckets) {
        free(set->buckets);
    }
    if (set->buckets_status) {
        free(set->buckets_status);
    }

    *set = new_set;

    return hm_set_ret_suc;
    

}


/**
 * Insert a key into the set
 * @note - Return `hm_set_ret_error` on failure
 * @note - Return `hm_set_ret_suc` on success
 * @note - If the key already exists, the old entry(key) remains in the map. And return `hm_set_ret_existed`.
 * Therefore, you should handle this special situation
 */
hm_set_ret hm_set_insert(hm_set* set, void* key) {
    size_t l = set->len, s = set->size;

    bool flag_fresh = false;
    size_t new_len = 0;
    if (l == 0) {
        flag_fresh = true;
        new_len = min_len;
    } else if (((double)(s) / l) > max_load_factor) {

        if (l > SIZE_MAX / 2) {
            return hm_set_ret_error;
        }

        flag_fresh = true;
        new_len = max_prime(2 * l);
        // Check the return number of `max_prime`
        if (new_len == SIZE_MAX) {
            return hm_set_ret_error;
        }
    }

    if (flag_fresh) {
        if (hm_set_fresh(set, new_len) != hm_set_ret_suc) {
            return hm_set_ret_error;
        }
    }
    return hm_set_addfunc(set, key);


}

/**
 * Get the index of the key in the set
 * @note - If the key does not exist in the set, this function returns `invalid_index(SIZE_MAX)`
 */
static size_t hm_set_get_index(hm_set* set, void* key) {
    size_t s = set->size, l = set->len;
    if (s == 0 || l == 0) {
        return invalid_index;
    }

    size_t index = set->hash(key) % l;

    hm_set_entry_status status;
    for (size_t i = 0; i < l; i++) {
        status = set->buckets_status[index];
        
        if (status == hm_none_in_set) {
            break;
        }
        if (status == hm_exist_in_set && set->cmp(set->buckets[index].key, key) == hm_same) {
            return index;
        } 
        index = (index + 1) % l;
    }
    return invalid_index;
}
/**
 * Get a pointer to the entry in the set
 * @note - If the key does not exist, return `NULL`
 */
hm_set_entry* hm_set_get(hm_set* set, void* key) {
    size_t s = set->size, l = set->len;
    if (s == 0 || l == 0) {
        return NULL;
    }
    size_t index = hm_set_get_index(set, key);

    if (index == invalid_index) {
        return NULL;
    } else {
        return &(set->buckets[index]);
    }
}
/**
 * Delete the entry associated with the given key
 * @note - If the key does not exist, returns `hm_set_ret_none`
 */
hm_set_ret hm_set_del(hm_set* set, void* key) {
    size_t s = set->size, l = set->len;
    if (s == 0 || l == 0) {
        return hm_set_ret_none;
    }
    size_t index = hm_set_get_index(set, key);

    if (index == invalid_index) {
        return hm_set_ret_none;
    } else {
        if (set->free_key) set->free_key(set->buckets[index].key);

        set->buckets_status[index] = hm_del_in_set;

        set->size--;

        return hm_set_ret_suc;
    }
}
/**
 * Shrink the length of set if possible
 * @note - Returns `hm_set_ret_none` if the set can't be shrunk
 */
hm_set_ret hm_set_shrink(hm_set* set) {
    size_t l = set->len, s = set->size;
    if (l < min_len * 2 || ((double)s / l) > min_load_factor) {
        return hm_set_ret_none;
    }

    size_t new_len = max_prime(l / 2);

    return hm_set_fresh(set, new_len);
}

/**
 * Free the keys in set but keeps the buckets and buckets_status array existed
 */
void hm_set_clear(hm_set* set) {
    size_t l = set->len;
    if (set->free_key) {
        for (size_t i = 0; i < l; i++) {
            if (set->buckets_status[i] == hm_exist_in_set) {
                set->free_key(set->buckets[i].key);
            }
            set->buckets_status[i] = hm_none_in_set;
        }
    } else {
        for (size_t i = 0; i < l; i++) {
            set->buckets_status[i] = hm_none_in_set;
        }
    }
    set->size = 0;

}
/**
 * Free all contents of the set
 */
void hm_set_free(hm_set* set) {
    hm_set_clear(set);
    free(set->buckets);
    free(set->buckets_status);
    hm_set_init(set, set->hash, set->cmp, set->free_key);
}


/**
 * Initialize the iterator of set
 */
void hm_set_iter_init(hm_set_iter* iter, hm_set* set) {
    iter->buckets = set->buckets;
    iter->buckets_status = set->buckets_status;
    iter->index = 0;
    iter->len = set->len;
}
/**
 * Check if the iterator has a next entry
 * @note - Return true if the iterator has next
 */
bool hm_set_iter_has_next(hm_set_iter* iter) {
    size_t l = iter->len;
    size_t index = iter->index;
    
    hm_set_entry_status status;
    while (index < l) {
        status = iter->buckets_status[index];
        if (status == hm_exist_in_set) {
            iter->index = index;
            return true;
        }
        index++;
    }
    return false;

}
/**
 * Get next entry of set
 * @note - Use `hm_set_iter_has_next()` to check before calling `hm_set_iter_next()`
 */
hm_set_entry* hm_set_iter_next(hm_set_iter* iter) {
    size_t l = iter->len;
    size_t index = iter->index;

    hm_set_entry_status status;
    while (index < l) {
        status = iter->buckets_status[index];
        if (status == hm_exist_in_set) {
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
 * Get the load factor of the set 
 * @note - Return a negative number when the length of the set is 0
 */
double hm_set_get_load_factor(hm_set* set) {
    if (set->len) {
        return (double)set->size / set->len;
    } else {
        return -1;
    }
}