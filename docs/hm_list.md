# The detailed information about `hm_list`

<p align = "center">
    English | <a href = "./Chinese/hm_list.zh-CN.md">简体中文</a>
</p>

## Introduction
- You can pass a pointer to any value into this list
- It provides basic list operations

## Functions

> **Small Functions**
```c
#define hm_list_size(l) ((l)->size)
```
<br><br><br>


> **Initialize**
```c
/**
 * Initialize a list
 * @note - Use `hm_list_free()` to free the list
 * @note - If you do `NOT` want the list to free its values,
 * set the `hm_free` function pointer to `NULL`
 */
void hm_list_init(hm_list* list, hm_free free);
```
<details>
<summary>try: init </summary>

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


> **Insert**
```c
/**
 * Insert a value at the head of the list
 * @note - This function will return `hm_list_ret_error` when insertion fails
 */
hm_list_ret hm_list_insert_head(hm_list* list, void* val);

/**
 * Insert a value at the tail of the list
 * @note - This function will return `hm_list_ret_error` when insertion fails
 */
hm_list_ret hm_list_insert_tail(hm_list* list, void* val);

/**
 * Insert a value at the given `index`
 * @note - `Index` must be >= `0`, and <= `the size of list`
 * 
 * @note - This function will return `hm_list_ret_warn` when the `index` is out of bounds
 * 
 * @note - Also, this function will return `hm_list_ret_error` when insertion fails
 */
hm_list_ret hm_list_insert_index(hm_list* list, void* val, size_t index);
```

> **Get**
```c
/**
 * Get a pointer to the value at the given `index`
 * @note - `Index` must be >= `0`, and < `the size of list`
 * @note - This function will return nullptr when the `index ` is out of bounds
 */
void* hm_list_get(hm_list* list, size_t index);
```
<details>
<summary>try: insert & get</summary>

```c
#include <hm_list.h>

#include <stdlib.h>
#include <stdio.h>

void print_list(hm_list* list) {
    int s = list->size;
    // get and print
    for (int i = 0; i < s; i++) {
        int* v = hm_list_get(list, i);
        printf("%d ", *v);
    }
    printf("\n");
}

int main() 
{
    hm_list list;
    // init
    hm_list_init(&list, free);

    int cnt = 10;
    // insert head
    for (int i = 0; i < cnt; i++) {
        int* v = (int*)malloc(sizeof(int));
        *v = i;
        hm_list_insert_head(&list, v);
    }
    print_list(&list);    

    // insert tail
    for (int i = 0; i < cnt; i++) {
        int* v = (int*)malloc(sizeof(int));
        *v = i;
        hm_list_insert_tail(&list, v);
    }
    print_list(&list);

    int* val = (int*)malloc(sizeof(int));
    *val = -1;
    // insert val at index 2
    hm_list_insert_index(&list, val, 2);
    print_list(&list);
    
    hm_list_free(&list);
    return 0;
}
```

<details>
<summary>run result</summary>

```txt
9 8 7 6 5 4 3 2 1 0 
9 8 7 6 5 4 3 2 1 0 0 1 2 3 4 5 6 7 8 9 
9 8 -1 7 6 5 4 3 2 1 0 0 1 2 3 4 5 6 7 8 9 
```
</details>

</details>
<br><br><br>

> **Del**
```c
/**
 * Delete the Node at the head of the list
 * @note - This function will return `hm_list_ret_none` when list is empty
 */
hm_list_ret hm_list_del_head(hm_list* list);

/**
 * Delete the Node at the tail of the list
 * @note - This function will return `hm_list_ret_none` when list is empty
 */
hm_list_ret hm_list_del_tail(hm_list* list);

/**
 * Delete the Node at the specified `index`
 * @note - `Index` must be >= `0`, and < `the size of list`
 * 
 * @note - This function will return `hm_list_ret_none` when the `index` is out of bounds
 * 
 */
hm_list_ret hm_list_del_index(hm_list* list, size_t index);
```
<details>
<summary>try: del</summary>

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
    // init
    hm_list_init(&list, free);

    int cnt = 20;
    // insert tail
    for (int i = 0; i < cnt; i++) {
        int* v = (int*)malloc(sizeof(int));
        *v = i;
        hm_list_insert_tail(&list, v);
    }
    print_list(&list);

    // del index | index: 4
    hm_list_del_index(&list, 4);
    print_list(&list);

    int num_h = 3;
    // del head | num: 3
    for (int i = 0; i < num_h; i++) {
        hm_list_del_head(&list);
    }
    print_list(&list);

    int num_t = 2;
    // del tail | num: 2
    for (int i = 0; i < num_t; i++) {
        hm_list_del_tail(&list);
    }
    print_list(&list);

    hm_list_free(&list);
    return 0;
}
```

<details>
<summary>run result</summary>

```txt
0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 
0 1 2 3 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 
3 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 
3 5 6 7 8 9 10 11 12 13 14 15 16 17 
```
</details>

</details>
<br><br><br>

> **Iterator**
```c

// These functions will be remove

/**
 * Initialize iterator of list
 * 
 */
