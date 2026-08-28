/*
 * Copyright (c) 2026 Flmpx
 * Licensed under MIT (see LICENSE).
 */
#include "../include/hm_set.h"
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>


size_t hm_set_size(hm_set* set) {
    assert(set != NULL);

    return set->size;
}

size_t hm_set_len(hm_set* set) {
    assert(set != NULL);

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
 * 
 * @return - Return `true` when pass-in number is prime number
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
 * 
 * @return - Return `SIZE_MAX` when failure 
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
 * Initialize set
 * 
 * @note - This function requires not only `free` function for keys, but also `hash` and `cmp` functions for keys
 * @note - Like `list`, the `free_key` parameters is optional (can be NULL), but `hash_key` and `cmp_key` must not be NULL
 */
void hm_set_init(hm_set* set, hm_hash hash_key, hm_cmp cmp_key, hm_free free_key) {
    assert(set != NULL);
    assert(hash_key != NULL);
    assert(cmp_key != NULL);

    *set = (hm_set){.buckets = NULL,
                    .buckets_status = NULL,
                    .cmp_key = cmp_key,
                    .free_key = free_key,
                    .hash_key = hash_key,
                    .len = 0,
                    .size = 0};
}

/**
 * Add and entry to the set
 * @note - If the key already exists, the old entry(key) remains in the set. And return `hm_set_ret_existed`. Therefore, you should handle this special situation
 * 
 * @return - Return `hm_set_ret_suc` when add success
 * @return - Return `hm_set_ret_existed` when the key has existed in set
 * 
 * @warning - This function is used by `hm_set_insert()`
 * @warning - Ensure there is enough space to insert before calling this function
 */
static hm_set_ret hm_set_addfunc(hm_set* set, void* key) {
    assert(set != NULL);

    size_t l = set->len;

    size_t index = set->hash_key(key) % l;

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

        if (set->buckets_status[index] == hm_exist_in_set && set->cmp_key(set->buckets[index].key, key) == hm_same) {
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
 * 
 * @return - Return `hm_set_ret_suc` when add success
 * 
 * @warning - This function is used by `hm_set_fresh()`, because the buckets are empty when the set is freshly created, so this function handles fewer cases
 */
static hm_set_ret hm_set_addfunc_fresh(hm_set* set, void* key) {
    assert(set != NULL);

    size_t l = set->len;
    size_t index = set->hash_key(key) % l;
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
 * @note - This function is used to change the length of set
 * 
 * @return - Return `hm_set_ret_warn` when `size of set` > `new_len` 
 * @return - Return `hm_set_ret_error` when fresh set failure
 * @return - Return `hm_set_ret_suc` when fresh set success
 */
static hm_set_ret hm_set_fresh(hm_set* set, size_t new_len) {
    assert(set != NULL);

    size_t old_l = set->len, old_s = set->size;
    if (old_s > new_len) {
        return hm_set_ret_warn;
    }

    hm_set new_set;

    hm_set_init(&new_set, set->hash_key, set->cmp_key, set->free_key);
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
 * 
 * @note - If the key already exists, the old entry(key) remains in the set. And return `hm_set_ret_existed`. Therefore, you should handle this special situation
 * 
 * @return - Return `hm_set_ret_suc` when insert success
 * @return - Return `hm_set_ret_error` when insert failure 
 * @return - Return `hm_set_ret_existed` when the key has existed in set
 */
hm_set_ret hm_set_insert(hm_set* set, void* key) {
    assert(set != NULL);

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
 * Reserve capacity and initialize set
 * 
 * @note - This function requires not only `free` function for keys, but also `hash` and `cmp` functions for keys
 * @note - Like `list`, the `free_key` parameters is optional (can be NULL), but `hash_key` and `cmp_key` must not be NULL
 * @note - Use the parameter `len` to set the start length of this set, the `min_len` is 17, `len` will be `min_len` if `len` < `min_len`
 * 
 * @return - Return `hm_set_ret_error` when initialize failure
 * @return - Return `hm_set_ret_suc` when initialize success
 */
hm_set_ret hm_set_init_reserve(hm_set* set, hm_hash hash_key, hm_cmp cmp_key, hm_free free_key, size_t len) {
    assert(set != NULL);
    assert(hash_key != NULL);
    assert(cmp_key != NULL);

    hm_set_init(set, hash_key, cmp_key, free_key);

    return hm_set_fresh(set, (len > min_len) ? len : min_len);

}

/**
 * Get the index of the key in the set
 * 
 * @return - Return `invalid_index(SIZE_MAX)` when key is not existed in set
 */
static size_t hm_set_get_index(hm_set* set, void* key) {
    assert(set != NULL);

    size_t s = set->size, l = set->len;
    if (s == 0 || l == 0) {
        return invalid_index;
    }

    size_t index = set->hash_key(key) % l;

    hm_set_entry_status status;
    for (size_t i = 0; i < l; i++) {
        status = set->buckets_status[index];
        
        if (status == hm_none_in_set) {
            break;
        }
        if (status == hm_exist_in_set && set->cmp_key(set->buckets[index].key, key) == hm_same) {
            return index;
        } 
        index = (index + 1) % l;
    }
    return invalid_index;
}
/**
 * Get a entry in the set
 * 
 * @note - Entry contains pointer to key
 * 
 * @return - Return `(hm_set_entry){NULL}` when key is not existed in set
 * 
 * @warning - Change key is prohibited
 */
hm_set_entry hm_set_get(hm_set* set, void* key) {
    assert(set != NULL);

    size_t s = set->size, l = set->len;
    if (s == 0 || l == 0) {
        return (hm_set_entry){NULL};
    }
    size_t index = hm_set_get_index(set, key);

    if (index == invalid_index) {
        return (hm_set_entry){NULL};
    } else {
        return set->buckets[index];
    }
}

/**
 * Pop the entry associated with the given key
 * 
 * @note - The entry will be removed but not free its memory(Memory Ownership Transfer)
 * @note - Entry contains pointer to key
 * 
 * @return - Return `(hm_set_entry){NULL}` when key is not existed in map
 */
hm_set_entry hm_set_pop(hm_set* set, void* key) {
    assert(set != NULL);

    size_t s = set->size, l = set->len;
    if (s == 0 || l == 0) {
        return (hm_set_entry){NULL};
    }
    size_t index = hm_set_get_index(set, key);

    if (index == invalid_index) {
        return (hm_set_entry){NULL};
    } else {

        set->buckets_status[index] = hm_del_in_set;

        set->size--;

        return set->buckets[index];
    }
}


/**
 * Delete the entry associated with the given key
 * 
 * @return - Return `hm_set_ret_suc` when delete success
 * @return - Return `hm_set_ret_none` when key is not existed in set
 */
hm_set_ret hm_set_del(hm_set* set, void* key) {
    assert(set != NULL);

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
 * 
 * @return - Return `hm_set_ret_suc` when shrink success
 * @return - Return `hm_set_ret_none` when the set can't be shrunk
 * @return - Return `hm_set_ret_error` when shrink failure
 */
hm_set_ret hm_set_shrink(hm_set* set) {
    assert(set != NULL);

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
    assert(set != NULL);

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
 * 
 * @warning - The set can't be used after call this function because the lifetime of set is over
 */
void hm_set_free(hm_set* set) {
    if (set == NULL) return;

    hm_set_clear(set);
    free(set->buckets);
    free(set->buckets_status);
    
    memset(set, 0, sizeof(hm_set));

}


/**
 * Initialize the iterator of set
 */
void hm_set_iter_init(hm_set_iter* iter, hm_set* set) {
    assert(iter != NULL);
    assert(set != NULL);

    iter->buckets = set->buckets;
    iter->buckets_status = set->buckets_status;
    iter->index = 0;
    iter->len = set->len;
}
/**
 * Check if the iterator has a next entry
 * 
 * @return - Return `true` if the iterator has next
 */
bool hm_set_iter_has_next(hm_set_iter* iter) {
    assert(iter != NULL);

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
 * 
 * @note - Use `hm_set_iter_has_next()` to check before calling `hm_set_iter_next()`
 * @note - Entry contains pointer to key
 * 
 * @return - Return `(hm_set_entry){NULL}` when iterator doesn't has next 
 */
hm_set_entry hm_set_iter_next(hm_set_iter* iter) {
    assert(iter != NULL);

    size_t l = iter->len;
    size_t index = iter->index;

    hm_set_entry_status status;
    while (index < l) {
        status = iter->buckets_status[index];
        if (status == hm_exist_in_set) {
            /*next index is start of next entry*/
            iter->index = index + 1;
            return iter->buckets[index];
        }
        index++;
    }
    iter->index = index;
    return (hm_set_entry){NULL};

}


/**
 * Get the load factor of the set 
 * 
 * @return - Return a `negative number` when the length of the set is `0`
 */
double hm_set_get_load_factor(hm_set* set) {
    assert(set != NULL);

    if (set->len) {
        return (double)set->size / set->len;
    } else {
        return -1;
    }
}