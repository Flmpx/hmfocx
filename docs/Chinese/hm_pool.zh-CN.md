# `hm_pool` 详细信息

<p align = "center">
    <a href = "../hm_pool.md">English</a> | 简体中文
</p>

## 介绍

- 你可以从该内存池中获取固定大小的内存块。
- 它提供基本的固定大小块内存池操作。

## 函数

- **初始化**
```c
/**
 * 初始化内存池
 * @note - 如果 `blocks_per_page` 或者 `block_size` 为零，`hm_pool_block_allocate` 将返回 `NULL`
 */
void hm_pool_init(hm_pool* pool, size_t block_size, size_t blocks_per_page);
```

- **分配**
```c
/**
 * 获取一个块的指针
 * @note - 分配失败时返回 `NULL`
 * @note - 如果 `blocks_per_page` 或者 `block_size` 为零，`hm_pool_block_allocate` 将返回 `NULL`
 */
void* hm_pool_block_allocate(hm_pool* pool);
```

- **释放**
```c
/**
 * 释放一个块
 * @note - 块的指针必须属于该内存池
 */
void hm_pool_block_free(hm_pool* pool, void* block);

/**
 * 释放内存池的所有内容
 * @note - 释放后请勿使用来自该内存池的块
 */
void hm_pool_free(hm_pool* pool);
```

- **小功能**
```c
/**
 * 获取给定内存池的页数
 */
size_t hm_pool_get_pages(hm_pool* pool);

/**
 * 获取内存池中每页的理论字节数
 */
size_t hm_pool_get_bytes_per_page(hm_pool* pool);
```

## 提示