# The detailed information about `hm_queue`

<p align = "center">
    English | <a href = "./Chinese/hm_queue.zh-CN.md">简体中文</a>
</p>

## Introduction
- You can pass a pointer to any value into this queue
- It provides basic queue operations
- It supports `dynamic-growth` and `fixed-size` modes

## Functions

### **Small Functions**
```c
#define hm_queue_size(s) ((s)->size)
#define hm_queue_capacity(s) ((s)->capacity)
```

### **Initialize**
```c
/**
 * Initialize the queue(fixed-size queue)
 * @note - Use the parameter `capacity` to set the size for this queue
 * @note - Return `hm_queue_ret_error` when initialization fails
 * @note - Return `hm_queue_ret_suc` when initialization succeeds
 * @note - If you do `NOT` want the queue to free its values,
 * set the `hm_free` function pointer to `NULL`
 */
hm_queue_ret hm_queue_init(hm_queue* queue, size_t capacity, hm_free free);

/**
 * Initialize the queue(dynamic-growth queue)
 * @note - Use the parameter `start_capacity` to set the start size for this queue
 * @note - Return `hm_queue_ret_error` when initialization fails
 * @note - Return `hm_queue_ret_suc` when initialization succeeds
 * @note - If you do `NOT` want the queue to free its values,
 * set the `hm_free` function pointer to `NULL`
 */
hm_queue_ret hm_queue_init_dynamic_grow(hm_queue* queue, size_t start_capacity, hm_free free);
```

### **Enqueue**
```c
/**
 * Enqueue a value
 * @note - Return `hm_queue_ret_full` when queue is full
 * @note - Return `hm_queue_ret_suc` when enqueue succeeds
 * @note - Return `hm_queue_ret_error` when queue is `dynamic-growth` and expansion fails
 */
hm_queue_ret hm_queue_enq(hm_queue* queue, void* val);
```

### **Dequeue And Peek**
```c
/**
 * Dequeue a value
 * @note - Return `NULL` when the queue is empty
 */
void* hm_queue_deq(hm_queue* queue);

/**
 * Peek a value
 * @note - Return `NULL` when the queue is empty 
 */
void* hm_queue_peek(hm_queue* queue);
```

### **Judge**
```c
/**
 * Check if the queue is full
 */
bool hm_queue_is_full(hm_queue* queue);

/**
 * Check if the queue is empty
 */
bool hm_queue_is_empty(hm_queue* queue);
```

### **Clear And Free**
```c
/**
 * Clear the queue 
 * @note - Only free the values(if possible),  but keep the vals array existed
 */
void hm_queue_clear(hm_queue* queue);

/**
 * Free all contents of the queue
 * @note - The queue can be reused when it is `dynamic-growth` but `fixed-size` cannot
 */
void hm_queue_free(hm_queue* queue);
```

## Tips