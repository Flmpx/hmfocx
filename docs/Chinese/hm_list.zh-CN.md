# `hm_list` 详细信息

<p align = "center">
    <a href = "../hm_list.md">English</a> | 简体中文
</p>


## 介绍

- 你可以向此链表传递任意值的指针。
- 它提供基本的链表操作。

## 函数

### **小功能**
```c
#define hm_list_size(l) ((l)->size)
```
<br><br>


### **初始化**
```c
/**
 * 初始化一个链表
 * @note - 使用 `hm_list_free()` 释放该链表
 * @note - 如果不想让链表释放其值，请将 `hm_free` 函数指针设为 `NULL`
 */
void hm_list_init(hm_list* list, hm_free free);
```
<details>
<summary style="color:yellow">Try: 初始化 </summary>

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
<br><br>


### **插入**
```c
/**
 * 在链表头部插入值
 * @note - 插入失败时返回 `hm_list_ret_error`
 */
hm_list_ret hm_list_insert_head(hm_list* list, void* val);

/**
 * 在链表尾部插入值
 * @note - 插入失败时返回 `hm_list_ret_error`
 */
hm_list_ret hm_list_insert_tail(hm_list* list, void* val);

/**
 * 在指定索引处插入值
 * @note - 索引必须 >= `0` 且 <= `链表大小`
 * @note - 索引无效时返回 `hm_list_ret_warn`
 * @note - 插入失败时返回 `hm_list_ret_error`
 */
hm_list_ret hm_list_insert_index(hm_list* list, void* val, size_t index);
```

### **获取**
```c
/**
 * 根据索引获取值的指针
 * @note - 索引无效时返回`空指针`
 */
void* hm_list_get(hm_list* list, size_t index);
```
<details>
<summary style="color:yellow">Try: 插入 & 获取</summary>

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
    
    hm_list_free(&list);
    return 0;
}
```

<details>
<summary style="color:red">运行结果</summary>

```txt
9 8 7 6 5 4 3 2 1 0 
9 8 7 6 5 4 3 2 1 0 0 1 2 3 4 5 6 7 8 9 
9 8 -1 7 6 5 4 3 2 1 0 0 1 2 3 4 5 6 7 8 9 
```
</details>

</details>
<br><br>


### **删除**
```c
/**
 * 删除链表头节点
 * @note - 链表为空时返回 `hm_list_ret_none`
 */
hm_list_ret hm_list_del_head(hm_list* list);

/**
 * 删除链表尾节点
 * @note - 链表为空时返回 `hm_list_ret_none`
 */
hm_list_ret hm_list_del_tail(hm_list* list);

/**
 * 删除指定索引处的节点
 * @note - 索引必须 >= `0` 且 < `链表大小`
 * @note - 索引无效时返回 `hm_list_ret_none`
 */
hm_list_ret hm_list_del_index(hm_list* list, size_t index);
```
<details>
<summary style="color:yellow">Try: 删除</summary>

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
<summary style="color:red">运行结果</summary>

```txt
0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 
0 1 2 3 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 
3 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 
3 5 6 7 8 9 10 11 12 13 14 15 16 17 
```
</details>

</details>
<br><br>


### **迭代器**
```c
// 这些函数将会被移除

/**
 * 初始化链表迭代器
 */
void hm_iter_list_init(hm_iter_list* iter, hm_list* list);

/**
 * 检查迭代器是否有下一个元素
 * @note - 有下一个元素时返回 `true`
 */
bool hm_iter_list_has_next(hm_iter_list* iter);

/**
 * 获取下一个元素的值
 * @note - 请先调用 `hm_iter_list_has_next()` 检查是否存在下一个值
 */
void* hm_iter_list_next(hm_iter_list* iter);


// 迭代器的新函数

/**
 * 初始化链表的迭代器
 * @note - 让迭代器指向链表头部
 */
void hm_iter_list_init_head(hm_iter_list* iter, hm_list* list);

/**
 * 初始化链表的迭代器
 * @note - 让迭代器指向链表尾部
 */
