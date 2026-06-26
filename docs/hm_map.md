# The detailed information about `hm_map`

<p align = "center">
    English | <a href = "./Chinese/hm_map.zh-CN.md">简体中文</a>
</p>

## Introduction
- You can pass a pointer to any value and key into this map
- It provides basic map operations
## Functions
- **Initialize**
```c
/**
 * Initialize hm_map
 * @note - This function requires not only `free` function for key and value, 
 * but also `hash` and `cmp` functions for keys
 * @note - Like `list`, the `free_key` and `free_val` parameters are optional (can be NULL), 
 * but `hash_key` and `cmp_key` must not be NULL
 */
void hm_map_init(hm_map* map, hm_hash hash_key, hm_cmp cmp_key, hm_free free_key, hm_free free_val);
```

- **Insert**
```c
/**
 * Insert a key-value pair into the map
 * @note - Rreturn `hm_map_ret_error` on failure
 * @note - Return `hm_map_ret_suc` on success
 * @note - If the key already exists, the old key remains in the map. 
 * Therefore, if your key was dynamically allocated, you should free it (optional suggestion)
 */
hm_map_ret hm_map_insert(hm_map* map, void* key, void* val);
```

- **Get**
```c
/**
 * Get a pointer to the  entry in the map
 * @note - If the key does not exist, return `NULL`
 */
hm_entry* hm_map_get(hm_map* map, void* key);
```

- **Del**
```c
/**
 * Delete the entry associated with the given key
 * @note - If the key does not exist, returns `hm_map_ret_none`
 */
hm_map_ret hm_map_del(hm_map* map, void* key);
```

- **Shrink**
```c
/**
 * Shrink the length of map if possible
 * @note - Returns `hm_map_ret_none` if the map can't be shrunk
 */
hm_map_ret hm_map_shrink(hm_map* map);
```

- **Iterator**
```c
/**
 * Initialize the iterator of map
 */
void hm_iter_map_init(hm_iter_map* iter, hm_map* map);

/**
 * Check if the iterator has a next entry
 * @note - Return true if the iterator has next
 */
bool hm_iter_map_has_next(hm_iter_map* iter);

/**
 * Get next entry of map
 * @note - Use `hm_iter_map_has_next()` to check before calling `hm_iter_map_next()`
 */
hm_entry* hm_iter_map_next(hm_iter_map* iter);
```

- **Clear And Free**
```c
/**
 * Free the keys and values in map but keeps the buckets and buckets_status array existed
 */
void hm_map_clear(hm_map* map);

/**
 * Free all contents of the map
 */
void hm_map_free(hm_map* map);
```

- **Small Functions**
```c
#define hm_map_size(l) ((l)->size)
#define hm_map_len(l) ((l)->len)
```

## Tips

- **Do not modify the map(del, insert, clear, or free) while iterating over it**