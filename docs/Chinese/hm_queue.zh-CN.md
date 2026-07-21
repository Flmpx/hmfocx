# `hm_queue` 详细信息

<p align = "center">
    <a href = "../hm_queue.md">English</a> | 简体中文
</p>




## 导航
- [介绍](#intro)
- [函数](#func)
    - [小功能](#smallfunc)
    - [初始化](#init)
    - [入队](#enq)
    - [出队与查看](#deqpeek)
    - [判断](#judge)
    - [清空与释放](#clearfree)
- [提示](#tip)


<a id = "intro"></a>

## 介绍
- 你可以向此队列传递任意值的指针。
- 它提供基本的队列操作。
- 支持动态增长和固定大小两种模式。


<a id = "func"></a>

## 函数


<a id = "smallfunc"></a>

> **小功能**
```c
#define hm_queue_size(s) ((s)->size)
#define hm_queue_capacity(s) ((s)->capacity)
```
<br><br><br>



<a id = "init"></a>

> **初始化**
```c
/**
 * 初始化队列（固定大小模式）
 * @note - 使用参数 `capacity` 设置队列的大小
 * @note - 初始化失败时返回 `hm_queue_ret_error`
 * @note - 初始化成功时返回 `hm_queue_ret_suc`
 * @note - 如果不想让队列释放其值，请将 `hm_free` 函数指针设为 `NULL`
 */
hm_queue_ret hm_queue_init(hm_queue* queue, size_t capacity, hm_free free);

/**
 * 初始化队列（动态增长模式）
 * @note - 使用参数 `start_capacity` 设置队列的初始大小
 * @note - 初始化失败时返回 `hm_queue_ret_error`
 * @note - 初始化成功时返回 `hm_queue_ret_suc`
 * @note - 如果不想让队列释放其值，请将 `hm_free` 函数指针设为 `NULL`
 */
hm_queue_ret hm_queue_init_dynamic_grow(hm_queue* queue, size_t start_capacity, hm_free free);
```
<details>
<summary>try: 初始化</summary>

```c
#include <hm_queue.h>

#include <stdio.h>
#include <stdlib.h>

int main()
{
    int capacity = 50;
    hm_queue queue;
    // 固定大小模式
    hm_queue_init(&queue, capacity, free);
    hm_queue_free(&queue);
    
    // 动态增长模式
    hm_queue_init_dynamic_grow(&queue, capacity, free);
    hm_queue_free(&queue);

    return 0;
}
```


</details>
<br><br><br>



<a id = "enq"></a>

> **入队**
```c
/**
 * 将一个值入队
 * @note - 队列满时返回 `hm_queue_ret_full`
 * @note - 入队成功时返回 `hm_queue_ret_suc`
 * @note - 若队列为动态增长模式且扩容失败，返回 `hm_queue_ret_error`
 */
hm_queue_ret hm_queue_enq(hm_queue* queue, void* val);
```


<a id = "deqpeek"></a>

> **出队与查看**
```c
/**
 * 从队列前端出队一个值
 * @note - 队列为空时返回 `NULL`
 */
void* hm_queue_deq(hm_queue* queue);

/**
 * 查看队列前端的值
 * @note - 队列为空时返回 `NULL`
 */
void* hm_queue_peek(hm_queue* queue);
```
<details>
<summary>try: 入队 & 查看 & 出队</summary>

```c
#include <hm_queue.h>

#include <stdio.h>
#include <stdlib.h>

int main()
{
    int capacity = 20;
    hm_queue queue;
    // 国定大小模式
    hm_queue_init(&queue, capacity, free);
    
    // 入队
    for (int i = 0; i < capacity; i++) {
        int* v = (int*)malloc(sizeof(int));
        *v = i;
        hm_queue_enq(&queue, v);
    }
    
    // 查看
    int* val = hm_queue_peek(&queue);
    printf("%d\n", *val);

    // 出队
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
<summary>运行结果</summary>

```txt
0
0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 
```
</details>

</details>
<br><br><br>



<a id = "judge"></a>

> **判断**
```c
/**
 * 检查队列是否已满
 */
bool hm_queue_is_full(hm_queue* queue);

/**
 * 检查队列是否为空
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
    // 固定大小模式
    hm_queue_init(&queue, capacity, free);
    
    if (hm_queue_is_empty(&queue)) {
        printf("queue is empty\n");
    }

    // 入队
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
<summary>运行结果</summary>

```txt
queue is empty
queue is full
```
</details>

</details>
<br><br><br>


<a id = "clearfree"></a>

> **清空与释放**
```c
/**
 * 清空队列
 * @note - 仅释放值（如果可以），内部数组仍然保留
 */
void hm_queue_clear(hm_queue* queue);

/**
 * 释放队列的所有内容
 * @note - 动态增长模式的队列可重复使用，而固定大小模式的队列不可以
 */
void hm_queue_free(hm_queue* queue);
```
<details>
<summary>try: 清空</summary>

```c
#include <hm_queue.h>

#include <stdio.h>
#include <stdlib.h>

void print_queue_status(hm_queue* queue) {
    // 打印队列的 size 和capacity
    printf("size: %-3d, capacity: %-3d\n", hm_queue_size(queue), hm_queue_capacity(queue));
}

int main()
{
    int capacity = 20;
    hm_queue queue;
    // 固定大小模式
    hm_queue_init(&queue, capacity, free);
    
    for (int i = 0; i < capacity; i++) {
        int* v = (int*)malloc(sizeof(int));
        *v = i;
        hm_queue_enq(&queue, v);
    }
    print_queue_status(&queue);
    
    // 清空
    hm_queue_clear(&queue);
    
    print_queue_status(&queue);

    hm_queue_free(&queue);
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
#include <hm_queue.h>

#include <stdio.h>
#include <stdlib.h>

int main()
{
    int capacity = 20;
    hm_queue queue;
    // 固定大小模式
    hm_queue_init(&queue, capacity, free);
    
    for (int i = 0; i < capacity; i++) {
        int* v = (int*)malloc(sizeof(int));
        *v = i;
        hm_queue_enq(&queue, v);
    }

    // 在使用完队列后必须释放掉
    hm_queue_free(&queue);
    return 0;
}
```

</details>
<br><br><br>



<a id = "tip"></a>

## 提示