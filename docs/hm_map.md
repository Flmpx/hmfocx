<a id = "head"></a>
# The detailed information about `hm_map`

<p align = "center">
    English | <a href = "./Chinese/hm_map.zh-CN.md">简体中文</a>
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
- You can pass a pointer to any value and key into this map
- It provides basic map operations


<a id = "func"></a>

## Functions


<a id = "smallfunc"></a>

> **Small Functions**
```c
#define hm_map_size(l) ((l)->size)
#define hm_map_len(l) ((l)->len)

/**
 * Get the load factor of the map 
 * @note - Return a negative number when the length of the map is 0
 */
double hm_map_get_load_factor(hm_map* map);
```
<br><br><br>


<a id = "init"></a>

> **Initialize**
```c
/**
 * Initialize hm_map
 * @note - This function requires not only `free` function for key and value, 
 * but also `hash` and `cmp` functions for keys
 * @note - Like `list`, the `free_key` and `free_val` parameters are optional (can be NULL), 
 * but `hash_key` and `cmp_key` must not be NULL
 */
void hm_map_init(hm_map* map, hm_hash hash_key, hm_cmp cmp_key, hm_free free_key, hm_free free_val);
```
<details>
<summary>try: init</summary>

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

> **Insert**
```c
/**
 * Insert a key-value pair into the map
 * @note - Rreturn `hm_map_ret_error` on failure
 * @note - Return `hm_map_ret_suc` on success
 * @note - If the key already exists, the old key remains in the map. 
 * Therefore, if your key was dynamically allocated, you should free it (optional suggestion)
 */
hm_map_ret hm_map_insert(hm_map* map, void* key, void* val);
```



<a id = "get"></a>

> **Get**
```c
/**
 * Get a pointer to the  entry in the map
 * @note - If the key does not exist, return `NULL`
 */
hm_entry* hm_map_get(hm_map* map, void* key);
```
<details>
<summary>try: insert & get</summary>

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
    // get and print
    for (int i = 0; i < num; i++) {
        hm_entry* e = hm_map_get(map, &i);
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

    // insert same key;
    char* v = "so, why?";
    int* k = (int*)malloc(sizeof(int));
    *k = 5;
    if (hm_map_insert(&map, k, v) == hm_map_ret_existed) {
        // you should free the key if it is existed in map(new key and old key is located at different memory)
        free(k);
    }
    print_map(&map, num);

    hm_map_free(&map);
    return 0;
}
```

<details>
<summary>run result</summary>

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

> **Del**
```c
/**
 * Delete the entry associated with the given key
 * @note - If the key does not exist, returns `hm_map_ret_none`
 */
hm_map_ret hm_map_del(hm_map* map, void* key);
```
<details>
<summary>try: del</summary>

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
    // get and print
    for (int i = 0; i < num; i++) {
        hm_entry* e = hm_map_get(map, &i);
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

    // del 1 3 5
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
<summary>run result</summary>

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

> **Shrink**
```c
/**
 * Shrink the length of map if possible
 * @note - Returns `hm_map_ret_none` if the map can't be shrunk
 */
hm_map_ret hm_map_shrink(hm_map* map);
```
<details>
<summary>try: shrink</summary>

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
    // get and print
    for (int i = 0; i < num; i++) {
        hm_entry* e = hm_map_get(map, &i);
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
    
    // del all
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
 * Initialize the iterator of map
 */
void hm_map_iter_init(hm_map_iter* iter, hm_map* map);

/**
 * Check if the iterator has a next entry
 * @note - Return true if the iterator has next
 */
bool hm_map_iter_has_next(hm_map_iter* iter);

/**
 * Get next entry of map
 * @note - Use `hm_map_iter_has_next()` to check before calling `hm_map_iter_next()`
 */
hm_entry* hm_map_iter_next(hm_map_iter* iter);
```
<details>
<summary>try: iter</summary>

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
    // get and print
    for (int i = 0; i < num; i++) {
        hm_entry* e = hm_map_get(map, &i);
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
        hm_entry* e = hm_map_iter_next(&iter);
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
<summary>run result</summary>

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

> **Clear**
```c
/**
 * Free the keys and values in map but keeps the buckets and buckets_status array existed
 */
void hm_map_clear(hm_map* map);
```
<details>
<summary>try: clear</summary>
<br><br><br>



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
        hm_entry* e = hm_map_iter_next(&iter);
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
    
    // clear
    hm_map_clear(&map);
    
    print_map(&map);
    print_load_factor(&map);

    hm_map_free(&map);
    return 0;
}
```

<details>
<summary>run result</summary>

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

> **Free**
```c
/**
 * Free all contents of the map
 */
void hm_map_free(hm_map* map);
```

<details>
<summary>try: free</summary>

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
    
    // map must be freed after use
    hm_map_free(&map);
    return 0;
}
```


</details>
<br><br><br>



<a id = "tip"></a>

## Tips

- **Do not modify the map(del, insert, clear, or free) while iterating over it**





<a id = "othercontainer"></a>

## Other Containers

1. [hm_list](hm_list.md)

2. hm_map

3. [hm_pool](hm_pool.md)

4. [hm_stack](hm_stack.md)

5. [hm_queue](hm_queue.md)

6. [hm_heap](hm_heap.md)

7. [hm_set](hm_set.md)



<br><br><br>
<div align = "right">
    <a href="#head">↑ Top</a>
</div>