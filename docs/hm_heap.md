# The detailed information about `hm_heap`

<p align = "center">
    English | <a href = "./Chinese/hm_heap.zh-CN.md">简体中文</a>
</p>

## Introduction
- You can pass a pointer to any value into this heap
- It provides basic heap operations
- It supports `dynamic-growth` and `fixed-size` modes
- It supports sort by your `cmp` function
- It supports build by your vals array

## Functions
- **Initialize**
```c
/**
 * Initialize the heap(fixed-size heap)
 * @note - Use the parameter `capacity` to set the capacity for this heap
 * @note - Return `hm_heap_ret_error` when initialization fails
 * @note - Return `hm_heap_ret_suc` when initialization succeeds
 * @note - If you do `NOT` want the heap to free its values,
 * set the `hm_free` function pointer to `NULL`
 * @note - The `hm_cmp` function pointer `must not be NULL`
 */
hm_heap_ret hm_heap_init(hm_heap* heap, size_t capacity, hm_free free, hm_cmp cmp);

/**
 * Initialize the heap(dynamic-growth heap)
 * @note - Use the parameter `capacity` to set the capacity for this heap
 * @note - Return `hm_heap_ret_error` when initialization fails
 * @note - Return `hm_heap_ret_suc` when initialization succeeds
 * @note - If you do `NOT` want the heap to free its values,
 * set the `hm_free` function pointer to `NULL`
 * @note - The `hm_cmp` function pointer `must not be NULL`
 */
hm_heap_ret hm_heap_init_dynamic_grow(hm_heap* heap, size_t start_capacity, hm_free free, hm_cmp cmp);
```

- **Insert**
```c
/**
 * Insert a value in the heap
 * @note - Return `hm_heap_ret_full` when heap is full
 * @note - Return `hm_heap_ret_suc` when insert succeeds
 * @note - Return `hm_heap_ret_error` when heap is `dynamic-growth` and expansion fails
 */
hm_heap_ret hm_heap_insert(hm_heap* heap, void* val);
```

- **Extract And Peek**
```c
/**
 * Extract a value 
 * @note - Return `NULL` when the heap is empty
 */
void* hm_heap_extract(hm_heap* heap);

/**
 * Peek a value
 * @note - Return `NULL` when the heap is empty 
 */
void* hm_heap_peek(hm_heap* heap);
```

- **Judge**
```c
/**
 * Check if the heap is full
 */
bool hm_heap_is_full(hm_heap* heap);

/**
 * Check if the heap is empty
 */
bool hm_heap_is_empty(hm_heap* heap);
```

- **Build**
```c
/**
 * Build a heap(fixed-size heap) by the pass-in vals and some parameter
 * @warning - vals should be located in `heap memory` of system
 * @note - Return `hm_heap_ret_warn` when `size > capacity`
 */
hm_heap_ret hm_heap_build(hm_heap* heap, void** vals, size_t size, size_t capacity, hm_free free, hm_cmp cmp);

/**
 * Build a heap(dynamic-growth heap) by the pass-in vals and some parameter
 * @warning - vals should be located in `heap memory` of system
 * @note - Return `hm_heap_ret_warn` when `size > capacity`
 */
hm_heap_ret hm_heap_build_dynamic_grow(hm_heap* heap, void** vals, size_t size, size_t capacity, hm_free free, hm_cmp cmp);

/**
 * Rebuild heap by the new cmp function
 */
void hm_heap_rebuild(hm_heap* heap, hm_cmp new_cmp);
```

- **Clear And Free**
```c
/**
 * Clear the heap 
 * @note - Only free the values(if possible),  but keep the vals array existed
 */
void hm_heap_clear(hm_heap* heap);

/**
 * Free all contents of the heap
 * @note - The heap can be reused when it is `dynamic-growth` but `fixed-size` cannot
 */
void hm_heap_free(hm_heap* heap);
```

- **Small Functions**
```c
#define hm_heap_size(s) ((s)->size)
#define hm_heap_capacity(s) ((s)->capacity)
```

## Tips