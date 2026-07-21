<a id = "head"></a>
# The detailed information about `hm_pool`

<p align = "center">
    English | <a href = "./Chinese/hm_pool.zh-CN.md">简体中文</a>
</p>



## Navigation
- [Introduction](#intro)
- [Functions](#func)
    - [Small Functions](#smallfunc)
    - [Initialize](#init)
    - [Allocate](#allocate)
    - [Free Block](#freeblock)
    - [Free Pool](#freepool)
- [Tips](#tip)
- [Other Containers](#othercontainer)


<a id = "intro"></a>

## Introduction
- You can get fixed-size memory block from this memory pool
- It provides basic fixed-size block memory pool operations


<a id = "func"></a>

## Functions


<a id = "smallfunc"></a>

> **Small Functions**
```c
/**
 * Get the number of pages in the given memory pool
 * 
 */
size_t hm_pool_get_pages(hm_pool* pool);

/**
 * Get the theoretical number of bytes per page in the memory pool
 * 
 */
size_t hm_pool_get_bytes_per_page(hm_pool* pool);
```
<br><br><br>



<a id = "init"></a>

> **Initialize**
```c
/**
 * Initialize the memory pool 
 * @note - `hm_pool_block_allocate` will return `NULL` if `blocks_per_page` is `zero` or `block_size` is `zero`
 */
void hm_pool_init(hm_pool* pool, size_t block_size, size_t blocks_per_page);
```
<details>
<summary>try: init</summary>

```c
#include <hm_pool.h>

#include <stdio.h>
#include <stdlib.h>

int main()
{
    hm_pool pool;
    int blocks_per_page = 24;
    hm_pool_init(&pool, sizeof(int), blocks_per_page);
    
    hm_pool_free(&pool);
    return 0;
}
```


</details>
<br><br><br>



<a id = "allocate"></a>

> **Allocate**
```c
/**
 * Get pointer of to block
 * @note - It will return `NULL` when allocation fails
 * @note - It will return `NULL` if `blocks_per_page` is `zero` or `block_size` is `zero`
 */
void* hm_pool_block_allocate(hm_pool* pool);
```
<details>
<summary>try: allocate</summary>

```c
#include <hm_pool.h>

#include <stdio.h>
#include <stdlib.h>

int main()
{
    hm_pool pool;
    int blocks_per_page = 24;
    hm_pool_init(&pool, sizeof(int), blocks_per_page);
    
    int num = 100;
    int* vals[num];
    for (int i = 0; i < num; i++) {
        int* v = (int*)hm_pool_block_allocate(&pool);
        *v = i;
        vals[i] = v;
    }
    // print
    for (int i = 0; i < num; i++) {
        printf("%d ", *vals[i]);
    }

    hm_pool_free(&pool);
    return 0;
}
```

<details>
<summary>run result</summary>

```txt
0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63 64 65 66 67 68 69 70 71 72 73 74 75 76 77 78 79 80 81 82 83 84 85 86 87 88 89 90 91 92 93 94 95 96 97 98 99  
```
</details>

</details>
<br><br><br>



<a id = "freeblock"></a>

> **Free Block**
```c
/**
 * Free a block
 * @note - The pointer of block must match the memory pool
 */
void hm_pool_block_free(hm_pool* pool, void* block);
```
<details>
<summary>try: free block</summary>

```c
#include <hm_pool.h>

#include <stdio.h>
#include <stdlib.h>

int main()
{
    hm_pool pool;
    int blocks_per_page = 24;
    hm_pool_init(&pool, sizeof(int), blocks_per_page);
    
    int num = 100;
    int* vals[num];
    for (int i = 0; i < num; i++) {
        int* v = (int*)hm_pool_block_allocate(&pool);
        *v = i;
        vals[i] = v;
    }
    // free block
    for (int i = 0; i < num; i++) {
        hm_pool_block_free(&pool, vals[i]);
        vals[i] = NULL;
    }
    
    hm_pool_free(&pool);
    return 0;
}
```
</details>
<br><br><br>

<a id = "freepool"></a>

> **Free Pool**
```c
/**
 * Free all content of the memory pool
 * @note Do not use blocks from a freed memory pool
 */
void hm_pool_free(hm_pool* pool);
```

<details>
<summary>try: free pool</summary>

```c
#include <hm_pool.h>

#include <stdio.h>
#include <stdlib.h>

int main()
{
    hm_pool pool;
    int blocks_per_page = 24;
    hm_pool_init(&pool, sizeof(int), blocks_per_page);
    
    int num = 100;
    int* vals[num];
    // allocate
    for (int i = 0; i < num; i++) {
        int* v = (int*)hm_pool_block_allocate(&pool);
        *v = i;
        vals[i] = v;
    }
    
    // pool must be freed after use
    hm_pool_free(&pool);
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

3. hm_pool

4. [hm_stack](hm_stack.md)

5. [hm_queue](hm_queue.md)

6. [hm_heap](hm_heap.md)



<br><br><br>
<div align = "right">
    <a href="#head">↑ Top</a>
</div>