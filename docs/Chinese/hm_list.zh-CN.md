<a id = "head"></a>
# `hm_list` 详细信息

<p align = "center">
    <a href = "../hm_list.md">English</a> | 简体中文
</p>



## 导航
- [介绍](#intro)
- [函数](#func)
    - [小功能](#smallfunc)
    - [初始化](#init)
    - [插入](#insert)
    - [获取](#get)
    - [弹出](#pop)
    - [迭代器](#iter)
    - [删除](#del)
    - [释放](#free)
    - [排序](#sort)
- [一点小事](#otherthings)
- [其他容器](#othercontainer)




<a id = "intro"></a>

## 介绍

- 你可以向此链表传递任意值的指针
- 它提供基本的链表操作



<a id = "func"></a>

## 函数


>  [!Note]  
>  - 所有的函数都不会检查 `hm_list* & hm_list_iter*` 是否为空指针, 所以你要确保这个指针是有效的
>  - 其他关于指针的参数是否可以为空指针, 具体看下面的函数注释

<br><br><br>


<a id = "smallfunc"></a>

> **小功能**
```c
size_t hm_list_size(hm_list* list);
```
<br><br><br>


<a id = "init"></a>

> **初始化**
```c
/**
 * 初始化一个链表
 * 
 * @note 使用 **hm_list_free()** 释放该链表
 * @note 如果不想让链表释放其值, 将 **free_val** 函数指针设为 **NULL**
 */
void hm_list_init(hm_list* list, hm_free free_val);
```
<details>
<summary>try: 初始化 </summary>

```c
#include <hm_list.h>

#include <stdlib.h>
#include <stdio.h>
int main() 
{
    hm_list list;
    hm_list_init(&list, free);

    hm_list_free(&list);
    return 0;
}
```
</details>  
<br><br><br>


<a id = "insert"></a>

> **插入**
```c
/**
 * 在链表头部插入值
 * 
 * @return 插入失败时返回 **hm_list_ret_error**
 * @return 插入成功时返回 **hm_list_ret_suc**
 */
hm_list_ret hm_list_insert_head(hm_list* list, void* val);

/**
 * 在链表尾部插入值
 * 
 * @return 插入失败时返回 **hm_list_ret_error**
 * @return 插入成功时返回 **hm_list_ret_suc**
 */
hm_list_ret hm_list_insert_tail(hm_list* list, void* val);

/**
 * 在指定索引处插入值
 * 
 * @note 索引必须 >= **0** 并且 <= **链表大小**
 * 
 * @return 索引无效时返回 **hm_list_ret_warn**
 * @return 插入失败时返回 **hm_list_ret_error**
 * @return 插入成功时返回 **hm_list_ret_suc**
 */
hm_list_ret hm_list_insert_index(hm_list* list, void* val, size_t index);
```


<a id = "get"></a>

> **获取**
```c
/**
 * 根据索引获取值的指针
 * 
 * @note 索引必须 >= **0** 并且 < **链表大小**
 * 
 * @return 索引无效时返回**空指针**
 */
void* hm_list_get(hm_list* list, size_t index);

/**
 * 根据索引获取链表节点的指针
 * 
 * @note 索引必须 >= **0** 并且 < **链表大小**
 * 
 * @return 索引无效时返回 **空指针**
 * 
 * @warning 禁止修改 **Node** 的 **prev** 和 **next**
 */
hm_listnode* hm_list_get_node(hm_list* list, size_t index);
```
<details>
<summary>try: 插入 & 获取</summary>

```c
#include <hm_list.h>

#include <stdlib.h>
#include <stdio.h>

void print_list(hm_list* list) {
    int s = list->size;
    // 获取和打印
    for (int i = 0; i < s; i++) {
        int* v = hm_list_get(list, i);
        printf("%d ", *v);
    }
    printf("\n");
}

int main() 
{
    hm_list list;
    // 初始化
    hm_list_init(&list, free);

    int cnt = 10;
    // 头插
    for (int i = 0; i < cnt; i++) {
        int* v = (int*)malloc(sizeof(int));
        *v = i;
        hm_list_insert_head(&list, v);
    }
    print_list(&list);    

    // 尾插
    for (int i = 0; i < cnt; i++) {
        int* v = (int*)malloc(sizeof(int));
        *v = i;
        hm_list_insert_tail(&list, v);
    }
    print_list(&list);

    int* val = (int*)malloc(sizeof(int));
    *val = -1;
    // 在下标为 2 的位置插入值
    hm_list_insert_index(&list, val, 2);
    print_list(&list);

    // 使用get去改变下标为2的值
    int* v = hm_list_get(&list, 3);
    *v = 66666666;
    print_list(&list);
    
    hm_list_free(&list);
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
<summary>try: 插入 & 获取节点</summary>

```c
#include <hm_list.h>

#include <stdlib.h>
#include <stdio.h>

char* vals[] = {"xl", "oi", "i", "hate", "love", "so", "family"};

void print_list(hm_list* list) {
    int s = hm_list_size(list);
    // 获取节点和打印
    for (int i = 0; i < s; i++) {
        hm_listnode* n = hm_list_get_node(list, i);
        char* v = n->val;
        printf("| %d. %s\n", i, v);
    }
    printf("\n");
}

int main() 
{
    hm_list list;
    // 初始化
    hm_list_init(&list, NULL);

    // 尾插
    int cnt = sizeof(vals) / sizeof(char*);
    for (int i = 0; i < cnt; i++) {
        hm_list_insert_tail(&list, vals[i]);
    }
    print_list(&list);

    char* tmp_str = "Hello, I'm Flmpx";
    // 使用 get_node 去改变下标为 3 的值的指针
    hm_listnode* v = hm_list_get_node(&list, 3);
    v->val = tmp_str;
    print_list(&list);
    
    hm_list_free(&list);
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



<a id = "pop"></a>

> **弹出**

```c
/**
 * 在指定索引处弹出一个值
 * 
 * @note 值会被移除但不会释放它所占的内存(内存权转移)
 * @note 索引必须 >= **0** 并且 <= **链表大小**
 * 
 * @return 当索引无效时返回 **NULL**
 */
void* hm_list_pop(hm_list* list, size_t index)
```

<details>
<summary>try: 弹出</summary>

```c
#include <hm_list.h>

#include <stdlib.h>
#include <stdio.h>

void print_list(hm_list* list) {
    int s = list->size;
    for (int i = 0; i < s; i++) {
        int* v = hm_list_get(list, i);
        printf("%d ", *v);
    }
    printf("\n");
}

int main()
{
    hm_list list;
    // 初始化
    hm_list_init(&list, free);

    int cnt = 20;
    // 插入
    for (int i = 0; i < cnt; i++) {
        int* v = (int*)malloc(sizeof(int));
        *v = i;
        hm_list_insert_tail(&list, v);
    }
    print_list(&list);

    // 弹出下标为 4 的元素
    int* pop_v = hm_list_pop(&list, 4);
    print_list(&list);

    // 打印被弹出的值
    printf("pop val: %d\n", *pop_v);

    free(pop_v); // 发生了内存权的交换, 所以你应该释放掉这块内存
    hm_list_free(&list);
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



<a id = "del"></a>

> **删除**
```c
/**
 * 删除链表头节点
 * 
 * @return 链表为空时返回 **hm_list_ret_none**
 * @return 删除成功时返回 **hm_list_ret_suc**
 */
hm_list_ret hm_list_del_head(hm_list* list);

/**
 * 删除链表尾节点
 * 
 * @return 链表为空时返回 **hm_list_ret_none**
 * @return 删除成功时返回 **hm_list_ret_suc**
 */
hm_list_ret hm_list_del_tail(hm_list* list);

/**
 * 删除指定索引处的节点
 * 
 * @note 索引必须 >= **0** 并且 < **链表大小**
 * 
 * @return 索引无效时返回 **hm_list_ret_none**
 * @return 删除成功时返回 **hm_list_ret_suc**
 */
hm_list_ret hm_list_del_index(hm_list* list, size_t index);
```
<details>
<summary>try: 删除</summary>

```c
#include <hm_list.h>

#include <stdlib.h>
#include <stdio.h>

void print_list(hm_list* list) {
    int s = list->size;
    for (int i = 0; i < s; i++) {
        int* v = hm_list_get(list, i);
        printf("%d ", *v);
    }
    printf("\n");
}

int main() 
{
    hm_list list;
    // 初始化
    hm_list_init(&list, free);

    int cnt = 20;
    // 尾插
    for (int i = 0; i < cnt; i++) {
        int* v = (int*)malloc(sizeof(int));
        *v = i;
        hm_list_insert_tail(&list, v);
    }
    print_list(&list);

    // 删除下标为 4 的节点
    hm_list_del_index(&list, 4);
    print_list(&list);

    int num_h = 3;
    // 三次删除头节点
    for (int i = 0; i < num_h; i++) {
        hm_list_del_head(&list);
    }
    print_list(&list);

    int num_t = 2;
    // 两次删除尾节点
    for (int i = 0; i < num_t; i++) {
        hm_list_del_tail(&list);
    }
    print_list(&list);

    hm_list_free(&list);
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


<a id = "iter"></a>

> **迭代器**
```c
// 这些函数将会被移除

/**
 * 初始化链表迭代器
 */
void hm_list_iter_init(hm_list_iter* iter, hm_list* list);

/**
 * 检查迭代器是否有下一个元素
 * 
 * @return 有下一个元素时返回 **true**
 */
bool hm_list_iter_has_next(hm_list_iter* iter);

/**
 * 获取下一个元素的值
 * 
 * @note 在调用 **hm_list_iter_next()** 之前, 先使用 **hm_list_iter_has_next()** 进行检查
 * 
 * @return 没有下一个时就返回 **NULL**
 */
void* hm_list_iter_next(hm_list_iter* iter);


// 迭代器的新函数

/**
 * 初始化链表的迭代器
 * 
 * @note 让迭代器指向链表头部
 */
void hm_list_iter_init_head(hm_list_iter* iter, hm_list* list);

/**
 * 初始化链表的迭代器
 * 
 * @note 让迭代器指向链表尾部
 */
void hm_list_iter_init_tail(hm_list_iter* iter, hm_list* list);

/**
 * 初始化链表的迭代器
 * 
 * @note 让迭代器指向链表中指定的索引位置
 * @note 如果索引越界, 迭代器将指向 **NULL**
 */
void hm_list_iter_init_index(hm_list_iter* iter, hm_list* list, size_t index);

/**
 * 检查迭代器的当前指向是否有效
 * 
 * @return 如果当前指向有效返回 **true**
 */
bool hm_list_iter_has_cur(hm_list_iter* iter);

/**
 * 获取迭代器的当前值
 * 
 * @note 在调用 **hm_list_iter_cur()** 之前, 先使用 **hm_list_iter_has_cur()** 进行检查
 * 
 * @return 当迭代器当前指向无效时返回 **NULL**
 */
void* hm_list_iter_cur(hm_list_iter* iter);

/**
 * 将迭代器的指针移动到下一个节点
 */
void hm_list_iter_move_next(hm_list_iter* iter);

/**
 * 将迭代器的指针移动到上一个节点
 */
void hm_list_iter_move_prev(hm_list_iter* iter);
```
<details>
<summary>try: 迭代器  [旧]</summary>

```c
#include <hm_list.h>

#include <stdlib.h>
#include <stdio.h>
int main() 
{
    hm_list list;
    // 初始化
    hm_list_init(&list, free);


    int cnt = 20;
    // 尾插
    for (int i = 0; i < cnt; i++) {
        int* v = (int*)malloc(sizeof(int));
        *v = i;
        hm_list_insert_tail(&list, v);
    }
    
    // 迭代器  [旧]
    
    hm_list_iter iter;
    hm_list_iter_init(&iter, &list);
    while (hm_list_iter_has_next(&iter)) {
        int* v = hm_list_iter_next(&iter);
        printf("%d ", *v);
    }
    hm_list_free(&list);
    return 0;
}
```

<details>
<summary>运行结果</summary>

```txt
0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 
```
</details>

</details>

<details>
<summary>try: 迭代器  [新]</summary>

```c
#include <hm_list.h>

#include <stdlib.h>
#include <stdio.h>
int main() 
{
    hm_list list;
    // 初始化
    hm_list_init(&list, free);

    int cnt = 20;
    // 尾插
    for (int i = 0; i < cnt; i++) {
        int* v = (int*)malloc(sizeof(int));
        *v = i;
        hm_list_insert_tail(&list, v);
    }
    
    // 迭代器  [新]
    hm_list_iter iter;

    // 从头开始遍历
    hm_list_iter_init_head(&iter, &list);
    while (hm_list_iter_has_cur(&iter)) {
        int* v = hm_list_iter_cur(&iter);
        printf("%d ", *v);
        // 游标移至后一个节点
        hm_list_iter_move_next(&iter);
    }
    printf("\n");
    
    // 从尾开始遍历 | 反向遍历
    hm_list_iter_init_tail(&iter, &list);
    while (hm_list_iter_has_cur(&iter)) {
        int* v = hm_list_iter_cur(&iter);
        printf("%d ", *v);
        // 游标移至前一个节点
        hm_list_iter_move_prev(&iter);
    }
    printf("\n");
    
    // 从下标为 4 的节点开始遍历
    hm_list_iter_init_index(&iter, &list, 4);
    while (hm_list_iter_has_cur(&iter)) {
        int* v = hm_list_iter_cur(&iter);
        printf("%d ", *v);
        // 游标移至后一个节点
        hm_list_iter_move_next(&iter);
    }
    printf("\n");

    hm_list_free(&list);    
    return 0;
}
```

<details>
<summary>运行结果</summary>

```txt
0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 
19 18 17 16 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0 
4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 
```
</details>

</details>
<br><br><br>



<a id = "free"></a>

> **释放**
```c
/**
 * 释放链表
 * 
 * @note 如果链表初始化时没有赋予释放能力, 此函数无法释放节点中的值
 */
void hm_list_free(hm_list* list);
```
<details>
<summary>try: free</summary>

```c
#include <hm_list.h>

#include <stdlib.h>
#include <stdio.h>
int main() 
{
    hm_list list;
    // 初始化
    hm_list_init(&list, free);

    int cnt = 20;
    // 尾插
    for (int i = 0; i < cnt; i++) {
        int* v = (int*)malloc(sizeof(int));
        *v = i;
        hm_list_insert_tail(&list, v);
    }
    
    // 链表在使用之后必须释放掉
    hm_list_free(&list);
    
    return 0;
}
```
</details>
<br><br><br>


<a id = "sort"></a>

> **排序**
```c
/**
 * 对链表排序
 * 
 * @note 传入一个比较函数
 */
void hm_list_sort(hm_list* list, hm_cmp cmp);
```
<details>
<summary>try: 排序</summary>

```c
#include <hm_list.h>

#include <stdlib.h>
#include <stdio.h>

void print_list(hm_list* list) {
    hm_list_iter iter;
    hm_list_iter_init_head(&iter, list);
    while (hm_list_iter_has_cur(&iter)) {
        int* v = hm_list_iter_cur(&iter);
        printf("%d ", *v);
        hm_list_iter_move_next(&iter);
    }
    printf("\n");
}

int cmp_up(const void* p1, const void* p2) {
    int a = *(int*)p1;
    int b = *(int*)p2;
    return (a > b) - (a < b);
}

int main() 
{
    hm_list list;
    // 初始化
    hm_list_init(&list, free);

    // 得到随机数
    int seed = 5201314;
    srand(seed);

    int cnt = 10;
    // 尾插
    for (int i = 0; i < cnt; i++) {
        int* v = (int*)malloc(sizeof(int));
        *v = rand();
        hm_list_insert_tail(&list, v);
    }
    print_list(&list);    

    hm_list_sort(&list, cmp_up);

    print_list(&list);

    hm_list_free(&list);
    return 0;
}
```

<details>
<summary>运行结果</summary>

```txt
11517 31859 16191 3650 6711 3535 9555 7633 30056 28891 
3535 3650 6711 7633 9555 11517 16191 28891 30056 31859 
```
</details>

</details>
<br><br><br>


<a id = "otherthings"></a>

## 一点小事

>  [!Tip]
>  - **遍历链表时, 禁止修改链表结构(如删除、插入、释放或排序)**
>  









<a id = "othercontainer"></a>

## 其他容器

1. hm_list

2. [hm_map](hm_map.zh-CN.md)

3. [hm_pool](hm_pool.zh-CN.md)

4. [hm_stack](hm_stack.zh-CN.md)

5. [hm_queue](hm_queue.zh-CN.md)

6. [hm_heap](hm_heap.zh-CN.md)

7. [hm_set](hm_set.zh-CN.md)

8. [hm_arr](hm_arr.zh-CN.md)



<br><br><br>
<div align = "right">
    <a href="#head">↑ Top</a>
</div>