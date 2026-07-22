<a id = "head"></a>
# The detailed information about `hm_set`

<p align = "center">
    English | <a href = "./Chinese/hm_set.zh-CN.md">简体中文</a>
</p>

## Navigation
- [Introduction](#intro)
- [Functions](#func)
    - [Small Functions](#smallfunc)
    - [Initialize](#init)
    - [Insert](#insert)
    - [Get](#get)
    - [Iterator](#iter)
    - [Del](#del)
    - [Shrink](#shrink)
    - [Clear](#clear)
    - [Free](#free)
- [Tips](#tip)
- [Other Containers](#othercontainer)


<a id = "intro"></a>

## Introduction
- You can pass a pointer to any key into this set
- It provides basic set operations


<a id = "func"></a>

## Functions


<a id = "smallfunc"></a>

> **Small Functions**
```c
#define hm_set_size(s) ((s)->size)
#define hm_set_len(s) ((s)->len)

/**
 * Get the load factor of the set 
 * @note - Return a negative number when the length of the set is 0
 */
double hm_set_get_load_factor(hm_set* set);
```
<br><br><br>


<a id = "init"></a>

> **Initialize**
```c
/**
 * Initialize hm_set
 * @note - This function requires not only `free` function for keys, 
 * but also `hash` and `cmp` functions for keys
 * @note - Like `list`, the `free_key` parameters is optional (can be NULL), 
 * but `hash_key` and `cmp_key` must not be NULL
 */
void hm_set_init(hm_set* set, hm_hash hash_key, hm_cmp cmp_key, hm_free free_key);
```
<details>
<summary>try: init</summary>

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

> **Insert**
```c
/**
 * Insert a key into the set
 * @note - Rreturn `hm_set_ret_error` on failure
 * @note - Return `hm_set_ret_suc` on success
 * @note - If the key already exists, the old key remains in the set. 
 * Therefore, if your key was dynamically allocated, you should free it (optional suggestion)
 */
hm_set_ret hm_set_insert(hm_set* set, void* key);
```



<a id = "get"></a>

> **Get**
```c
/**
 * Get a pointer to the entry in the set
 * @note - If the key does not exist, return `NULL`
 */
hm_set_entry* hm_set_get(hm_set* set, void* key);
```
<details>
<summary>try: insert & get</summary>

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
    // get and print
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

    // insert same key;
    int* k = (int*)malloc(sizeof(int));
    *k = 5;
    if (hm_set_insert(&set, k) == hm_set_ret_existed) {
        // you should free the key if it is existed in set(new key and old key is located at different memory)
        free(k);
    }
    print_set(&set, num);

    hm_set_free(&set);
    return 0;
}
```

<details>
<summary>run result</summary>

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

> **Del**
```c
/**
 * Delete the entry associated with the given key
 * @note - If the key does not exist, returns `hm_set_ret_none`
 */
hm_set_ret hm_set_del(hm_set* set, void* key);
```
<details>
<summary>try: del</summary>

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
    // get and print
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

    // del 1 3 5
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
<summary>run result</summary>

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

> **Shrink**
```c
/**
 * Shrink the length of set if possible
 * @note - Returns `hm_set_ret_none` if the set can't be shrunk
 */
hm_set_ret hm_set_shrink(hm_set* set);
```
<details>
<summary>try: shrink</summary>

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
    
    // del some
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
<summary>run result</summary>

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

> **Iterator**
```c
/**
 * Initialize the iterator of set
 */
void hm_set_iter_init(hm_set_iter* iter, hm_set* set);

/**
 * Check if the iterator has a next entry
 * @note - Return true if the iterator has next
 */
bool hm_set_iter_has_next(hm_set_iter* iter);

/**
 * Get next entry of set
 * @note - Use `hm_set_iter_has_next()` to check before calling `hm_set_iter_next()`
 */
hm_set_entry* hm_set_iter_next(hm_set_iter* iter);
```
<details>
<summary>try: iter</summary>

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
    // get and print
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
<summary>run result</summary>

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

> **Clear**
```c
/**
 * Free the keys in set but keeps the buckets and buckets_status array existed
 */
void hm_set_clear(hm_set* set);
```
<details>
<summary>try: clear</summary>
<br><br><br>



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
    
    // clear
    hm_set_clear(&set);
    
    print_set(&set);
    print_load_factor(&set);

    hm_set_free(&set);
    return 0;
}
```

<details>
<summary>run result</summary>

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

> **Free**
```c
/**
 * Free all contents of the set
 */
void hm_set_free(hm_set* set);
```

<details>
<summary>try: free</summary>

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
    
    // set must be freed after use
    hm_set_free(&set);
    return 0;
}
```


</details>
<br><br><br>



<a id = "tip"></a>

## Tips

- **Do not modify the set(del, insert, clear, or free) while iterating over it**





<a id = "othercontainer"></a>

## Other Containers

1. [hm_list](hm_list.md)

2. [hm_map](hm_map.md)

3. [hm_pool](hm_pool.md)

4. [hm_stack](hm_stack.md)

5. [hm_queue](hm_queue.md)

6. [hm_heap](hm_heap.md)

7. hm_set



<br><br><br>
<div align = "right">
    <a href="#head">↑ Top</a>
</div>