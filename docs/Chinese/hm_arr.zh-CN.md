<a id = "head"></a>
# `hm_arr` 详细信息

<p align = "center">
    <a href = "../hm_arr.md">English</a> | 简体中文
</p>


## 导航
- [介绍](#intro)
- [函数](#func)
    - [小功能](#smallfunc)
    - [初始化](#init)
    - [插入](#insert)
    - [获取](#get)
    - [删除](#del)
    - [弹出](#pop)
    - [判断](#judge)
    - [缩容](#shrink)
    - [清空](#clear)
    - [释放](#free)
- [一点小事](#otherthings)
- [其他容器](#othercontainer)


<a id = "intro"></a>

## 介绍
- 你可以向此数组传递任意值的指针
- 它提供基本的数组操作
- 支持动态增长和固定大小两种模式

<a id = "func"></a>

## 函数


>  [!Note]  
>  - 所有的函数都不会检查 `hm_arr*` 是否为空指针, 所以你要确保这个指针是有效的
>  - 其他关于指针的参数是否可以为空指针, 具体看下面的函数注释

<br><br><br>



<a id = "smallfunc"></a>

> **小功能**

```c
size_t hm_arr_size(hm_arr* arr);
size_t hm_arr_capacity(hm_arr* arr);
```

<br><br><br>


<a id = "init"></a>

> **初始化**

```c
/**
 * 初始化数组 (固定大小模式)
 * 
 * @note 使用参数 **capacity** 设置数组的容量
 * @note 如果不想让数组释放其值, 将 **free_val** 函数指针设为 **NULL**
 * 
 * @return 初始化失败时返回 **hm_arr_ret_error**
 * @return 初始化成功时返回 **hm_arr_ret_suc**
 */
hm_arr_ret hm_arr_init(hm_arr* arr, size_t capacity, hm_free free_val);

/**
 * 初始化数组 (动态增长模式)
 * 
 * @note 使用参数 **start_capacity** 设置数组的初始容量
 * @note 如果不想让数组释放其值, 将 **free_val** 函数指针设为 **NULL**
 * 
 * @return 初始化失败时返回 **hm_arr_ret_error**
 * @return 初始化成功时返回 **hm_arr_ret_suc**
 */
hm_arr_ret hm_arr_init_dynamic_grow(hm_arr* arr, size_t start_capacity, hm_free free_val);
```

<details>
<summary>try: 初始化</summary>

```c
#include <hm_arr.h>

#include <stdio.h>
#include <stdlib.h>

int main()
{
    int capacity = 50;
    hm_arr arr;
    // 固定大小模式
    hm_arr_init(&arr, capacity, free);
    hm_arr_free(&arr);
    
    // 动态增长模式
    hm_arr_init_dynamic_grow(&arr, capacity, free);
    hm_arr_free(&arr);

    return 0;
}
```

</details>
<br><br><br>


<a id = "insert"></a>

> **插入**

```c
/**
 * 在数组头部插入值
 * 
 * @return 数组满时返回 **hm_arr_ret_full**
 * @return 插入成功时返回 **hm_arr_ret_suc**
 * @return 若数组尾动态增长模式并且扩容失败时返回 **hm_arr_ret_error**
 */
hm_arr_ret hm_arr_insert_head(hm_arr* arr, void* val);

/**
 * 在数组尾部插入值
 * 
 * @return 数组满时返回 **hm_arr_ret_full**
 * @return 插入成功时返回 **hm_arr_ret_suc**
 * @return 若数组尾动态增长模式并且扩容失败时返回 **hm_arr_ret_error**
 */
hm_arr_ret hm_arr_insert_tail(hm_arr* arr, void* val);

/**
 * 在指定索引处插入值
 * 
 * @note 索引必须 >= **0** 并且 <= **数组大小**
 * 
 * @return 索引无效时返回 **hm_arr_ret_warn**
 * @return 数组满时返回 **hm_arr_ret_full**
 * @return 插入成功时返回 **hm_arr_ret_suc**
 * @return 若数组尾动态增长模式并且扩容失败时返回 **hm_arr_ret_error**
 */
hm_arr_ret hm_arr_insert_index(hm_arr* arr, void* val, size_t index);
```


<a id = "get"></a>

> **获取**

```c
/**
 * 根据索引获取值的指针
 * 
 * @note 索引必须 >= **0** 并且 < **数组大小**
 * 
 * @return 索引无效时返回 **NULL**
 */
void* hm_arr_get(hm_arr* arr, size_t index);

/**
 * 根据索引获取值的指针的指针
 * 
 * @note 使用这个指针可以改变 **指向值的指针**
 * @note 索引必须 >= **0** 并且 < **数组大小**
 * 
 * @return 索引无效时返回 **NULL**
 */
void** hm_arr_get_pointer(hm_arr* arr, size_t index);
```


<details>
<summary>try: 插入 & 获取</summary>

```c
#include <hm_arr.h>

#include <stdlib.h>
#include <stdio.h>

void print_arr(hm_arr* arr) {
    int s = hm_arr_size(arr);
    // 获取和打印
    for (int i = 0; i < s; i++) {
        int* v = hm_arr_get(arr, i);
        printf("%d ", *v);
    }
    printf("\n");
}

int main() 
{
    hm_arr arr;
    // 初始化
    int capacity = 30;
    // 固定大小模式
    hm_arr_init(&arr, capacity, free);

    int cnt = 10;
    // 头插
    for (int i = 0; i < cnt; i++) {
        int* v = (int*)malloc(sizeof(int));
        *v = i;
        hm_arr_insert_head(&arr, v);
    }
    print_arr(&arr);    

    // 尾插
    for (int i = 0; i < cnt; i++) {
        int* v = (int*)malloc(sizeof(int));
        *v = i;
        hm_arr_insert_tail(&arr, v);
    }
    print_arr(&arr);

    int* val = (int*)malloc(sizeof(int));
    *val = -1;
    // 将值插入下标为 2 的位置
    hm_arr_insert_index(&arr, val, 2);
    print_arr(&arr);


    // 使用get去改变下标为2的值
    int* v = hm_arr_get(&arr, 3);
    *v = 66666666;
    print_arr(&arr);
    
    hm_arr_free(&arr);
    return 0;
}
```

<details>
<summary>运行结果</summary>

```txt
9 8 7 6 5 4 3 2 1 0 
9 8 7 6 5 4 3 2 1 0 0 1 2 3 4 5 6 7 8 9 
9 8 -1 7 6 5 4 3 2 1 0 0 1 2 3 4 5 6 7 8 9 
9 8 -1 66666666 6 5 4 3 2 1 0 0 1 2 3 4 5 6 7 8 9
```

</details>

</details>


<details>
<summary>try: 插入 & 获取指针</summary>

```c
#include <hm_arr.h>

#include <stdlib.h>
#include <stdio.h>

char* vals[] = {"xl", "oi", "i", "hate", "love", "so", "family"};

void print_arr(hm_arr* arr) {
    int s = hm_arr_size(arr);
    // 获取指针和打印
    for (int i = 0; i < s; i++) {
        char** v = (char**)hm_arr_get_pointer(arr, i);
        printf("| %d. %s\n", i, *v);
    }
    printf("\n");
}

int main() 
{
    hm_arr arr;
    // 初始化
    int capacity = 30;
    hm_arr_init(&arr, capacity, NULL);

    // 插入
    int cnt = sizeof(vals) / sizeof(char*);
    for (int i = 0; i < cnt; i++) {
        hm_arr_insert_tail(&arr, vals[i]);
    }
    print_arr(&arr);


    char* tmp_str = "Hello, I'm Flmpx";
    // 使用 get_pointer 去改变下标为 3 的值的指针
    char** v = (char**)hm_arr_get_pointer(&arr, 3);
    *v = tmp_str;
    print_arr(&arr);
    
    hm_arr_free(&arr);
    return 0;
}
```

<details>
<summary>运行结果</summary>

```txt
| 0. xl
| 1. oi
| 2. i
| 3. hate
| 4. love
| 5. so
| 6. family

| 0. xl
| 1. oi
| 2. i
| 3. Hello, I'm Flmpx
| 4. love
| 5. so
| 6. family

```

</details>

</details>
<br><br><br>



<a id = "del"></a>

> **删除**

```c
/**
 * 删除数组头部的值
 * 
 * @return 数组为空时返回 **hm_arr_ret_none**
 * @return 删除成功时返回 **hm_arr_ret_suc**
 */
hm_arr_ret hm_arr_del_head(hm_arr* arr);

/**
 * 删除数组的尾部的值
 * 
 * @return 数组为空时返回 **hm_arr_ret_none**
 * @return 删除成功时返回 **hm_arr_ret_suc**
 */
hm_arr_ret hm_arr_del_tail(hm_arr* arr);

/**
 * 删除数组指定索引处的值
 * 
 * @note 索引必须 >= **0** 并且 < **数组大小**
 * 
 * @return 索引无效或者数组为空时返回 **hm_arr_ret_none**
 * @return 删除成功时返回 **hm_arr_ret_suc**
 */
hm_arr_ret hm_arr_del_index(hm_arr* arr, size_t index);
```

<details>
<summary>try: 删除</summary>

```c
#include <hm_arr.h>

#include <stdlib.h>
#include <stdio.h>

void print_arr(hm_arr* arr) {
    int s = arr->size;
    for (int i = 0; i < s; i++) {
        int* v = hm_arr_get(arr, i);
        printf("%d ", *v);
    }
    printf("\n");
}

int main() 
{
    hm_arr arr;
    // 初始化
    int capacity = 20;
    // 固定大小模式
    hm_arr_init(&arr, capacity, free);

    // 插满
    for (int i = 0; i < capacity; i++) {
        int* v = (int*)malloc(sizeof(int));
        *v = i;
        hm_arr_insert_tail(&arr, v);
    }
    print_arr(&arr);

    // 删除下标为 4 的节点
    hm_arr_del_index(&arr, 4);
    print_arr(&arr);

    int num_h = 3;
    // 三次删除起始位置
    for (int i = 0; i < num_h; i++) {
        hm_arr_del_head(&arr);
    }
    print_arr(&arr);

    int num_t = 2;
    // 两次删除末位置
    for (int i = 0; i < num_t; i++) {
        hm_arr_del_tail(&arr);
    }
    print_arr(&arr);

    hm_arr_free(&arr);
    return 0;
}
```

<details>
<summary>运行结果</summary>

```txt
0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 
0 1 2 3 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 
3 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 
3 5 6 7 8 9 10 11 12 13 14 15 16 17 
```

</details>

</details>
<br><br><br>




<a id = "pop"></a>

> **弹出**

```c
/**
 * 从数组指定索引处弹出一个值
 * 
 * @note 值会被移除但不会释放它所占的内存(内存权转移)
 * @note 索引必须 >= **0** 并且 < **数组大小**
 * 
 * @return 当索引无效时返回 **NULL**
 */
void* hm_arr_pop(hm_arr* arr, size_t index);
```

<details>
<summary>try: 弹出</summary>

```c
#include <hm_arr.h>

#include <stdlib.h>
#include <stdio.h>

void print_arr(hm_arr* arr) {
    int s = arr->size;
    for (int i = 0; i < s; i++) {
        int* v = hm_arr_get(arr, i);
        printf("%d ", *v);
    }
    printf("\n");
}

int main()
{
    hm_arr arr;
    // 初始化
    int capacity = 20;
    // 固定大小模式
    hm_arr_init(&arr, capacity, free);

    // 插满
    for (int i = 0; i < capacity; i++) {
        int* v = (int*)malloc(sizeof(int));
        *v = i;
        hm_arr_insert_tail(&arr, v);
    }
    print_arr(&arr);

    // 弹出下标为 4 的元素
    int* pop_v = hm_arr_pop(&arr, 4);
    print_arr(&arr);

    // 打印被弹出的值
    printf("pop val: %d\n", *pop_v);

    free(pop_v); // 发生了内存权的交换, 所以你应该释放掉这块内存
    hm_arr_free(&arr);
    return 0;
}
```

<details>
<summary>运行结果</summary>

```txt
0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 
0 1 2 3 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 
pop val: 4
```

</details>

</details>
<br><br><br>




<a id = "judge"></a>

> **判断**

```c
/**
 * 判断数组是否已满
 */
bool hm_arr_is_full(hm_arr* arr);

/**
 * 判断数组是否为空
 */
bool hm_arr_is_empty(hm_arr* arr);
```

<details>
<summary>try: 判断</summary>

```c
#include <hm_arr.h>

#include <stdio.h>
#include <stdlib.h>

int main()
{
    int capacity = 20;
    hm_arr arr;
    // 固定大小模式
    hm_arr_init(&arr, capacity, free);
    
    if (hm_arr_is_empty(&arr)) {
        printf("arr is empty\n");
    }

    // 插入
    int i = 0;
    while (!hm_arr_is_full(&arr)) {
        int* v = (int*)malloc(sizeof(int));
        *v = i;
        hm_arr_insert_tail(&arr, v);
        i++;
    }

    if (hm_arr_is_full(&arr)) {
        printf("arr is full\n");
    }

    hm_arr_free(&arr);
    return 0;
}
```

<details>
<summary>运行结果</summary>

```txt
arr is empty
arr is full
```

</details>

</details>
<br><br><br>




<a id = "shrink"></a>

> **缩容**

```c
/**
 * 如果可以, 对数组进行缩容
 * 
 * @note 只用动态增长的数组有机会缩容
 * 
 * @return 如果缩容成功, 返回 **hm_arr_ret_suc**
 * @return 如果数组无法缩容, 返回 **hm_arr_ret_none**
 * @return 如果缩容失败, 返回 **hm_arr_ret_error**
 */
hm_arr_ret hm_arr_shrink(hm_arr* arr);
```

<details>
<summary>try: 缩容</summary>

```c
#include <hm_arr.h>

#include <stdlib.h>
#include <stdio.h>

void print_arr_status(hm_arr* arr) {
    printf("size: %zu, capacity: %zu\n", hm_arr_size(arr), hm_arr_capacity(arr));
}

int main()
{
    hm_arr arr;
    int capacity = 520;
    // 只用动态增长的数组才可以做到
    hm_arr_init_dynamic_grow(&arr, capacity, free);
    print_arr_status(&arr);

    while (hm_arr_shrink(&arr) == hm_arr_ret_suc) {
        print_arr_status(&arr);
    }

    hm_arr_free(&arr);
    return 0;
}
```

<details>
<summary>运行结果</summary>

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

> **清空**

```c
/**
 * 清空数组
 * 
 * @note 仅释放值 (如果可以), 内部数组仍然保留
 */
void hm_arr_clear(hm_arr* arr);
```

<details>
<summary>try: 清空</summary>

```c
#include <hm_arr.h>

#include <stdio.h>
#include <stdlib.h>

void print_arr_status(hm_arr* arr) {
    // 打印数组的 size 和 capacity
    printf("size: %-3zu, capacity: %-3zu\n", hm_arr_size(arr), hm_arr_capacity(arr));
}

int main()
{
    int capacity = 20;
    hm_arr arr;
    // 固定大小模式
    hm_arr_init(&arr, capacity, free);
    
    for (int i = 0; i < capacity; i++) {
        int* v = (int*)malloc(sizeof(int));
        *v = i;
        hm_arr_insert_tail(&arr, v);
    }
    print_arr_status(&arr);
    
    // 清空
    hm_arr_clear(&arr);
    
    print_arr_status(&arr);

    hm_arr_free(&arr);
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
<br><br><br>




<a id = "free"></a>

> **释放**

```c
/**
 * 释放数组的所有内容
 * 
 * @note 动态增长模式的数组可重复使用, 但固定大小模式的数组不可以
 */
void hm_arr_free(hm_arr* arr);
```

<details>
<summary>try: 释放</summary>

```c
#include <hm_arr.h>

#include <stdio.h>
#include <stdlib.h>

int main()
{
    int capacity = 20;
    hm_arr arr;
    // 固定大小模式
    hm_arr_init(&arr, capacity, free);
    
    for (int i = 0; i < capacity; i++) {
        int* v = (int*)malloc(sizeof(int));
        *v = i;
        hm_arr_insert_tail(&arr, v);
    }

    // 在使用完数组后必须释放掉
    hm_arr_free(&arr);
    return 0;
}
```

</details>
<br><br><br>

<a id = "otherthings"></a>

## 一点小事

>  [!Tip]
>  - 注意 **数组大小** 和 **数组容量** 的区别, 前者是数组中元素的个数, 后者是可容纳元素的总容量
>

<br>

>  [!Tip]
>  - **返回值的指针的指针**: 传入的是指针(值的指针), 内部容器会有区域存储这个指针, 返回的是这个区域的指针, 可用于修改指向, 但注意要自行管理内存
>  





<a id = "othercontainer"></a>

## 其他容器

1. [hm_list](hm_list.zh-CN.md)

2. [hm_map](hm_map.zh-CN.md)

3. [hm_pool](hm_pool.zh-CN.md)

4. [hm_stack](hm_stack.zh-CN.md)

5. [hm_queue](hm_queue.zh-CN.md)

6. [hm_heap](hm_heap.zh-CN.md)

7. [hm_set](hm_set.zh-CN.md)

8. hm_arr

9. [hm_str](hm_str.zh-CN.md)

<br><br><br>
<div align = "right">
    <a href="#head">↑ Top</a>
</div>
