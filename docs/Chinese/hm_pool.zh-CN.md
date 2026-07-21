<a id = "head"></a>
# `hm_pool` 详细信息

<p align = "center">
    <a href = "../hm_pool.md">English</a> | 简体中文
</p>

## 导航
- [介绍](#intro)
- [函数](#func)
    - [小功能](#smallfunc)
    - [初始化](#init)
    - [分配](#allocate)
    - [释放块内存](#freeblock)
    - [释放池](#freepool)
- [提示](#tip)
- [其他容器](#othercontainer)



<a id = "intro"></a>

## 介绍

- 你可以从该内存池中获取固定大小的内存块。
- 它提供基本的固定大小块内存池操作。


<a id = "func"></a>

## 函数


<a id = "smallfunc"></a>

> **小功能**
```c
/**
 * 获取给定内存池的页数
 */
size_t hm_pool_get_pages(hm_pool* pool);

/**
 * 获取内存池中每页的理论字节数
 */
size_t hm_pool_get_bytes_per_page(hm_pool* pool);
```
<br><br><br>


<a id = "init"></a>

> **初始化**
```c
/**
 * 初始化内存池
 * @note - 如果 `blocks_per_page` 或者 `block_size` 为零，`hm_pool_block_allocate` 将返回 `NULL`
 */
void hm_pool_init(hm_pool* pool, size_t block_size, size_t blocks_per_page);
```
<details>
<summary>try: 初始化</summary>

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

> **分配**
```c
/**
 * 获取一个块的指针
 * @note - 分配失败时返回 `NULL`
 * @note - 如果 `blocks_per_page` 或者 `block_size` 为零，`hm_pool_block_allocate` 将返回 `NULL`
 */
void* hm_pool_block_allocate(hm_pool* pool);
```
<details>
<summary>try: 分配</summary>

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
    // 打印
    for (int i = 0; i < num; i++) {
        printf("%d ", *vals[i]);
    }

    hm_pool_free(&pool);
    return 0;
}
```

<details>
<summary>运行结果</summary>

```txt
0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63 64 65 66 67 68 69 70 71 72 73 74 75 76 77 78 79 80 81 82 83 84 85 86 87 88 89 90 91 92 93 94 95 96 97 98 99  
```
</details>

</details>
<br><br><br>



<a id = "freeblock"></a>

> **释放块内存**
```c
/**
 * 释放一个块
 * @note - 块的指针必须属于该内存池
 */
void hm_pool_block_free(hm_pool* pool, void* block);
```
<details>
<summary>try: 释放块内存</summary>

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
    // 释放块
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

> **释放池**
```c
/**
 * 释放内存池的所有内容
 * @note - 释放后请勿使用来自该内存池的块
 */
void hm_pool_free(hm_pool* pool);
```


<details>
<summary>try: 释放池</summary>

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
    // 分配
    for (int i = 0; i < num; i++) {
        int* v = (int*)hm_pool_block_allocate(&pool);
        *v = i;
        vals[i] = v;
    }
    
    // 在使用完内存池之后必须释放掉
    hm_pool_free(&pool);
    return 0;
}
```


</details>
<br><br><br>




<a id = "tip"></a>

## 提示








<a id = "othercontainer"></a>

## 其他容器

1. [hm_list](hm_list.zh-CN.md)

2. [hm_map](hm_map.zh-CN.md)

3. hm_pool

4. [hm_stack](hm_stack.zh-CN.md)

5. [hm_queue](hm_queue.zh-CN.md)

6. [hm_heap](hm_heap.zh-CN.md)

6. [hm_set](hm_set.zh-CN.md)




<br><br><br>
<div align = "right">
    <a href="#head">↑ Top</a>
</div>