void hm_iter_list_init(hm_iter_list* iter, hm_list* list);

/**
 * Check if the iterator has a next element
 * @note - Return true if iterator has next
 */
bool hm_iter_list_has_next(hm_iter_list* iter);

/**
 * Get next value of list
 * @note - Use `hm_iter_list_has_next()` to check before calling `hm_iter_list_next()`
 */
void* hm_iter_list_next(hm_iter_list* iter);


// New functions of iterator

/**
 * Initialize iterator of list
 * @note - Let the iterator point to the head of the list
 */
void hm_iter_list_init_head(hm_iter_list* iter, hm_list* list);

/**
 * Initialize iterator of list
 * @note - Let the iterator point to the tail of the list
 */
void hm_iter_list_init_tail(hm_iter_list* iter, hm_list* list);

/**
 * Initialize iterator of list
 * @note - Let the iterator point to the specified `index` of the list
 * @note - Iterator will point to `NULL` if `index` is out of bounds
 */
void hm_iter_list_init_index(hm_iter_list* iter, hm_list* list, size_t index);

/**
 * Check if the iterator's current pointer is valid
 */
bool hm_iter_list_has_cur(hm_iter_list* iter);

/**
 * Get the current value of the iterator
 * Use `hm_iter_list_has_cur()` to check before calling `hm_iter_list_cur()`
 */
void* hm_iter_list_cur(hm_iter_list* iter);

/**
 * Move the iterator's pointer to next
 */
void hm_iter_list_move_next(hm_iter_list* iter);

/**
 * Move the iterator's pointer to prev
 */
void hm_iter_list_move_prev(hm_iter_list* iter);
```
<details>
<summary>try: iter  [old]</summary>

```c
#include <hm_list.h>

#include <stdlib.h>
#include <stdio.h>
int main() 
{
    hm_list list;
    // init
    hm_list_init(&list, free);


    int cnt = 20;
    // insert tail
    for (int i = 0; i < cnt; i++) {
        int* v = (int*)malloc(sizeof(int));
        *v = i;
        hm_list_insert_tail(&list, v);
    }
    
    // iter [old]
    
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
<summary>run result</summary>

```txt
0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 
```
</details>

</details>

<details>
<summary>try: iter  [new]</summary>

```c
#include <hm_list.h>

#include <stdlib.h>
#include <stdio.h>
int main() 
{
    hm_list list;
    // init
    hm_list_init(&list, free);

    int cnt = 20;
    // insert tail
    for (int i = 0; i < cnt; i++) {
        int* v = (int*)malloc(sizeof(int));
        *v = i;
        hm_list_insert_tail(&list, v);
    }
    
    // iter [new]
    hm_iter_list iter;

    // iterate from head 
    hm_iter_list_init_head(&iter, &list);
    while (hm_iter_list_has_cur(&iter)) {
        int* v = hm_iter_list_cur(&iter);
        printf("%d ", *v);
        // move next
        hm_iter_list_move_next(&iter);
    }
    printf("\n");
    
    // iterate from tail | reverse
    hm_iter_list_init_tail(&iter, &list);
    while (hm_iter_list_has_cur(&iter)) {
        int* v = hm_iter_list_cur(&iter);
        printf("%d ", *v);
        // move prev
        hm_iter_list_move_prev(&iter);
    }
    printf("\n");
    
    // iterate from index 4
    hm_iter_list_init_index(&iter, &list, 4);
    while (hm_iter_list_has_cur(&iter)) {
        int* v = hm_iter_list_cur(&iter);
        printf("%d ", *v);
        // move next
        hm_iter_list_move_next(&iter);
    }
    printf("\n");

    hm_list_free(&list);    
    return 0;
}
```

<details>
<summary>run result</summary>

```txt
0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 
19 18 17 16 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0 
4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 
```
</details>

</details>
<br><br><br>


> **Free**
```c
/**
 * Free a list
 * @note - If the list was initialized without freeing capacity, 
 * this function can't free the values stored in the Nodes
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
    // init
    hm_list_init(&list, free);

    int cnt = 20;
    // insert tail
    for (int i = 0; i < cnt; i++) {
        int* v = (int*)malloc(sizeof(int));
        *v = i;
        hm_list_insert_tail(&list, v);
    }
    
    // list must be freed after use
    hm_list_free(&list);
    
    return 0;
}
```
</details>
<br><br><br>


> **Sort**
```c
/**
 * Sort list
 * @note - Pass a comparison function to this function
 */
void hm_list_sort(hm_list* list, hm_cmp cmp);
```
<details>
<summary>try: sort</summary>

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
    // init
    hm_list_init(&list, free);

    // get random number
    int seed = 5201314;
    srand(seed);

    int cnt = 10;
    // insert tail
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
<summary>run result</summary>

```txt
11517 31859 16191 3650 6711 3535 9555 7633 30056 28891 
3535 3650 6711 7633 9555 11517 16191 28891 30056 31859 
```
</details>

</details>
<br><br><br>

## Tips

- **Do not modify the list (del, insert, free, or sort) while iterating over it**
