# The detailed information about `hm_list`

<p align = "center">
    English | <a href = "./Chinese/hm_list.zh-CN.md">简体中文</a>
</p>

## Introduction
- You can pass a pointer to any value into this list
- It provides basic list operations

## Functions
- **Initialize**
```c
/**
 * Initialize a list
 * @note - Use `hm_list_free()` to free the list
 * @note - If you do `NOT` want the list to free its values,
 * set the `hm_free` function pointer to `NULL`
 */
void hm_list_init(hm_list* list, hm_free free);
```

- **Insert**
```c
/**
 * Insert a value at the head of the list
 * @note - This function will return `hm_list_ret_error` when insertion fails
 */
hm_list_ret hm_list_insert_head(hm_list* list, void* val);

/**
 * Insert a value at the tail of the list
 * @note - This function will return `hm_list_ret_error` when insertion fails
 */
hm_list_ret hm_list_insert_tail(hm_list* list, void* val);

/**
 * Insert a value at the given `index`
 * @note - `Index` must be >= `0`, and <= `the size of list`
 * 
 * @note - This function will return `hm_list_ret_warn` when the `index` is out of bounds
 * 
 * @note - Also, this function will return `hm_list_ret_error` when insertion fails
 */
hm_list_ret hm_list_insert_index(hm_list* list, void* val, size_t index);
```

- **Get**
```c
/**
 * Get a pointer to the value at the given `index`
 * @note - `Index` must be >= `0`, and < `the size of list`
 * @note - This function will return nullptr when the `index ` is out of bounds
 */
void* hm_list_get(hm_list* list, size_t index);
```

- **Del**
```c
/**
 * Delete the Node at the head of the list
 * @note - This function will return `hm_list_ret_none` when list is empty
 */
hm_list_ret hm_list_del_head(hm_list* list);

/**
 * Delete the Node at the tail of the list
 * @note - This function will return `hm_list_ret_none` when list is empty
 */
hm_list_ret hm_list_del_tail(hm_list* list);

/**
 * Delete the Node at the specified `index`
 * @note - `Index` must be >= `0`, and < `the size of list`
 * 
 * @note - This function will return `hm_list_ret_none` when the `index` is out of bounds
 * 
 */
hm_list_ret hm_list_del_index(hm_list* list, size_t index);
```

- **Iterator**
```c

// These functions will be remove

/**
 * Initialize iterator of list
 * 
 */
void hm_iter_list_init(hm_iter_list* iter, hm_list* list);

/**
 * Check if the iterator has a next element
 * @note - Return true if iterator has next
 */
bool hm_iter_list_has_next(hm_iter_list* iter);

/**
 * Get next value of list
 * @note - Use `hm_iter_list_has_next()` to check before calling `hm_iter_list_next()`
 */
void* hm_iter_list_next(hm_iter_list* iter);


// New functions of iterator

/**
 * Initialize iterator of list
 * @note - Let the iterator point to the head of the list
 */
void hm_iter_list_init_head(hm_iter_list* iter, hm_list* list);

/**
 * Initialize iterator of list
 * @note - Let the iterator point to the tail of the list
 */
void hm_iter_list_init_tail(hm_iter_list* iter, hm_list* list);

/**
 * Initialize iterator of list
 * @note - Let the iterator point to the specified `index` of the list
 * @note - Iterator will point to `NULL` if `index` is out of bounds
 */
void hm_iter_list_init_index(hm_iter_list* iter, hm_list* list, size_t index);

/**
 * Check if the iterator's current pointer is valid
 */
bool hm_iter_list_has_cur(hm_iter_list* iter);

/**
 * Get the current value of the iterator
 * Use `hm_iter_list_has_cur()` to check before calling `hm_iter_list_cur()`
 */
void* hm_iter_list_cur(hm_iter_list* iter);

/**
 * Move the iterator's pointer to next
 */
void hm_iter_list_move_next(hm_iter_list* iter);

/**
 * Move the iterator's pointer to prev
 */
void hm_iter_list_move_prev(hm_iter_list* iter);
```


- **Free**
```c
/**
 * Free a list
 * @note - If the list was initialized without freeing capacity, 
 * this function can't free the values stored in the Nodes
 */
void hm_list_free(hm_list* list);
```

- **Sort**
```c
/**
 * Sort list
 * @note - Pass a comparison function to this function
 */
void hm_list_sort(hm_list* list, hm_cmp cmp);
```

- **Small Functions**
```c
#define hm_list_size(l) ((l)->size)
```



## Tips

- **Do not modify the list (del, insert, free, or sort) while iterating over it**
