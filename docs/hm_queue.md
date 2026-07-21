<a id = "head"></a>
# The detailed information about `hm_queue`

<p align = "center">
    English | <a href = "./Chinese/hm_queue.zh-CN.md">简体中文</a>
</p>



## Navigation
- [Introduction](#intro)
- [Functions](#func)
    - [Small Functions](#smallfunc)
    - [Initialize](#init)
    - [Enqueue](#enq)
    - [Dequeue And Peek](#deqpeek)
    - [Judge](#judge)
    - [Clear And Free](#clearfree)
- [Tips](#tip)
- [Other Containers](#othercontainer)



<a id = "intro"></a>

## Introduction
- You can pass a pointer to any value into this queue
- It provides basic queue operations
- It supports `dynamic-growth` and `fixed-size` modes


<a id = "func"></a>

## Functions


<a id = "smallfunc"></a>

> **Small Functions**
```c
#define hm_queue_size(s) ((s)->size)
#define hm_queue_capacity(s) ((s)->capacity)
```
<br><br><br>


<a id = "init"></a>

> **Initialize**
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
<details>
<summary>try: init</summary>

```c
#include <hm_queue.h>

#include <stdio.h>
#include <stdlib.h>

int main()
{
    int capacity = 50;
    hm_queue queue;
    // fixed-size
    hm_queue_init(&queue, capacity, free);
    hm_queue_free(&queue);
    
    // dynamic-grow
    hm_queue_init_dynamic_grow(&queue, capacity, free);
    hm_queue_free(&queue);

    return 0;
}
```


</details>
<br><br><br>



<a id = "enq"></a>

> **Enqueue**
```c
/**
 * Enqueue a value
 * @note - Return `hm_queue_ret_full` when queue is full
 * @note - Return `hm_queue_ret_suc` when enqueue succeeds
 * @note - Return `hm_queue_ret_error` when queue is `dynamic-growth` and expansion fails
 */
hm_queue_ret hm_queue_enq(hm_queue* queue, void* val);
```


<a id = "deqpeek"></a>

> **Dequeue And Peek**
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
<details>
<summary>try: enqueue & peek & dequeue</summary>

```c
#include <hm_queue.h>

#include <stdio.h>
#include <stdlib.h>

int main()
{
    int capacity = 20;
    hm_queue queue;
    // fixed-size
    hm_queue_init(&queue, capacity, free);
    
    // enqueue
    for (int i = 0; i < capacity; i++) {
        int* v = (int*)malloc(sizeof(int));
        *v = i;
        hm_queue_enq(&queue, v);
    }
    
    // peek
    int* val = hm_queue_peek(&queue);
    printf("%d\n", *val);

    // dequeue
    for (int i = 0; i < capacity; i++) {
        int* v = hm_queue_deq(&queue);
        printf("%d ", *v);
        free(v);
    }

    hm_queue_free(&queue);
    return 0;
}
```

<details>
<summary>run result</summary>

```txt
0
0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 
```
</details>

</details>
<br><br><br>




<a id = "judge"></a>

> **Judge**
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
<details>
<summary>try: judge</summary>

```c
#include <hm_queue.h>

#include <stdio.h>
#include <stdlib.h>

int main()
{
    int capacity = 20;
    hm_queue queue;
    // fixed-size
    hm_queue_init(&queue, capacity, free);
    
    if (hm_queue_is_empty(&queue)) {
        printf("queue is empty\n");
    }

    // enqueue
    int i = 0;
    while (!hm_queue_is_full(&queue)) {
        int* v = (int*)malloc(sizeof(int));
        *v = i;
        hm_queue_enq(&queue, v);
        i++;
    }

    if (hm_queue_is_full(&queue)) {
        printf("queue is full\n");
    }

    hm_queue_free(&queue);
    return 0;
}
```

<details>
<summary>run result</summary>

```txt
queue is empty
queue is full
```
</details>

</details>
<br><br><br>



<a id = "clearfree"></a>

> **Clear And Free**
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
<details>
<summary>try: clear</summary>

```c
#include <hm_queue.h>

#include <stdio.h>
#include <stdlib.h>

void print_queue_status(hm_queue* queue) {
    // print size and capacity of queue
    printf("size: %-3d, capacity: %-3d\n", hm_queue_size(queue), hm_queue_capacity(queue));
}

int main()
{
    int capacity = 20;
    hm_queue queue;
    // fixed-size
    hm_queue_init(&queue, capacity, free);
    
    for (int i = 0; i < capacity; i++) {
        int* v = (int*)malloc(sizeof(int));
        *v = i;
        hm_queue_enq(&queue, v);
    }
    print_queue_status(&queue);
    
    // clear
    hm_queue_clear(&queue);
    
    print_queue_status(&queue);

    hm_queue_free(&queue);
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


<details>
<summary>try: free</summary>

```c
#include <hm_queue.h>

#include <stdio.h>
#include <stdlib.h>

int main()
{
    int capacity = 20;
    hm_queue queue;
    // fixed-size
    hm_queue_init(&queue, capacity, free);
    
    for (int i = 0; i < capacity; i++) {
        int* v = (int*)malloc(sizeof(int));
        *v = i;
        hm_queue_enq(&queue, v);
    }

    // queue must be freed after use
    hm_queue_free(&queue);
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

4. [hm_stack](hm_stack.md)

5. [hm_heap](hm_heap.md)



<br><br><br>
<div align = "right">
    <a href="#head">↑ Top</a>
</div>