# The detail infomation of `hm_list`
## Introduce
- You can pass the pointer of any value to this list
- It contains some basic functon of list

## Functions
- **Initialize**
```c
/**
 * Initialize a list, this list can be freed with hm_list_free()
 * @note if you want this list don't have the power of free this value, let the hm_free function pointer to be NULL
 */
void hm_list_init(hm_list* list, hm_free free);
```

- **Insert**
```c
/**
 * Insert value in the head of list
 * this function will return `hm_list_ret_error` when malloc failed
 * 
 */
hm_list_ret hm_list_insert_head(hm_list* list, void* val);

/**
 * Insert value in the tail of list
 * this function will return `hm_list_ret_error` when malloc failed
 */
hm_list_ret hm_list_insert_tail(hm_list* list, void* val);

/**
 * Insert value at the index that you given in this list
 * the range of index must be >= `0`, and <= `the size of list`
 * 
 * this function will return `hm_list_ret_warn` when `index` is invalid
 * 
 * Also, this function will return `hm_list_ret_error` when malloc failed
 */
hm_list_ret hm_list_insert_index(hm_list* list, void* val, size_t index);
```

- **Get**
```c
/**
 * Get pointer of value by index that you given in a list
 * this function will return nullptr when index is invalid
 */
void* hm_list_get(hm_list* list, size_t index);
```

- **Del**
```c
/**
 * Del hm_listnode with value at head in list
 * this function will return `hm_list_ret_none` when list is empty
 */
hm_list_ret hm_list_del_head(hm_list* list);

/**
 * Del hm_listnode with value at end in list
 * this function will return `hm_list_ret_none` when list is empty
 */
hm_list_ret hm_list_del_tail(hm_list* list);

/**
 * Del hm_listnode with value at the index that you given in this list
 * the range of index must be >= `0`, and < `the size of list`
 * 
 * this function will return `hm_list_ret_none` when `index` is invalid
 * 
 */
hm_list_ret hm_list_del_index(hm_list* list, size_t index);
```

- **Iterator**
```c
/**
 * Initialize iterator of list
 * 
 */
void hm_iter_list_init(hm_iter_list* iter, hm_list* list);

/**
 * Check if the iterator of list has next value
 * Return true if iterator has next
 */
bool hm_iter_list_has_next(hm_iter_list* iter);

/**
 * Get next value of list
 * Please use function of `hm_iter_list_has_next` to check if list has next value
 */
void* hm_iter_list_next(hm_iter_list* iter);
```


- **Free**
```c
/**
 * Free a list
 * if this list don't have the power of free value,
 * this function can't free the val in hm_listnode
 */
void hm_list_free(hm_list* list);
```

- **Small Functions**
```c
#define hm_list_size(l) ((l)->size)
```



## Tips

- **You cann't change the structure of this list(like del, insert and free) when you are iterating a list.**
