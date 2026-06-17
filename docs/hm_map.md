# The detail infomation of `hm_map`
## Introduce
- You can pass the pointer of any value and key to this map
- It contains some basic functon of map

## Functions
- **Initialize**
```c
/**
 * Initialize hm_map
 * This function not only need the function of free key and value,
 * but also hash and compare key function for this function
 * like list, there some functions(`free_key` and `free_val`) that you don't to pass in
 * 
 * But `hash_key` and `cmp_key` cannot be NULL
 */
void hm_map_init(hm_map* map, hm_hash hash_key, hm_cmp cmp_key, hm_free free_key, hm_free free_val);
```

- **Insert**
```c
/**
 * Insert key and val in a map
 * Insert failed when this function return `hm_map_ret_error`,
 * and will return `hm_map_ret_suc` when insert successful
 * @note if the key has existed in this map, the old key will still existed in this map,
 * and this function will return `hm_map_ret_existed`,
 * so, if the key that you given is allocted, you should free it(only suggestion)
 */
hm_map_ret hm_map_insert(hm_map* map, void* key, void* val);
```

- **Get**
```c
/**
 * Get the pointer of entry in map
 * if this key is not existed in map, this function will return NULL
 */
hm_entry* hm_map_get(hm_map* map, void* key);
```

- **Del**
```c
/**
 * Del the entry by key in map
 * if the key is not existed in map, this funtion will return `hm_map_ret_none`
 */
hm_map_ret hm_map_del(hm_map* map, void* key);
```

- **Shrink**
```c
/**
 * shrink the len of map if possible
 * the function will return `hm_map_ret_none` if the map can't shrink the len
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
 * Check if the iterator of map has next entry
 * Return true if the iterator has next
 */
bool hm_iter_map_has_next(hm_iter_map* iter);

/**
 * Get next pointer of entry of map by iterator
 * Please use function of `hm_iter_map_has_next` to check if map has next entry
 */
hm_entry* hm_iter_map_next(hm_iter_map* iter);
```

- **Clear And Free**
```c
/**
 * free the key and val in map but keep the buckets and buckets_status exist
 */
void hm_map_clear(hm_map* map);

/**
 * free the all content of this map
 */
void hm_map_free(hm_map* map);
```

- **Small Functions**
```c
#define hm_map_size(l) ((l)->size)
#define hm_map_len(l) ((l)->len)
```

## Tips

- **You cann't change the structure of this map(like del, insert, clear and free) when you are iterating a map.**