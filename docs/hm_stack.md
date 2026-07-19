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
<br><br>

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
<details>
<summary style="color:yellow">Try: init</summary>

```c
#include <hm_stack.h>

#include <stdio.h>
#include <stdlib.h>

int main()
{
    int capacity = 50;
    hm_stack stack;
    // fixed-size
    hm_stack_init(&stack, capacity, free);
    hm_stack_free(&stack);
    
    // dynamic-grow
    hm_stack_init_dynamic_grow(&stack, capacity, free);
    hm_stack_free(&stack);

    return 0;
}
```
</details>
<br><br>


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
</details>


<details>
<summary style="color:yellow">Try: push & peek & pop</summary>

```c
#include <hm_stack.h>

#include <stdio.h>
#include <stdlib.h>

int main()
{
    int capacity = 20;
    hm_stack stack;
    // fixed-size
    hm_stack_init(&stack, capacity, free);
    
    // push
    for (int i = 0; i < capacity; i++) {
        int* v = (int*)malloc(sizeof(int));
        *v = i;
        hm_stack_push(&stack, v);
    }
    
    // peek
    int* val = hm_stack_peek(&stack);
    printf("%d\n", *val);

    // pop
    for (int i = 0; i < capacity; i++) {
        int* v = hm_stack_pop(&stack);
        printf("%d ", *v);
        free(v);
    }

    hm_stack_free(&stack);
    return 0;
}
```

<details>
<summary style="color:red">Run Result</summary>

```txt
19
19 18 17 16 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0 
```
</details>

</details>
<br><br>



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
<details>
<summary style="color:yellow">Try: judge</summary>

```c
#include <hm_stack.h>

#include <stdio.h>
#include <stdlib.h>

int main()
{
    int capacity = 20;
    hm_stack stack;
    // fixed-size
    hm_stack_init(&stack, capacity, free);
    
    if (hm_stack_is_empty(&stack)) {
        printf("stack is empty\n");
    }

    // push
    int i = 0;
    while (!hm_stack_is_full(&stack)) {
        int* v = (int*)malloc(sizeof(int));
        *v = i;
        hm_stack_push(&stack, v);
        i++;
    }

    if (hm_stack_is_full(&stack)) {
        printf("stack is full\n");
    }

    hm_stack_free(&stack);
    return 0;
}
```

<details>
<summary style="color:red">Run Result</summary>

```txt
stack is empty
stack is full
```
</details>

</details>
<br><br>


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
<details>
<summary style="color:yellow">Try: clear</summary>

```c
#include <hm_stack.h>

#include <stdio.h>
#include <stdlib.h>

void print_stack_status(hm_stack* stack) {
    // print size and capacity of stack
    printf("size: %-3d, capacity: %-3d\n", hm_stack_size(stack), hm_stack_capacity(stack));
}

int main()
{
    int capacity = 20;
    hm_stack stack;
    // fixed-size
    hm_stack_init(&stack, capacity, free);
    
    for (int i = 0; i < capacity; i++) {
        int* v = (int*)malloc(sizeof(int));
        *v = i;
        hm_stack_push(&stack, v);
    }
    print_stack_status(&stack);
    
    // clear
    hm_stack_clear(&stack);
    
    print_stack_status(&stack);

    hm_stack_free(&stack);
    return 0;
}
```

<details>
<summary style="color:red">Run Result</summary>

```txt
size: 20 , capacity: 20 
size: 0  , capacity: 20 
```
</details>

</details>


<details>
<summary style="color:yellow">Try: free</summary>

```c
#include <hm_stack.h>

#include <stdio.h>
#include <stdlib.h>

int main()
{
    int capacity = 20;
    hm_stack stack;
    // fixed-size
    hm_stack_init(&stack, capacity, free);
    
    for (int i = 0; i < capacity; i++) {
        int* v = (int*)malloc(sizeof(int));
        *v = i;
        hm_stack_push(&stack, v);
    }

    // stack must be freed after use
    hm_stack_free(&stack);
    return 0;
}
```

</details>
<br><br>


## Tips