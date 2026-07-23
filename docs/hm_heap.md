<a id = "head"></a>
# The detailed information about `hm_heap`

<p align = "center">
    English | <a href = "./Chinese/hm_heap.zh-CN.md">简体中文</a>
</p>

## Navigation
- [Introduction](#intro)
- [Functions](#func)
    - [Small Functions](#smallfunc)
    - [Initialize](#init)
    - [Insert](#insert)
    - [Extract](#extract)
    - [Peek](#peek)
    - [Judge](#judge)
    - [Shrink](#shrink)
    - [Build](#build)
    - [Rebuild](#rebuild)
    - [Clear](#clear)
    - [Free](#free)
- [Tips](#tip)
- [Other Containers](#othercontainer)



<a id = "intro"></a>

## Introduction

- You can pass a pointer to any value into this heap
- It provides basic heap operations
- It supports `dynamic-growth` and `fixed-size` modes
- It supports sort by your `cmp` function
- It supports build by your vals array

<a id = "func"></a>

## Functions


<a id = "smallfunc"></a>

> **Small Functions**

```c
size_t hm_heap_size(hm_heap* heap);
size_t hm_heap_capacity(hm_heap* heap);
```
<br><br><br>

<a id = "init"></a>

> **Initialize**

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
<details>
<summary>try: init</summary>

```c
#include <hm_heap.h>

#include <stdio.h>
#include <stdlib.h>

int cmp_up(const void* p1, const void* p2) {
    int a = *(int*)p1;
    int b = *(int*)p2;
    return (a > b) - (a < b);
}

int main()
{
    int capacity = 50;
    hm_heap heap;
    // fixed-size
    hm_heap_init(&heap, capacity, free, cmp_up);
    hm_heap_free(&heap);
    
    // dynamic-grow
    hm_heap_init_dynamic_grow(&heap, capacity, free, cmp_up);
    hm_heap_free(&heap);

    return 0;
}
```

</details>
<br><br><br>


<a id = "insert"></a>

> **Insert**

```c
/**
 * Insert a value in the heap
 * @note - Return `hm_heap_ret_full` when heap is full
 * @note - Return `hm_heap_ret_suc` when insert succeeds
 * @note - Return `hm_heap_ret_error` when heap is `dynamic-growth` and expansion fails
 */
hm_heap_ret hm_heap_insert(hm_heap* heap, void* val);
```

<a id = "extract"></a>

> **Extract**

```c
/**
 * Extract a value 
 * @note - Return `NULL` when the heap is empty
 */
void* hm_heap_extract(hm_heap* heap);
```

<a id = "peek"></a>

> **Peek**

```c
/**
 * Peek a value
 * @note - Return `NULL` when the heap is empty 
 */
void* hm_heap_peek(hm_heap* heap);
```


<details>
<summary>try: insert & peek & extract</summary>

```c
#include <hm_heap.h>

#include <stdio.h>
#include <stdlib.h>

int cmp_up(const void* p1, const void* p2) {
    int a = *(int*)p1;
    int b = *(int*)p2;
    return (a > b) - (a < b);
}

int main()
{
    int capacity = 12;

    // get random number
    int seed = 5201314;
    srand(seed);

    hm_heap heap;
    // fixed-size
    hm_heap_init(&heap, capacity, free, cmp_up);
    
    // insert
    for (int i = 0; i < capacity; i++) {
        int* v = (int*)malloc(sizeof(int));
        *v = rand();
        printf("%d ", *v);
        hm_heap_insert(&heap, v);
    }
    printf("\n");
    
    // peek
    int* val = hm_heap_peek(&heap);
    printf("%d\n", *val);

    // extract
    for (int i = 0; i < capacity; i++) {
        int* v = hm_heap_extract(&heap);
        printf("%d ", *v);
        free(v);
    }

    hm_heap_free(&heap);
    return 0;
}
```

<details>
<summary>run result</summary>

```txt
11517 31859 16191 3650 6711 3535 9555 7633 30056 28891 26505 14682 
3535
3535 3650 6711 7633 9555 11517 14682 16191 26505 28891 30056 31859 
```
</details>

</details>
<br><br><br>


<a id = "judge"></a>

> **Judge**

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
<details>
<summary>try: judge</summary>

```c
#include <hm_heap.h>

#include <stdio.h>
#include <stdlib.h>

int cmp_up(const void* p1, const void* p2) {
    int a = *(int*)p1;
    int b = *(int*)p2;
    return (a > b) - (a < b);
}

int main()
{
    int capacity = 20;
    hm_heap heap;
    // fixed-size
    hm_heap_init(&heap, capacity, free, cmp_up);
    
    if (hm_heap_is_empty(&heap)) {
        printf("heap is empty\n");
    }

    // insert
    int i = 0;
    while (!hm_heap_is_full(&heap)) {
        int* v = (int*)malloc(sizeof(int));
        *v = i;
        hm_heap_insert(&heap, v);
        i++;
    }

    if (hm_heap_is_full(&heap)) {
        printf("heap is full\n");
    }

    hm_heap_free(&heap);
    return 0;
}
```

<details>
<summary>run result</summary>

```txt
heap is empty
heap is full
```
</details>

</details>
<br><br><br>


<a id = "shrink"></a>

> **Shrink**

```c
/**
 * Shrink the capacity of heap if possible
 * @note - Only dynamic-grow heap have a chance to shrink
 * @note - Return `hm_heap_ret_none` if the heap can't be shrunk
 */
hm_heap_ret hm_heap_shrink(hm_heap* heap);
```
<br><br><br>



<a id = "build"></a>

> **Build**

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
```
<details>
<summary>try: build</summary>

```c
#include <hm_heap.h>

#include <stdio.h>
#include <stdlib.h>

int cmp_up(const void* p1, const void* p2) {
    int a = *(int*)p1;
    int b = *(int*)p2;
    return (a > b) - (a < b);
}

int main()
{
    // get random number
    int seed = 9420;
    srand(seed);

    int capacity = 12;
    hm_heap heap;
    
    void** vals = (void**)malloc(sizeof(void*) * capacity);
    for (int i = 0; i < capacity; i++) {
        int* v = (int*)malloc(sizeof(int));
        *v = rand();
        printf("%d ", *v);
        vals[i] = v;
    }
    printf("\n");

    // fixed-size
    hm_heap_build(&heap, vals, capacity, capacity, free, cmp_up);
    for (int i = 0; i < capacity; i++) {
        int* v = hm_heap_extract(&heap);
        printf("%d ", *v);
        free(v);
    }
    printf("\n");

    hm_heap_free(&heap);
    return 0;
}
```

<details>
<summary>run result</summary>

```txt
30800 4587 5285 22596 15026 6520 294 15244 32703 16870 21601 17036 
294 4587 5285 6520 15026 15244 16870 17036 21601 22596 30800 32703 
```
</details>

</details>
<br><br><br>



<a id = "rebuild"></a>

> **Rebuild**

```c
/**
 * Rebuild heap by the new cmp function
 */
void hm_heap_rebuild(hm_heap* heap, hm_cmp new_cmp);
```


<details>
<summary>try: rebuild</summary>

```c
#include <hm_heap.h>

#include <stdio.h>
#include <stdlib.h>

const int seed = 9420;
const int capacity = 12;

int cmp_up(const void* p1, const void* p2) {
    int a = *(int*)p1;
    int b = *(int*)p2;
    return (a > b) - (a < b);
}

int cmp_down(const void* p1, const void* p2) {
    int a = *(int*)p1;
    int b = *(int*)p2;
    return (a < b) - (a > b);
}

void heap_insert(hm_heap* heap) {
    // get same and random number
    srand(seed);
    for (int i = 0; i < capacity; i++) {
        int* v = (int*)malloc(sizeof(int));
        *v = rand();
        hm_heap_insert(heap, v);
    }
}

void print_heap_extract(hm_heap* heap) {
    for (int i = 0; i < capacity; i++) {
        int* v = hm_heap_extract(heap);
        printf("%d ", *v);
        free(v);
    }
}

int main()
{
    hm_heap heap;
    
    hm_heap_init(&heap, capacity, free, cmp_up);

    srand(seed);
    // print original number
    for (int i = 0; i < capacity; i++) {
        printf("%d ", rand());
    }
    printf("\n");
    
    heap_insert(&heap);
    print_heap_extract(&heap);
    printf("\n");
    
    heap_insert(&heap);
    // rebuild
    hm_heap_rebuild(&heap, cmp_down);
    print_heap_extract(&heap);
    printf("\n");


    hm_heap_free(&heap);
    return 0;
}
```

<details>
<summary>run result</summary>

```txt
30800 4587 5285 22596 15026 6520 294 15244 32703 16870 21601 17036 
294 4587 5285 6520 15026 15244 16870 17036 21601 22596 30800 32703 
32703 30800 22596 21601 17036 16870 15244 15026 6520 5285 4587 294 
```
</details>

</details>
<br><br><br>


<a id = "clear"></a>

> **Clear**

```c
/**
 * Clear the heap 
 * @note - Only free the values(if possible),  but keep the vals array existed
 */
void hm_heap_clear(hm_heap* heap);
```
<details>
<summary>try: clear</summary>

```c
#include <hm_heap.h>

#include <stdio.h>
#include <stdlib.h>

int cmp_up(const void* p1, const void* p2) {
    int a = *(int*)p1;
    int b = *(int*)p2;
    return (a > b) - (a < b);
}

void print_heap_status(hm_heap* heap) {
    // print size and capacity of heap
    printf("size: %-3d, capacity: %-3d\n", hm_heap_size(heap), hm_heap_capacity(heap));
}

int main()
{
    int capacity = 20;
    hm_heap heap;
    // fixed-size
    hm_heap_init(&heap, capacity, free, cmp_up);
    
    for (int i = 0; i < capacity; i++) {
        int* v = (int*)malloc(sizeof(int));
        *v = i;
        hm_heap_insert(&heap, v);
    }
    print_heap_status(&heap);
    
    // clear
    hm_heap_clear(&heap);
    
    print_heap_status(&heap);

    hm_heap_free(&heap);
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
 * Free all contents of the heap
 * @note - The heap can be reused when it is `dynamic-growth` but `fixed-size` cannot
 */
void hm_heap_free(hm_heap* heap);
```

<details>
<summary>try: free</summary>

```c
#include <hm_heap.h>

#include <stdio.h>
#include <stdlib.h>

int cmp_up(const void* p1, const void* p2) {
    int a = *(int*)p1;
    int b = *(int*)p2;
    return (a > b) - (a < b);
}

int main()
{
    int capacity = 20;
    hm_heap heap;
    // fixed-size
    hm_heap_init(&heap, capacity, free, cmp_up);
    
    for (int i = 0; i < capacity; i++) {
        int* v = (int*)malloc(sizeof(int));
        *v = i;
        hm_heap_insert(&heap, v);
    }

    // heap must be freed after use
    hm_heap_free(&heap);
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

5. [hm_queue](hm_queue.md)


6. hm_heap

7. [hm_set](hm_set.md)



<br><br><br>
<div align = "right">
    <a href="#head">↑ Top</a>
</div>