<a id = "head"></a>
# `hm_map` 详细信息

<p align = "center">
    <a href = "../hm_map.md">English</a> | 简体中文
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

- 你可以向此散列表传递任意值和键的指针。
- 它提供基本的散列表操作。


<a id = "func"></a>

## 函数


<a id = "smallfunc"></a>

> **小功能**
```c
#define hm_map_size(l) ((l)->size)
#define hm_map_len(l) ((l)->len)

/**
 * 获取散列表的负载因子
 * @note - 如果散列表的容量为0, 那将返回一个负数
 */
double hm_map_get_load_factor(hm_map* map)
```
<br><br><br>


<a id = "init"></a>

> **初始化**
```c
/**
 * 初始化 hm_map
 * @note - 该函数不仅需要键和值的`释放`函数，还需要`哈希`和`比较`键的函数
 * @note - 与链表类似，`free_key` 和 `free_val` 是可选的(可为 `NULL`)，但 `hash_key` 和 `cmp_key` 不能为 `NULL`
 */
void hm_map_init(hm_map* map, hm_hash hash_key, hm_cmp cmp_key, hm_free free_key, hm_free free_val);
```
<details>
<summary>try: 初始化</summary>

```c
#include <hm_map.h>

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
    hm_map map;
    hm_map_init(&map, hash, cmp, free, free);

    hm_map_free(&map);
    return 0;
}
```
</details>
<br><br><br>


<a id = "insert"></a>

> **插入**
```c
/**
 * 向散列表中插入一个键值对
 * @note - 插入失败时返回 `hm_map_ret_error`
 * @note - 插入成功时返回 `hm_map_ret_suc`
 * @note - 如果键已存在，旧键仍保留在散列表中，函数返回 `hm_map_ret_existed`
 * @note - 因此，如果你的键是动态分配的，建议你自行释放它(仅为建议)
 */
hm_map_ret hm_map_insert(hm_map* map, void* key, void* val);
```


<a id = "get"></a>

> **获取**
```c
/**
 * 获取散列表中条目的指针
 * @note - 如果键不存在，返回 `NULL`
 */
hm_map_entry* hm_map_get(hm_map* map, void* key);
```
<details>
<summary>try: 插入 & 获取</summary>

```c
#include <hm_map.h>

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

void print_map(hm_map* map, int num) {
    // 获取 和 打印
    for (int i = 0; i < num; i++) {
        hm_map_entry* e = hm_map_get(map, &i);
        if (e) {
            int* k = e->key;
            char* v = e->val;
            printf("| k: %d, v: %s\n", *k, v);
        }
    }
    printf("\n");
}

int main() 
{
    hm_map map;
    hm_map_init(&map, hash, cmp, free, NULL);

    for (int i = 0; i < num; i++) {
        int* k = (int*)malloc(sizeof(int));
        *k = i;
        char* v = val[i];
        hm_map_insert(&map, k, v);
    }
    print_map(&map, num);

    // 插入相同的 key
    char* v = "so, why?";
    int* k = (int*)malloc(sizeof(int));
    *k = 5;
    if (hm_map_insert(&map, k, v) == hm_map_ret_existed) {
        // 如果这个 key 已经在 map 中了, 那这个 malloc 来的 key 最好释放掉(新 key 和 旧 key 的 不是同一块内存区域)
        free(k);
    }
    print_map(&map, num);

    hm_map_free(&map);
    return 0;
}
```

<details>
<summary>运行结果</summary>

```txt
| k: 0, v: xl
| k: 1, v: oi
| k: 2, v: i
| k: 3, v: hate
| k: 4, v: love
| k: 5, v: so
| k: 6, v: family

| k: 0, v: xl
| k: 1, v: oi
| k: 2, v: i
| k: 3, v: hate
| k: 4, v: love
| k: 5, v: so, why
| k: 6, v: family

```
</details>

</details>
<br><br><br>


<a id = "del"></a>

