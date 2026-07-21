# `hm_heap` 详细信息

<p align = "center">
    <a href = "../hm_heap.md">English</a> | 简体中文
</p>


## 导航
- [介绍](#intro)
- [函数](#func)
    - [小功能](#smallfunc)
    - [初始化](#init)
    - [插入](#insert)
    - [取出和查看](#extractpeek)
    - [判断](#judge)
    - [构建](#build)
    - [清空与释放](#clearfree)
- [提示](#tip)


<a id = "intro"></a>

## 介绍
- 你可以向此堆传递任意值的指针。
- 它提供基本的堆操作。
- 支持动态增长和固定大小两种模式。
- 支持通过自定义 `cmp` 函数进行排序。
- 支持通过传入的数组构建堆。


<a id = "func"></a>

## 函数


<a id = "smallfunc"></a>

> **小功能**
```c
#define hm_heap_size(s) ((s)->size)
#define hm_heap_capacity(s) ((s)->capacity)
```
<br><br><br>


<a id = "init"></a>

> **初始化**
```c
/**
 * 初始化堆（固定大小模式）
 * @note - 使用参数 capacity 设置堆的容量
 * @note - 初始化失败时返回 hm_heap_ret_error
 * @note - 初始化成功时返回 hm_heap_ret_suc
 * @note - 如果不想让堆释放其值，请将 hm_free 函数指针设为 NULL
 * @note - hm_cmp 函数指针不能为 NULL
 */
hm_heap_ret hm_heap_init(hm_heap* heap, size_t capacity, hm_free free, hm_cmp cmp);

/**
 * 初始化堆（动态增长模式）
 * @note - 使用参数 start_capacity 设置堆的初始容量
 * @note - 初始化失败时返回 hm_heap_ret_error
 * @note - 初始化成功时返回 hm_heap_ret_suc
 * @note - 如果不想让堆释放其值，请将 hm_free 函数指针设为 NULL
 * @note - hm_cmp 函数指针不能为 NULL
 */
hm_heap_ret hm_heap_init_dynamic_grow(hm_heap* heap, size_t start_capacity, hm_free free, hm_cmp cmp);
```
<details>
<summary>try: 初始化</summary>

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
    // 固定大小模式
    hm_heap_init(&heap, capacity, free, cmp_up);
    hm_heap_free(&heap);
    
    // 动态增长模式
    hm_heap_init_dynamic_grow(&heap, capacity, free, cmp_up);
    hm_heap_free(&heap);

    return 0;
}
```

</details>
<br><br><br>




<a id = "insert"></a>

> **插入**
```c
/**
 * 向堆中插入一个值
 * @note - 堆满时返回 hm_heap_ret_full
 * @note - 插入成功时返回 hm_heap_ret_suc
 * @note - 若堆为动态增长模式且扩容失败，返回 hm_heap_ret_error
 */
hm_heap_ret hm_heap_insert(hm_heap* heap, void* val);
```


<a id = "extractpeek"></a>

> **取出与查看**
```c
/**
 * 从堆中取出一个值
 * @note - 堆为空时返回 NULL
 */
void* hm_heap_extract(hm_heap* heap);

/**
 * 查看堆顶的值
 * @note - 堆为空时返回 NULL
 */
void* hm_heap_peek(hm_heap* heap);
```
<details>
<summary>try: 插入 & 查看 & 取出</summary>

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

    // 得到随机数
    int seed = 5201314;
    srand(seed);

    hm_heap heap;
    // 固定大小模式
    hm_heap_init(&heap, capacity, free, cmp_up);
    
    // 插入
    for (int i = 0; i < capacity; i++) {
        int* v = (int*)malloc(sizeof(int));
        *v = rand();
        printf("%d ", *v);
        hm_heap_insert(&heap, v);
    }
    printf("\n");
    
    // 查看
    int* val = hm_heap_peek(&heap);
    printf("%d\n", *val);

    // 取出
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
<summary>运行结果</summary>

```txt
11517 31859 16191 3650 6711 3535 9555 7633 30056 28891 26505 14682 
3535
3535 3650 6711 7633 9555 11517 14682 16191 26505 28891 30056 31859 
```
</details>

</details>
<br><br><br>



<a id = "judge"></a>

> **判断**
```c
/**
 * 检查堆是否已满
 */
bool hm_heap_is_full(hm_heap* heap);

/**
 * 检查堆是否为空
 */
bool hm_heap_is_empty(hm_heap* heap);
```
<details>
<summary>try: 判断</summary>

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
    // 固定大小模式
    hm_heap_init(&heap, capacity, free, cmp_up);
    
    if (hm_heap_is_empty(&heap)) {
        printf("heap is empty\n");
    }

    // 插入
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
<summary>运行结果</summary>

```txt
heap is empty
heap is full
```
</details>

</details>
<br><br><br>



<a id = "build"></a>

> **构建**
```c
/**
 * 通过传入的值数组和相关参数构建堆（固定大小模式）
 * @warning - vals 应位于系统的堆内存中
 * @note - 当 `size > capacity` 返回 `hm_heap_ret_warn` 
 */
hm_heap_ret hm_heap_build(hm_heap* heap, void** vals, size_t size, size_t capacity, hm_free free, hm_cmp cmp);

/**
 * 通过传入的值数组和相关参数构建堆（动态增长模式）
 * @warning - vals 应位于系统的堆内存中
 * @note - 当 `size > capacity` 返回 `hm_heap_ret_warn` 
 */
hm_heap_ret hm_heap_build_dynamic_grow(hm_heap* heap, void** vals, size_t size, size_t capacity, hm_free free, hm_cmp cmp);

/**
 * 使用新的 cmp 函数重建堆
 */
void hm_heap_rebuild(hm_heap* heap, hm_cmp new_cmp);
```
<details>
<summary>try: 构建</summary>

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
    // 得到随机数
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

    // 固定大小模式
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
<summary>运行结果</summary>

```txt
30800 4587 5285 22596 15026 6520 294 15244 32703 16870 21601 17036 
294 4587 5285 6520 15026 15244 16870 17036 21601 22596 30800 32703 
```
</details>

</details>


<details>
<summary>try: 重建</summary>

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
    // 得到相同的随机数
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
    // 打印最开始的数字
    for (int i = 0; i < capacity; i++) {
        printf("%d ", rand());
    }
    printf("\n");
    
    heap_insert(&heap);
    print_heap_extract(&heap);
    printf("\n");
    
    heap_insert(&heap);
    // 重建
    hm_heap_rebuild(&heap, cmp_down);
    print_heap_extract(&heap);
    printf("\n");


    hm_heap_free(&heap);
    return 0;
}
```

<details>
<summary>运行结果</summary>

```txt
30800 4587 5285 22596 15026 6520 294 15244 32703 16870 21601 17036 
294 4587 5285 6520 15026 15244 16870 17036 21601 22596 30800 32703 
32703 30800 22596 21601 17036 16870 15244 15026 6520 5285 4587 294 
```
</details>

</details>
<br><br><br>


<a id = "clearfree"></a>

> **清空与释放**
```c
/**
 * 清空堆
 * @note - 仅释放值（如果可以），内部数组仍然保留
 */
void hm_heap_clear(hm_heap* heap);

/**
 * 释放堆的所有内容
 * @note - 动态增长模式的堆可重复使用，而固定大小模式的堆不可以
 */
void hm_heap_free(hm_heap* heap);
```
<details>
<summary>try: 清空</summary>

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
    // 打印堆的 size 和 capacity
    printf("size: %-3d, capacity: %-3d\n", hm_heap_size(heap), hm_heap_capacity(heap));
}

int main()
{
    int capacity = 20;
    hm_heap heap;
    // 固定大小模式
    hm_heap_init(&heap, capacity, free, cmp_up);
    
    for (int i = 0; i < capacity; i++) {
        int* v = (int*)malloc(sizeof(int));
        *v = i;
        hm_heap_insert(&heap, v);
    }
    print_heap_status(&heap);
    
    // 清空
    hm_heap_clear(&heap);
    
    print_heap_status(&heap);

    hm_heap_free(&heap);
    return 0;
}
```

<details>
<summary>运行结果</summary>

```txt
size: 20 , capacity: 20 
size: 0  , capacity: 20 
```
</details>

</details>

<details>
<summary>try: 释放</summary>

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
    // 固定大小模式
    hm_heap_init(&heap, capacity, free, cmp_up);
    
    for (int i = 0; i < capacity; i++) {
        int* v = (int*)malloc(sizeof(int));
        *v = i;
        hm_heap_insert(&heap, v);
    }

    // 堆在使用完后必须释放掉
    hm_heap_free(&heap);
    return 0;
}
```

</details>
<br><br><br>


<a id = "tip"></a>

## 提示