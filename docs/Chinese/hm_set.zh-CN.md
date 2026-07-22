<a id = "head"></a>
# `hm_set` 详细信息

<p align = "center">
    <a href = "../hm_set.md">English</a> | 简体中文
</p>



## 导航
- [介绍](#intro)
- [函数](#func)
    - [小功能](#smallfunc)
    - [初始化](#init)
    - [插入](#insert)
    - [获取](#get)
    - [迭代器](#iter)
    - [删除](#del)
    - [收缩](#shrink)
    - [清空](#clear)
    - [释放](#free)
- [提示](#tip)
- [其他容器](#othercontainer)

<a id = "intro"></a>

## 介绍

- 你可以向此集合传递任意键的指针。
- 它提供基本的集合操作。


<a id = "func"></a>

## 函数


<a id = "smallfunc"></a>

> **小功能**
```c
#define hm_set_size(s) ((s)->size)
#define hm_set_len(s) ((s)->len)

/**
 * 获取集合的负载因子
 * @note - 如果集合的容量为0, 那将返回一个负数
 */
double hm_set_get_load_factor(hm_set* set)
```
<br><br><br>


<a id = "init"></a>

> **初始化**
```c
/**
 * 初始化 hm_set
 * @note - 该函数不仅需要键的`释放`函数，还需要`哈希`和`比较`键的函数
 * @note - 与链表类似，`free_key` 是可选的(可为 `NULL`)，但 `hash_key` 和 `cmp_key` 不能为 `NULL`
 */
void hm_set_init(hm_set* set, hm_hash hash_key, hm_cmp cmp_key, hm_free free_key);
```
<details>
<summary>try: 初始化</summary>

```c
#include <hm_set.h>

#include <stdlib.h>
#include <stdio.h>

int cmp(const void* p1, const void* p2) {
    int a = *(int*)p1;
    int b = *(int*)p2;
    return (a > b) - (a < b);
}

size_t hash(void* key) {
    unsigned int k = *(int*)key;
    k = ((k >> 16) ^ k) * 0x45d9f3b; 
    k = ((k >> 16) ^ k) * 0x45d9f3b; 
    k = (k >> 16) ^ k;
    return (size_t)k;
}

int main() 
{
    hm_set set;
    hm_set_init(&set, hash, cmp, free);

    hm_set_free(&set);
    return 0;
}
```
</details>
<br><br><br>


<a id = "insert"></a>

> **插入**
```c
/**
 * 向集合中插入一个键
 * @note - 插入失败时返回 `hm_set_ret_error`
 * @note - 插入成功时返回 `hm_set_ret_suc`
 * @note - 如果键已存在，旧键仍保留在集合中，函数返回 `hm_set_ret_existed`
 * @note - 因此，如果你的键是动态分配的，建议你自行释放它(仅为建议)
 */
hm_set_ret hm_set_insert(hm_set* set, void* key, void* val);
```


<a id = "get"></a>

> **获取**
```c
/**
 * 获取集合中条目的指针
 * @note - 如果键不存在，返回 `NULL`
 */
hm_entry* hm_set_get(hm_set* set, void* key);
```
<details>
<summary>try: 插入 & 获取</summary>

```c
#include <hm_set.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int cmp(const void* p1, const void* p2) {
    int a = *(int*)p1;
    int b = *(int*)p2;
    return (a > b) - (a < b);
}

size_t hash(const void* key) {
    unsigned int k = *(int*)key;
    k = ((k >> 16) ^ k) * 0x45d9f3b; 
    k = ((k >> 16) ^ k) * 0x45d9f3b; 
    k = (k >> 16) ^ k;
    return (size_t)k;
}


void print_set(hm_set* set, int num) {
    // 获取和打印
    for (int i = 0; i < num; i++) {
        hm_set_entry* e = hm_set_get(set, &i);
        if (e) {
            int* k = e->key;
            printf("| k: %d\n", *k);
        }
    }
    printf("\n");
}

int main() 
{
    int num = 6;

    hm_set set;
    hm_set_init(&set, hash, cmp, free);

    for (int i = 0; i < num; i++) {
        int* k = (int*)malloc(sizeof(int));
        *k = i;
        hm_set_insert(&set, k);
    }
    print_set(&set, num);

    // 插入相同的键
    int* k = (int*)malloc(sizeof(int));
    *k = 5;
    if (hm_set_insert(&set, k) == hm_set_ret_existed) {
        // 如果这个 key 已经在 map 中了, 那这个 malloc 来的 key 最好释放掉(新 key 和 旧 key 的 不是同一块内存区域)
        free(k);
    }
    print_set(&set, num);

    hm_set_free(&set);
    return 0;
}
```

<details>
<summary>运行结果</summary>

```txt
| k: 0
| k: 1
| k: 2
| k: 3
| k: 4
| k: 5

| k: 0
| k: 1
| k: 2
| k: 3
| k: 4
| k: 5

```
</details>

</details>
<br><br><br>


<a id = "del"></a>

> **删除**
```c
/**
 * 根据键删除集合中的条目
 * @note - 如果键不存在，返回 `hm_set_ret_none`
 */
hm_set_ret hm_set_del(hm_set* set, void* key);
```
<details>
<summary>try: 删除</summary>

```c
#include <hm_set.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int cmp(const void* p1, const void* p2) {
    int a = *(int*)p1;
    int b = *(int*)p2;
    return (a > b) - (a < b);
}

size_t hash(const void* key) {
    unsigned int k = *(int*)key;
    k = ((k >> 16) ^ k) * 0x45d9f3b; 
    k = ((k >> 16) ^ k) * 0x45d9f3b; 
    k = (k >> 16) ^ k;
    return (size_t)k;
}

char* val[] = {"xl", "oi", "i", "hate", "love", "so", "family"};

int num = sizeof(val) / sizeof(char*);

void print_set(hm_set* set, int num) {
    // 获取和打印
    for (int i = 0; i < num; i++) {
        hm_set_entry* e = hm_set_get(set, &i);
        if (e) {
            int* k = e->key;
            printf("| k: %d\n", *k);
        }
    }
    printf("\n");
}

int main() 
{
    hm_set set;
    hm_set_init(&set, hash, cmp, free);

    for (int i = 0; i < num; i++) {
        int* k = (int*)malloc(sizeof(int));
        *k = i;
        hm_set_insert(&set, k);
    }
    print_set(&set, num);

    // 删掉 1 3 5
    for (int i = 0; i < num; i++) {
        if (i % 2) {
            hm_set_del(&set, &i);
        }
    }
    print_set(&set, num);
    
    hm_set_free(&set);
    return 0;
}
```

<details>
<summary>运行结果</summary>

```txt
| k: 0
| k: 1
| k: 2
| k: 3
| k: 4
| k: 5
| k: 6

| k: 0
| k: 2
| k: 4
| k: 6

```
</details>

</details>
<br><br><br>


<a id = "shrink"></a>

> **收缩**
```c
/**
 * 如果可以, 对集合进行缩容
 * @note - 如果无法缩容，返回 `hm_set_ret_none`
 */
hm_set_ret hm_set_shrink(hm_set* set);
```
<details>
<summary>try: 收缩</summary>

```c
#include <hm_set.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int cmp(const void* p1, const void* p2) {
    int a = *(int*)p1;
    int b = *(int*)p2;
    return (a > b) - (a < b);
}

size_t hash(const void* key) {
    unsigned int k = *(int*)key;
    k = ((k >> 16) ^ k) * 0x45d9f3b; 
    k = ((k >> 16) ^ k) * 0x45d9f3b; 
    k = (k >> 16) ^ k;
    return (size_t)k;
}

void print_load_factor(hm_set* set) {
    printf("| size: %-5zu len: %5zu, load factor: %lf\n", hm_set_size(set), hm_set_len(set), hm_set_get_load_factor(set));
}

int main() 
{
    hm_set set;
    hm_set_init(&set, hash, cmp, free);

    int num = 7;
    for (int i = 0; i < num * 1000; i++) {
        int* k = (int*)malloc(sizeof(int));
        *k = i;
        hm_set_insert(&set, k);
    }
    print_load_factor(&set);
    
    // 删掉大部分
    for (int i = 0; i < num * 1000 - num * 10; i++) {
        hm_set_del(&set, &i);
    }
    print_load_factor(&set);
    
    printf("\n");
    while (hm_set_shrink(&set) == hm_set_ret_suc) {
        print_load_factor(&set);
    }

    hm_set_free(&set);
    return 0;
}
```

<details>
<summary>运行结果</summary>

```txt
| size: 7000  len: 10949, load factor: 0.639328
| size: 70    len: 10949, load factor: 0.006393

| size: 70    len:  5477, load factor: 0.012781
| size: 70    len:  2741, load factor: 0.025538
| size: 70    len:  1373, load factor: 0.050983
| size: 70    len:   691, load factor: 0.101302
| size: 70    len:   347, load factor: 0.201729
| size: 70    len:   173, load factor: 0.404624
```
</details>

</details>
<br><br><br>


<a id = "iter"></a>

> **迭代器**
```c
/**
 * 初始化集合的迭代器
 */
void hm_set_iter_init(hm_set_iter* iter, hm_set* set);

/**
 * 检查迭代器是否有下一个条目
 * @note - 有下一个条目时返回 `true`
 */
bool hm_set_iter_has_next(hm_set_iter* iter);

/**
 * 通过迭代器获取下一个条目的指针
 * @note - 请先调用 `hm_set_iter_has_next()` 检查是否存在下一个条目
 */
hm_entry* hm_set_iter_next(hm_set_iter* iter);
```
<details>
<summary>try: 迭代</summary>

```c
#include <hm_set.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int cmp(const void* p1, const void* p2) {
    int a = *(int*)p1;
    int b = *(int*)p2;
    return (a > b) - (a < b);
}

size_t hash(const void* key) {
    unsigned int k = *(int*)key;
    k = ((k >> 16) ^ k) * 0x45d9f3b; 
    k = ((k >> 16) ^ k) * 0x45d9f3b; 
    k = (k >> 16) ^ k;
    return (size_t)k;
}

void print_set(hm_set* set, int num) {
    // 获取和打印
    for (int i = 0; i < num; i++) {
        hm_set_entry* e = hm_set_get(set, &i);
        if (e) {
            int* k = e->key;
            printf("| k: %d\n", *k);
        }
    }
    printf("\n");
}

int main() 
{
    hm_set set;
    hm_set_init(&set, hash, cmp, free);

    int num = 7;
    for (int i = 0; i < num; i++) {
        int* k = (int*)malloc(sizeof(int));
        *k = i;
        hm_set_insert(&set, k);
    }
    print_set(&set, num);

    hm_set_iter iter;
    hm_set_iter_init(&iter, &set);
    while (hm_set_iter_has_next(&iter)) {
        hm_set_entry* e = hm_set_iter_next(&iter);
        int* k = e->key;
        printf("| k: %d\n", *k);
    }
    printf("\n");

    hm_set_free(&set);
    return 0;
}
```

<details>
<summary>运行结果</summary>

```txt
| k: 0
| k: 1
| k: 2
| k: 3
| k: 4
| k: 5
| k: 6

| k: 0
| k: 6
| k: 3
| k: 2
| k: 5
| k: 1
| k: 4

```

</details>

</details>
<br><br><br>




<a id = "clear"></a>

> **清空**
```c
/**
 * 释放集合中的键，但保留桶和状态标志数组
 */
void hm_set_clear(hm_set* set);
```
<details>
<summary>try: 清空</summary>

```c
#include <hm_set.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int cmp(const void* p1, const void* p2) {
    int a = *(int*)p1;
    int b = *(int*)p2;
    return (a > b) - (a < b);
}

size_t hash(const void* key) {
    unsigned int k = *(int*)key;
    k = ((k >> 16) ^ k) * 0x45d9f3b; 
    k = ((k >> 16) ^ k) * 0x45d9f3b; 
    k = (k >> 16) ^ k;
    return (size_t)k;
}

void print_set(hm_set* set) {
    hm_set_iter iter;
    hm_set_iter_init(&iter, set);
    while (hm_set_iter_has_next(&iter)) {
        hm_set_entry* e = hm_set_iter_next(&iter);
        int* k = e->key;
        printf("| k: %d\n", *k);
    }
    printf("\n");
}

void print_load_factor(hm_set* set) {
    printf("| size: %-5zu len: %5zu, load factor: %lf\n", hm_set_size(set), hm_set_len(set), hm_set_get_load_factor(set));
}

int main() 
{
    hm_set set;
    hm_set_init(&set, hash, cmp, free);

    int num = 7;
    for (int i = 0; i < num; i++) {
        int* k = (int*)malloc(sizeof(int));
        *k = i;
        hm_set_insert(&set, k);
    }
    print_set(&set);
    print_load_factor(&set);
    
    // 清空
    hm_set_clear(&set);
    
    print_set(&set);
    print_load_factor(&set);

    hm_set_free(&set);
    return 0;
}
```

<details>
<summary>运行结果</summary>

```txt
| k: 0
| k: 6
| k: 3
| k: 2
| k: 5
| k: 1
| k: 4

| size: 7     len:    17, load factor: 0.411765

| size: 0     len:    17, load factor: 0.000000
```
</details>

</details>
<br><br><br>

<a id = "free"></a>

> **释放**
```c
/**
 * 释放集合的所有内容
 */
void hm_set_free(hm_set* set);
```

<details>
<summary>try: 释放</summary>

```c
#include <hm_set.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int cmp(const void* p1, const void* p2) {
    int a = *(int*)p1;
    int b = *(int*)p2;
    return (a > b) - (a < b);
}

size_t hash(const void* key) {
    unsigned int k = *(int*)key;
    k = ((k >> 16) ^ k) * 0x45d9f3b; 
    k = ((k >> 16) ^ k) * 0x45d9f3b; 
    k = (k >> 16) ^ k;
    return (size_t)k;
}

int main() 
{
    hm_set set;
    hm_set_init(&set, hash, cmp, free);

    int num = 7;
    for (int i = 0; i < num; i++) {
        int* k = (int*)malloc(sizeof(int));
        *k = i;
        hm_set_insert(&set, k);
    }
    
    // 集合在使用完之后必须释放掉
    hm_set_free(&set);
    return 0;
}
```


</details>
<br><br><br>



<a id = "tip"></a>

## 提示

- **遍历集合时，请勿修改集合结构(如删除、插入、清空或释放)。**








<a id = "othercontainer"></a>

## 其他容器

1. [hm_list](hm_list.zh-CN.md)

2. [hm_map](hm_map.zh-CN.md)

3. [hm_pool](hm_pool.zh-CN.md)

4. [hm_stack](hm_stack.zh-CN.md)

5. [hm_queue](hm_queue.zh-CN.md)

6. [hm_heap](hm_heap.zh-CN.md)

7. hm_set



<br><br><br>
<div align = "right">
    <a href="#head">↑ Top</a>
</div>