> **删除**
```c
/**
 * 根据键删除散列表中的条目
 * @note - 如果键不存在，返回 `hm_map_ret_none`
 */
hm_map_ret hm_map_del(hm_map* map, void* key);
```
<details>
<summary>try: 删除</summary>

```c
#include <hm_map.h>

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

void print_map(hm_map* map, int num) {
    // 获取和打印
    for (int i = 0; i < num; i++) {
        hm_map_entry* e = hm_map_get(map, &i);
        if (e) {
            int* k = e->key;
            char* v = e->val;
            printf("| k: %d, v: %s\n", *k, v);
        }
    }
    printf("\n");
}

int main() 
{
    hm_map map;
    hm_map_init(&map, hash, cmp, free, NULL);

    for (int i = 0; i < num; i++) {
        int* k = (int*)malloc(sizeof(int));
        *k = i;
        char* v = val[i];
        hm_map_insert(&map, k, v);
    }
    print_map(&map, num);

    // 删除 1 3 5
    for (int i = 0; i < num; i++) {
        if (i % 2) {
            hm_map_del(&map, &i);
        }
    }
    print_map(&map, num);
    
    hm_map_free(&map);
    return 0;
}
```

<details>
<summary>运行结果</summary>

```txt
| k: 0, v: xl
| k: 1, v: oi
| k: 2, v: i
| k: 3, v: hate
| k: 4, v: love
| k: 5, v: so
| k: 6, v: family

| k: 0, v: xl
| k: 2, v: i
| k: 4, v: love
| k: 6, v: family

```
</details>

</details>
<br><br><br>


<a id = "shrink"></a>

> **收缩**
```c
/**
 * 如果可以, 对散列表进行缩容
 * @note - 如果无法缩容，返回 `hm_map_ret_none`
 */
hm_map_ret hm_map_shrink(hm_map* map);
```
<details>
<summary>try: 收缩</summary>

```c
#include <hm_map.h>

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

void print_map(hm_map* map, int num) {
    // 获取和打印
    for (int i = 0; i < num; i++) {
        hm_map_entry* e = hm_map_get(map, &i);
        if (e) {
            int* k = e->key;
            char* v = e->val;
            printf("| k: %d, v: %s\n", *k, v);
        }
    }
    printf("\n");
}

void print_load_factor(hm_map* map) {
    printf("| size: %-5zu len: %5zu, load factor: %lf\n", hm_map_size(map), hm_map_len(map), hm_map_get_load_factor(map));
}

int main() 
{
    hm_map map;
    hm_map_init(&map, hash, cmp, free, NULL);

    for (int i = 0; i < num * 1000; i++) {
        int* k = (int*)malloc(sizeof(int));
        *k = i;
        char* v = val[i % num];
        hm_map_insert(&map, k, v);
    }
    print_load_factor(&map);
    
    // 删掉大部分
    for (int i = 0; i < num * 1000 - num * 10; i++) {
        hm_map_del(&map, &i);
    }
    print_load_factor(&map);
    
    printf("\n");
    while (hm_map_shrink(&map) == hm_map_ret_suc) {
        print_load_factor(&map);
    }

    hm_map_free(&map);
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
 * 初始化散列表的迭代器
 */
void hm_map_iter_init(hm_map_iter* iter, hm_map* map);

/**
 * 检查迭代器是否有下一个条目
 * @note - 有下一个条目时返回 `true`
 */
bool hm_map_iter_has_next(hm_map_iter* iter);

/**
 * 通过迭代器获取下一个条目的指针
 * @note - 请先调用 `hm_map_iter_has_next()` 检查是否存在下一个条目
 */
hm_map_entry* hm_map_iter_next(hm_map_iter* iter);
```
<details>
<summary>try: 迭代</summary>

