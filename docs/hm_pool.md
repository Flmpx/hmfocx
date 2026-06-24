# The detail infomation of `hm_pool`
## Introduce
- You can get fixed-size memory block from this memory pool
- It contains some basic functon of pool with fixed-size block

## Functions
- **Initialize**
```c
/**
 * Initialize the memory pool 
 * @note - `hm_pool_block_allocate` will reurn a valid pointer when the `block_size` is `zero` because memory alignment
 * @note - `hm_pool_block_allocate` will return `NULL` if `blocks_per_page` is `zero` 
 */
void hm_pool_init(hm_pool* pool, size_t block_size, size_t blocks_per_page);
```

- **Allocate**
```c
/**
 * Get pointer of a block
 * @note - It will return `NULL` when allocate failed
 */
void* hm_pool_block_allocate(hm_pool* pool);
```

- **Free**
```c
/**
 * Free a block
 * @note - The pointer of block must match to the memory pool
 */
void hm_pool_block_free(hm_pool* pool, void* block);

/**
 * Free the all content of memory pool
 * @note You don't use the block from a freed memory pool
 */
void hm_pool_free(hm_pool* pool);
```

- **Small Functions**
```c
/**
 * Get the number of pages in the memory pool that passed in
 * 
 */
size_t hm_pool_get_pages(hm_pool* pool);

/**
 * Get the theoretical number of bytes of every page in the memory pool
 * 
 */
size_t hm_pool_get_bytes_per_page(hm_pool* pool);
```


## Tips