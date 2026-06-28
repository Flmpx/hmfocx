# The detailed information about `hm_pool`

<p align = "center">
    English | <a href = "./Chinese/hm_pool.zh-CN.md">简体中文</a>
</p>

## Introduction
- You can get fixed-size memory block from this memory pool
- It provides basic fixed-size block memory pool operations

## Functions
- **Initialize**
```c
/**
 * Initialize the memory pool 
 * @note - `hm_pool_block_allocate` will return `NULL` if `blocks_per_page` is `zero` or `block_size` is `zero`
 */
void hm_pool_init(hm_pool* pool, size_t block_size, size_t blocks_per_page);
```

- **Allocate**
```c
/**
 * Get pointer of to block
 * @note - It will return `NULL` when allocation fails
 * @note - It will return `NULL` if `blocks_per_page` is `zero` or `block_size` is `zero`
 */
void* hm_pool_block_allocate(hm_pool* pool);
```

- **Free**
```c
/**
 * Free a block
 * @note - The pointer of block must match the memory pool
 */
void hm_pool_block_free(hm_pool* pool, void* block);

/**
 * Free all content of the memory pool
 * @note Do not use blocks from a freed memory pool
 */
void hm_pool_free(hm_pool* pool);
```

- **Small Functions**
```c
/**
 * Get the number of pages in the given memory pool
 * 
 */
size_t hm_pool_get_pages(hm_pool* pool);

/**
 * Get the theoretical number of bytes per page in the memory pool
 * 
 */
size_t hm_pool_get_bytes_per_page(hm_pool* pool);
```


## Tips