```c
#include <hm_map.h>

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

void print_map(hm_map* map, int num) {
    // 获取和打印
    for (int i = 0; i < num; i++) {
        hm_map_entry* e = hm_map_get(map, &i);
        if (e) {
            int* k = e->key;
            char* v = e->val;
            printf("| k: %d, v: %s\n", *k, v);
        }
    }
    printf("\n");
}

int main() 
{
    hm_map map;
    hm_map_init(&map, hash, cmp, free, NULL);

    for (int i = 0; i < num; i++) {
        int* k = (int*)malloc(sizeof(int));
        *k = i;
        char* v = val[i % num];
        hm_map_insert(&map, k, v);
    }
    print_map(&map, num);

    hm_map_iter iter;
    hm_map_iter_init(&iter, &map);
    while (hm_map_iter_has_next(&iter)) {
        hm_map_entry* e = hm_map_iter_next(&iter);
        int* k = e->key;
        char* v = e->val;
        printf("| k: %d, v: %s\n", *k, v);
    }
    printf("\n");

    hm_map_free(&map);
    return 0;
}
```

<details>
<summary>运行结果</summary>

```txt
| k: 0, v: xl
| k: 1, v: oi
| k: 2, v: i
| k: 3, v: hate
| k: 4, v: love
| k: 5, v: so
| k: 6, v: family

| k: 0, v: xl
| k: 6, v: family
| k: 3, v: hate
| k: 2, v: i
| k: 5, v: so
| k: 1, v: oi
| k: 4, v: love

```

</details>

</details>
<br><br><br>




<a id = "clear"></a>

> **清空**
```c
/**
 * 释放散列表中的键和值，但保留桶和状态标志数组
 */
void hm_map_clear(hm_map* map);
```
<details>
<summary>try: 清空</summary>

```c
#include <hm_map.h>

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

void print_map(hm_map* map) {
    hm_map_iter iter;
    hm_map_iter_init(&iter, map);
    while (hm_map_iter_has_next(&iter)) {
        hm_map_entry* e = hm_map_iter_next(&iter);
        int* k = e->key;
        char* v = e->val;
        printf("| k: %d, v: %s\n", *k, v);
    }
    printf("\n");
}

void print_load_factor(hm_map* map) {
    printf("| size: %-5zu len: %5zu, load factor: %lf\n", hm_map_size(map), hm_map_len(map), hm_map_get_load_factor(map));
}

int main() 
{
    hm_map map;
    hm_map_init(&map, hash, cmp, free, NULL);

    for (int i = 0; i < num; i++) {
        int* k = (int*)malloc(sizeof(int));
        *k = i;
        char* v = val[i % num];
        hm_map_insert(&map, k, v);
    }
    print_map(&map);
    print_load_factor(&map);
    
    // 清空
    hm_map_clear(&map);
    
    print_map(&map);
    print_load_factor(&map);

    hm_map_free(&map);
    return 0;
}
```

<details>
<summary>运行结果</summary>

```txt
| k: 0, v: xl
| k: 6, v: family
| k: 3, v: hate
| k: 2, v: i
| k: 5, v: so
| k: 1, v: oi
| k: 4, v: love

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
 * 释放散列表的所有内容
 */
void hm_map_free(hm_map* map);
```

<details>
<summary>try: 释放</summary>

```c
#include <hm_map.h>

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

int main() 
{
    hm_map map;
    hm_map_init(&map, hash, cmp, free, NULL);

    for (int i = 0; i < num; i++) {
        int* k = (int*)malloc(sizeof(int));
        *k = i;
        char* v = val[i % num];
        hm_map_insert(&map, k, v);
    }
    
    // 散列表在使用完后必须释放掉
    hm_map_free(&map);
    return 0;
}
```


</details>
<br><br><br>



<a id = "tip"></a>

## 提示

- **遍历散列表时，请勿修改散列表结构(如删除、插入、清空或释放)。**








<a id = "othercontainer"></a>

## 其他容器

1. [hm_list](hm_list.zh-CN.md)

2. hm_map

3. [hm_pool](hm_pool.zh-CN.md)

4. [hm_stack](hm_stack.zh-CN.md)

5. [hm_queue](hm_queue.zh-CN.md)

6. [hm_heap](hm_heap.zh-CN.md)

7. [hm_set](hm_set.zh-CN.md)



<br><br><br>
<div align = "right">
    <a href="#head">↑ Top</a>
</div>