void hm_iter_list_init_tail(hm_iter_list* iter, hm_list* list);

/**
 * 初始化链表的迭代器
 * @note - 让迭代器指向链表中指定的索引位置
 * @note - 如果索引越界，迭代器将指向 `NULL`
 */
void hm_iter_list_init_index(hm_iter_list* iter, hm_list* list, size_t index);

/**
 * 检查迭代器的当前指向是否有效
 */
bool hm_iter_list_has_cur(hm_iter_list* iter);

/**
 * 获取迭代器的当前值
 * 在调用 `hm_iter_list_cur()` 之前，请先使用 `hm_iter_list_has_cur()` 进行检查
 */
void* hm_iter_list_cur(hm_iter_list* iter);

/**
 * 将迭代器的指针移动到下一个节点
 */
void hm_iter_list_move_next(hm_iter_list* iter);

/**
 * 将迭代器的指针移动到上一个节点
 */
void hm_iter_list_move_prev(hm_iter_list* iter);
```
<details>
<summary style="color:yellow">Try: 迭代器  [旧]</summary>

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
    
    hm_iter_list iter;
    hm_iter_list_init(&iter, &list);
    while (hm_iter_list_has_next(&iter)) {
        int* v = hm_iter_list_next(&iter);
        printf("%d ", *v);
    }
    hm_list_free(&list);
    return 0;
}
```

<details>
<summary style="color:red">运行结果</summary>

```txt
0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 
```
</details>

</details>

<details>
<summary style="color:yellow">Try: 迭代器  [新]</summary>

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
    hm_iter_list iter;

    // 从头开始遍历
    hm_iter_list_init_head(&iter, &list);
    while (hm_iter_list_has_cur(&iter)) {
        int* v = hm_iter_list_cur(&iter);
        printf("%d ", *v);
        // 游标移至后一个节点
        hm_iter_list_move_next(&iter);
    }
    printf("\n");
    
    // 从尾开始遍历 | 反向遍历
    hm_iter_list_init_tail(&iter, &list);
    while (hm_iter_list_has_cur(&iter)) {
        int* v = hm_iter_list_cur(&iter);
        printf("%d ", *v);
        // 游标移至前一个节点
        hm_iter_list_move_prev(&iter);
    }
    printf("\n");
    
    // 从下标为 4 的节点开始遍历
    hm_iter_list_init_index(&iter, &list, 4);
    while (hm_iter_list_has_cur(&iter)) {
        int* v = hm_iter_list_cur(&iter);
        printf("%d ", *v);
        // 游标移至后一个节点
        hm_iter_list_move_next(&iter);
    }
    printf("\n");

    hm_list_free(&list);    
    return 0;
}
```

<details>
<summary style="color:red">运行结果</summary>

```txt
0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 
19 18 17 16 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0 
4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 
```
</details>

</details>
<br><br>


### **释放**
```c
/**
 * 释放链表
 * @note - 如果链表初始化时没有赋予释放能力，此函数无法释放节点中的值
 */
void hm_list_free(hm_list* list);
```
<details>
<summary style="color:yellow">Try: free</summary>

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
<br><br>


### **排序**
```c
/**
 * 对链表排序
 * @note - 传入一个比较函数
 */
void hm_list_sort(hm_list* list, hm_cmp cmp);
```
<details>
<summary style="color:yellow">Try: 排序</summary>

```c
#include <hm_list.h>

#include <stdlib.h>
#include <stdio.h>

void print_list(hm_list* list) {
    hm_iter_list iter;
    hm_iter_list_init_head(&iter, list);
    while (hm_iter_list_has_cur(&iter)) {
        int* v = hm_iter_list_cur(&iter);
        printf("%d ", *v);
        hm_iter_list_move_next(&iter);
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
<summary style="color:red">运行结果</summary>

```txt
11517 31859 16191 3650 6711 3535 9555 7633 30056 28891 
3535 3650 6711 7633 9555 11517 16191 28891 30056 31859 
```
</details>

</details>
<br><br>

## 提示

- **遍历链表时，请勿修改链表结构(如删除、插入、释放或排序)。**