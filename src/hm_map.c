#include "../include/hm_map.h"
#include <stdlib.h>
#include <stdint.h>
/**
 * hm_get_index will use the number to return the flag of not found
 */
static size_t invalid_index = SIZE_MAX;


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

static size_t max_prime(size_t n) {
    size_t i;
    for (i = n; i < SIZE_MAX; i++) {
        if (is_prime(i)){
            return i;
        }
    }
    return i;
}


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


static hm_info hm_map_addfunc(hm_map* map, void* key, void* val) {
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
            if (map->free_val) {
                map->free_val(map->buckets[index].val);
            }
            /*keep the same and old key */
            map->buckets[index].val = val;
            return hm_success;
        }

        index = (index + 1) % l;
    }
    if (flag_find_del) {
        index = first_del_index;
    }

    map->buckets[index] = (hm_entry){key, val};
    map->buckets_status[index] = hm_exist_in_map;

    map->size++;

    return hm_success;

    

}

static hm_info hm_map_addfunc_fresh(hm_map* map, void* key, void* val) {
    size_t l = map->len;
    size_t index = map->hash(key) % l;
    while (map->buckets_status[index] != hm_none_in_map) {
        index = (index + 1) % l;
    }

    map->buckets[index] = (hm_entry){key, val};
    map->buckets_status[index] = hm_exist_in_map;
    map->size++;

    return hm_success;
}

static hm_info hm_map_fresh(hm_map* map, size_t new_len) {
    size_t old_l = map->len, old_s = map->size;
    if (old_s > new_len) {
        return hm_warning;
    }

    hm_map new_map;

    hm_map_init(&new_map, map->hash, map->cmp, map->free_key, map->free_val);
    new_map.len = new_len;

    new_map.buckets = (hm_entry*)malloc(new_len * sizeof(hm_entry));
    if (new_map.buckets == NULL) {
        return hm_error;
    }
    new_map.buckets_status = (hm_entry_status*)malloc(new_len * sizeof(hm_entry_status));
    if (new_map.buckets_status == NULL) {
        free(new_map.buckets);
        return hm_error;
    }
    for (size_t i = 0; i < new_len; i++) {
        new_map.buckets_status[i] = hm_none_in_map;
    }

    hm_entry e;
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

    return hm_success;
    

}


hm_info hm_map_insert(hm_map* map, void* key, void* val) {
    size_t l = map->len, s = map->size;

    bool flag_fresh = false;
    size_t new_len = 0;
    if (l == 0) {
        flag_fresh = true;
        new_len = 17;
    } else if (4 * s > 3 * l) {
        if ( 2 * l < l) {
            return hm_error;
        }
        flag_fresh = true;
        new_len = max_prime(2 * l);
    }

    if (flag_fresh) {
        if (hm_map_fresh(map, new_len) != hm_success) {
            return hm_error;
        }
    }
    return hm_map_addfunc(map, key, val);


}


static size_t hm_map_get_index(hm_map* map, void* key) {
    size_t s = map->size, l = map->len;
    if (s == 0 || l == 0) {
        return invalid_index;
    }

    size_t index = map->hash(key) % l;

    hm_entry_status status;
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

hm_entry* hm_map_get(hm_map* map, void* key) {
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

hm_info hm_map_del(hm_map* map, void* key) {
    size_t s = map->size, l = map->len;
    if (s == 0 || l == 0) {
        return hm_none;
    }
    size_t index = hm_map_get_index(map, key);

    if (index == invalid_index) {
        return hm_none;
    } else {
        if (map->free_key) map->free_key(map->buckets[index].key);
        if (map->free_val) map->free_val(map->buckets[index].val);

        map->buckets_status[index] = hm_del_in_map;

        map->size--;

        return hm_success;
    }
}

hm_info hm_map_shrink(hm_map* map) {
    size_t l = map->len, s = map->size;
    if (l < 34 || 4 * s > l) {
        return hm_none;
    }

    size_t new_len = max_prime(l / 2);

    return hm_map_fresh(map, new_len);
}


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

void hm_map_free(hm_map* map) {
    hm_map_clear(map);
    free(map->buckets);
    free(map->buckets_status);
    hm_map_init(map, map->hash, map->cmp, map->free_key, map->free_val);
}



void hm_iter_map_init(hm_iter_map* iter, hm_map* map) {
    iter->buckets = map->buckets;
    iter->buckets_status = map->buckets_status;
    iter->index = 0;
    iter->len = map->len;
}

bool hm_iter_map_has_next(hm_iter_map* iter) {
    size_t l = iter->len;
    size_t index = iter->index;
    
    hm_entry_status status;
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

hm_entry* hm_iter_map_next(hm_iter_map* iter) {
    size_t l = iter->len;
    size_t index = iter->index;

    hm_entry_status status;
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
