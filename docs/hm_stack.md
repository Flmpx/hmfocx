# The detailed information about `hm_stack`

<p align = "center">
    English | <a href = "./Chinese/hm_stack.zh-CN.md">简体中文</a>
</p>

## Introduction
- You can pass a pointer to any value into this stack
- It provides basic stack operations
- It supports `dynamic-growth` and `fixed-size` modes

## Functions

### **Small Functions**
```c
#define hm_stack_size(s) ((s)->top)
#define hm_stack_capacity(s) ((s)->capacity)
```

### **Initialize**
```c
/**
 * Initialize the stack(fixed-size stack)
 * @note - Use the parameter `capacity` to set the size for this stack
 * @note - Return `hm_stack_ret_error` when initialization fails
 * @note - Return `hm_stack_ret_suc` when initialization succeeds
 * @note - If you do `NOT` want the stack to free its values,
 * set the `hm_free` function pointer to `NULL`
 */
hm_stack_ret hm_stack_init(hm_stack* stack, size_t capacity, hm_free free);

/**
 * Initialize the stack(dynamic-growth stack)
 * @note - Use the parameter `start_capacity` to set the start size for this stack
 * @note - Return `hm_stack_ret_error` when initialization fails
 * @note - Return `hm_stack_ret_suc` when initialization succeeds
 * @note - If you do `NOT` want the stack to free its values,
 * set the `hm_free` function pointer to `NULL`
 */
hm_stack_ret hm_stack_init_dynamic_grow(hm_stack* stack, size_t start_capacity, hm_free free);
```

### **Push**
```c
/**
 * Push a value to the stack
 * @note - Return `hm_stack_ret_full` when stack is full
 * @note - Return `hm_stack_ret_suc` when push succeeds
 * @note - Return `hm_stack_ret_error` when stack is `dynamic-growth` and expansion fails
 */
hm_stack_ret hm_stack_push(hm_stack* stack, void* val);
```

### **Pop And Peek**
```c
/**
 * Pop a value from the stack
 * @note - Return `NULL` when the stack is empty
 */
void* hm_stack_pop(hm_stack* stack);

/**
 * Peek a value from the stack
 * @note - Return `NULL` when the stack is empty 
 */
void* hm_stack_peek(hm_stack* stack);
```

### **Judge**
```c
/**
 * Check if the stack is full
 */
bool hm_stack_is_full(hm_stack* stack);

/**
 * Check if the stack is empty
 */
bool hm_stack_is_empty(hm_stack* stack);
```

### **Clear And Free**
```c
/**
 * Clear the stack 
 * @note - Only free the values(if possible),  but keep the vals array existed
 */
void hm_stack_clear(hm_stack* stack);

/**
 * Free all contents of the stack
 * @note - The stack can be reused when it is `dynamic-growth` but `fixed-size` cannot
 */
void hm_stack_free(hm_stack* stack);
```

## Tips