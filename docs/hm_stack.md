<a id = "head"></a>
# The detailed information about `hm_stack`

<p align = "center">
    English | <a href = "./Chinese/hm_stack.zh-CN.md">简体中文</a>
</p>




## Navigation
- [Introduction](#intro)
- [Functions](#func)
    - [Small Functions](#smallfunc)
    - [Initialize](#init)
    - [Push](#push)
    - [Pop](#pop)
    - [Peek](#peek)
    - [Judge](#judge)
    - [Shrink](#shrink)
    - [Clear](#clear)
    - [Free](#free)
- [Tips](#tip)
- [Other Containers](#othercontainer)


<a id = "intro"></a>

## Introduction
- You can pass a pointer to any value into this stack
- It provides basic stack operations
- It supports `dynamic-growth` and `fixed-size` modes


<a id = "func"></a>

## Functions


<a id = "smallfunc"></a>

> **Small Functions**
```c
size_t hm_stack_size(hm_stack* stack);
size_t hm_stack_capacity(hm_stack* stack);
```
<br><br><br>


<a id = "init"></a>

> **Initialize**
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
<summary>try: init</summary>

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
<br><br><br>



<a id = "push"></a>

> **Push**
```c
/**
 * Push a value to the stack
 * @note - Return `hm_stack_ret_full` when stack is full
 * @note - Return `hm_stack_ret_suc` when push succeeds
 * @note - Return `hm_stack_ret_error` when stack is `dynamic-growth` and expansion fails
 */
hm_stack_ret hm_stack_push(hm_stack* stack, void* val);
```


<a id = "pop"></a>

> **Pop**
```c
/**
 * Pop a value from the stack
 * @note - Return `NULL` when the stack is empty
 */
void* hm_stack_pop(hm_stack* stack);
```
</details>


<a id = "peek"></a>

> **Peek**
```c
/**
 * Peek a value from the stack
 * @note - Return `NULL` when the stack is empty 
 */
void* hm_stack_peek(hm_stack* stack);
```


<details>
<summary>try: push & peek & pop</summary>

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
<summary>run result</summary>

```txt
19
19 18 17 16 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0 
```
</details>

</details>
<br><br><br>





<a id = "judge"></a>

> **Judge**
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
<summary>try: judge</summary>

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
<summary>run result</summary>

```txt
stack is empty
stack is full
```
</details>

</details>
<br><br><br>


<a id = "shrink"></a>

> **Shrink**
```c
/**
 * Shrink the capacity of stack if possible
 * @note - Only dynamic-grow stack have a chance to shrink
 * @note - Returns `hm_stack_ret_none` if the stack can't be shrunk
 */
hm_stack_ret hm_stack_shrink(hm_stack* stack);
```
<details>
<summary>try: shrink</summary>

```c
#include <hm_stack.h>

#include <stdlib.h>
#include <stdio.h>

void print_stack_status(hm_stack* stack) {
    printf("size: %zu, capacity: %zu\n", hm_stack_size(stack), hm_stack_capacity(stack));
}

int main()
{
    hm_stack stack;
    int capacity = 520;
    // only dynamic-grow stack can do
    hm_stack_init_dynamic_grow(&stack, capacity, free);
    print_stack_status(&stack);

    while (hm_stack_shrink(&stack) == hm_stack_ret_suc) {
        print_stack_status(&stack);
    }

    hm_stack_free(&stack);
    return 0;
}
```

<details>
<summary>run result</summary>

```txt
size: 0, capacity: 520
size: 0, capacity: 260
size: 0, capacity: 130
size: 0, capacity: 65
size: 0, capacity: 32
size: 0, capacity: 16
size: 0, capacity: 8
size: 0, capacity: 4
size: 0, capacity: 2
size: 0, capacity: 1
```
</details>


</details>


<br><br><br>


<a id = "clear"></a>

> **Clear**
```c
/**
 * Clear the stack 
 * @note - Only free the values(if possible),  but keep the vals array existed
 */
void hm_stack_clear(hm_stack* stack);
```
<details>
<summary>try: clear</summary>

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
<summary>run result</summary>

```txt
size: 20 , capacity: 20 
size: 0  , capacity: 20 
```
</details>

</details>
<br><br><br>

<a id = "free"></a>

> **Free**
```c
/**
 * Free all contents of the stack
 * @note - The stack can be reused when it is `dynamic-growth` but `fixed-size` cannot
 */
void hm_stack_free(hm_stack* stack);
```

<details>
<summary>try: free</summary>

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
<br><br><br>



<a id = "tip"></a>

## Tips






<a id = "othercontainer"></a>

## Other Containers

1. [hm_list](hm_list.md)

2. [hm_map](hm_map.md)

3. [hm_pool](hm_pool.md)

4. hm_stack

5. [hm_queue](hm_queue.md)

6. [hm_heap](hm_heap.md)

7. [hm_set](hm_set.md)



<br><br><br>
<div align = "right">
    <a href="#head">↑ Top</a>
</div>