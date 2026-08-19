<a id = "head"></a>

# The detailed information about `hm_arr`


## Navigation
- [Introduction](#intro)
- [Functions](#func)
    - [Small Functions](#smallfunc)
    - [Initialize](#init)
    - [Insert](#insert)
    - [Get](#get)
    - [Del](#del)
    - [Pop](#pop)
    - [Judge](#judge)
    - [Shrink](#shrink)
    - [Clear](#clear)
    - [Free](#free)
- [Other Things](#otherthings)
- [Other Containers](#othercontainer)



<a id = "intro"></a>

## Introduction
- You can pass a pointer to any value into this arr
- It provides basic arr operations
- It supports **dynamic-grow** and **fixed-size** modes

<a id = "func"></a>

## Functions

>  [!Note]  
>  - Most of the functions will **abort**(in debug build) when `hm_arr*` is `NULL`, so, you have to make sure the pointer is valid
>  - **Exception:** Pass `NULL` in `hm_arr_free()` is allowed
>  - Whether other pointer parameters can be `NULL` depends on the function comments below

<br><br><br>


<a id = "smallfunc"></a>

> **Small Functions**

```c
size_t hm_arr_size(hm_arr* arr);
size_t hm_arr_capacity(hm_arr* arr);
```

<br><br><br>


<a id = "init"></a>

> **Initialize**

```c
/**
 * Initialize the arr(fixed-size arr)
 * 
 * @note Use the parameter **capacity** to set the capacity for this arr
 * @note If you do **NOT** want the arr to free its values, set the **free_val** function pointer to **NULL**
 * 
 * @return Return **hm_arr_ret_error** when initialize failure
 * @return Return **hm_arr_ret_suc** when initialize success
 */
hm_arr_ret hm_arr_init(hm_arr* arr, size_t capacity, hm_free free_val);

/**
 * Initialize the arr(dynamic-grow arr)
 * 
 * @note Use the parameter **start_capacity** to set the start capacity for this arr
 * @note If you do **NOT** want the arr to free its values, set the **free_val** function pointer to **NULL**
 * 
 * @return Return **hm_arr_ret_error** when initialize failure
 * @return Return **hm_arr_ret_suc** when initialize success
 */
hm_arr_ret hm_arr_init_dynamic_grow(hm_arr* arr, size_t start_capacity, hm_free free_val);
```

<details>
<summary>try: init</summary>

```c
#include <hm_arr.h>

#include <stdio.h>
#include <stdlib.h>

int main()
{
    int capacity = 50;
    hm_arr arr;
    // fixed-size
    hm_arr_init(&arr, capacity, free);
    hm_arr_free(&arr);
    
    // dynamic-grow
    hm_arr_init_dynamic_grow(&arr, capacity, free);
    hm_arr_free(&arr);

    return 0;
}
```

</details>
<br><br><br>


<a id = "insert"></a>

> **Insert**

```c
/**
 * Insert a value at the head of the arr
 * 
 * @return Return **hm_arr_ret_full** when arr is full
 * @return Return **hm_arr_ret_suc** when insert success
 * @return Return **hm_arr_ret_error** when arr is **dynamic-grow** and expand failure
 */
hm_arr_ret hm_arr_insert_head(hm_arr* arr, void* val);

/**
 * Insert a value at the tail of the arr
 * 
 * @return Return **hm_arr_ret_full** when arr is full
 * @return Return **hm_arr_ret_suc** when insert success
 * @return Return **hm_arr_ret_error** when arr is **dynamic-grow** and expand failure
 */
hm_arr_ret hm_arr_insert_tail(hm_arr* arr, void* val);

/**
 * Insert a value at the given **index**
 * 
 * @note **Index** must be >= **0**, and <= **the size of arr**
 * 
 * @return Return **hm_arr_ret_warn** when the **index** is out of bounds
 * @return Return **hm_arr_ret_full** when arr is full
 * @return Return **hm_arr_ret_suc** when insert success
 * @return Return **hm_arr_ret_error** when arr is **dynamic-grow** and expand failure
 */
hm_arr_ret hm_arr_insert_index(hm_arr* arr, void* val, size_t index);
```


<a id = "get"></a>

> **Get**

```c
/**
 * Get a pointer to the value at the specified **index**
 * 
 * @note **Index** must be >= **0**, and < **the size of arr**
 * 
 * @return Return **NULL** when the **index** is out of bounds
 */
void* hm_arr_get(hm_arr* arr, size_t index);

/**
 * Get a pointer to the pointer of value at the specified **index**
 * 
 * @note Use the pointer can change **the pointer to the value**
 * @note **Index** must be >= **0**, and < **the size of arr**
 * 
 * @return Return **NULL** when the **index** is out of bounds
 */
void** hm_arr_get_pointer(hm_arr* arr, size_t index);
```


<details>
<summary>try: insert & get</summary>

```c
#include <hm_arr.h>

#include <stdlib.h>
#include <stdio.h>

void print_arr(hm_arr* arr) {
    int s = hm_arr_size(arr);
    // get and print
    for (int i = 0; i < s; i++) {
        int* v = hm_arr_get(arr, i);
        printf("%d ", *v);
    }
    printf("\n");
}

int main() 
{
    hm_arr arr;
    // init
    int capacity = 30;
    // fixed-size
    hm_arr_init(&arr, capacity, free);

    int cnt = 10;
    // insert head
    for (int i = 0; i < cnt; i++) {
        int* v = (int*)malloc(sizeof(int));
        *v = i;
        hm_arr_insert_head(&arr, v);
    }
    print_arr(&arr);    

    // insert tail
    for (int i = 0; i < cnt; i++) {
        int* v = (int*)malloc(sizeof(int));
        *v = i;
        hm_arr_insert_tail(&arr, v);
    }
    print_arr(&arr);

    int* val = (int*)malloc(sizeof(int));
    *val = -1;
    // insert val at index 2
    hm_arr_insert_index(&arr, val, 2);
    print_arr(&arr);


    // use get to change val at index 3
    int* v = hm_arr_get(&arr, 3);
    *v = 66666666;
    print_arr(&arr);
    
    hm_arr_free(&arr);
    return 0;
}
```

<details>
<summary>run result</summary>

```txt
9 8 7 6 5 4 3 2 1 0 
9 8 7 6 5 4 3 2 1 0 0 1 2 3 4 5 6 7 8 9 
9 8 -1 7 6 5 4 3 2 1 0 0 1 2 3 4 5 6 7 8 9 
9 8 -1 66666666 6 5 4 3 2 1 0 0 1 2 3 4 5 6 7 8 9
```

</details>

</details>


<details>
<summary>try: insert & get pointer</summary>

```c
#include <hm_arr.h>

#include <stdlib.h>
#include <stdio.h>

char* vals[] = {"xl", "oi", "i", "hate", "love", "so", "family"};

void print_arr(hm_arr* arr) {
    int s = hm_arr_size(arr);
    // get_pointer and print
    for (int i = 0; i < s; i++) {
        char** v = (char**)hm_arr_get_pointer(arr, i);
        printf("| %d. %s\n", i, *v);
    }
    printf("\n");
}

int main() 
{
    hm_arr arr;
    // init
    int capacity = 30;
    hm_arr_init(&arr, capacity, NULL);

    // insert
    int cnt = sizeof(vals) / sizeof(char*);
    for (int i = 0; i < cnt; i++) {
        hm_arr_insert_tail(&arr, vals[i]);
    }
    print_arr(&arr);


    char* tmp_str = "Hello, I'm Flmpx";
    // use get_pointer to change the pointer of val at index 3
    char** v = (char**)hm_arr_get_pointer(&arr, 3);
    *v = tmp_str;
    print_arr(&arr);
    
    hm_arr_free(&arr);
    return 0;
}
```

<details>
<summary>run result</summary>

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

> **Del**

```c
/**
 * Delete the val at the head of the arr
 * 
 * @return Return **hm_arr_ret_none** when arr is empty
 * @return Return **hm_arr_ret_suc** when delete success
 */
hm_arr_ret hm_arr_del_head(hm_arr* arr);

/**
 * Delete the val at the tail of the arr
 * 
 * @return Return **hm_arr_ret_none** when arr is empty
 * @return Return **hm_arr_ret_suc** when delete success
 */
hm_arr_ret hm_arr_del_tail(hm_arr* arr);

/**
 * Delete the val at the specified **index**
 * 
 * @note **Index** must be >= **0**, and < **the size of arr**
 * 
 * @return Return **hm_arr_ret_none** when the **index** is out of bounds or arr is empty
 * @return Return **hm_arr_ret_suc** when delete success
 */
hm_arr_ret hm_arr_del_index(hm_arr* arr, size_t index);
```

<details>
<summary>try: del</summary>

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
    // init
    int capacity = 20;
    // fixed-size
    hm_arr_init(&arr, capacity, free);

    // insert tail
    for (int i = 0; i < capacity; i++) {
        int* v = (int*)malloc(sizeof(int));
        *v = i;
        hm_arr_insert_tail(&arr, v);
    }
    print_arr(&arr);

    // del index | index: 4
    hm_arr_del_index(&arr, 4);
    print_arr(&arr);

    int num_h = 3;
    // del head | num: 3
    for (int i = 0; i < num_h; i++) {
        hm_arr_del_head(&arr);
    }
    print_arr(&arr);

    int num_t = 2;
    // del tail | num: 2
    for (int i = 0; i < num_t; i++) {
        hm_arr_del_tail(&arr);
    }
    print_arr(&arr);

    hm_arr_free(&arr);
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




<a id = "pop"></a>

> **Pop**

```c
/**
 * Pop the val at the specified **index**
 * 
 * @note The val will be removed but not free its memory(Memory Ownership Transfer)
 * @note **Index** must be >= **0**, and < **the size of arr**
 * 
 * @return Return **NULL** when the **index** is out of bounds
 */
void* hm_arr_pop(hm_arr* arr, size_t index);
```

<details>
<summary>try: pop</summary>

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
    // init
    int capacity = 20;
    // fixed-size
    hm_arr_init(&arr, capacity, free);

    // insert
    for (int i = 0; i < capacity; i++) {
        int* v = (int*)malloc(sizeof(int));
        *v = i;
        hm_arr_insert_tail(&arr, v);
    }
    print_arr(&arr);

    // pop | index: 4
    int* pop_v = hm_arr_pop(&arr, 4);
    print_arr(&arr);

    // print poped val
    printf("pop val: %d\n", *pop_v);

    free(pop_v); // memory ownership swap is happend, so, you should free it
    hm_arr_free(&arr);
    return 0;
}
```

<details>
<summary>run result</summary>

```txt
0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 
0 1 2 3 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 
pop val: 4
```

</details>

</details>
<br><br><br>




<a id = "judge"></a>

> **Judge**

```c
/**
 * Check if the arr is full
 */
bool hm_arr_is_full(hm_arr* arr);

/**
 * Check if the arr is empty
 */
bool hm_arr_is_empty(hm_arr* arr);
```

<details>
<summary>try: judge</summary>

```c
#include <hm_arr.h>

#include <stdio.h>
#include <stdlib.h>

int main()
{
    int capacity = 20;
    hm_arr arr;
    // fixed-size
    hm_arr_init(&arr, capacity, free);
    
    if (hm_arr_is_empty(&arr)) {
        printf("arr is empty\n");
    }

    // insert
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
<summary>run result</summary>

```txt
arr is empty
arr is full
```

</details>

</details>
<br><br><br>




<a id = "shrink"></a>

> **Shrink**

```c
/**
 * Shrink the capacity of arr if possible
 * 
 * @note Only dynamic-grow arr have a chance to shrink
 * 
 * @return Return **hm_arr_ret_suc** when shrink success
 * @return Return **hm_arr_ret_none** when the arr can't be shrunk
 * @return Return **hm_arr_ret_error** when shrink failure
 */
hm_arr_ret hm_arr_shrink(hm_arr* arr);
```

<details>
<summary>try: shrink</summary>

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
    // only dynamic-grow arr can do
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
<summary>run result</summary>

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

> **Clear**

```c
/**
 * Clear the arr 
 * 
 * @note Only free the values(if possible),  but keep the vals array existed
 */
void hm_arr_clear(hm_arr* arr);
```

<details>
<summary>try: clear</summary>

```c
#include <hm_arr.h>

#include <stdio.h>
#include <stdlib.h>

void print_arr_status(hm_arr* arr) {
    // print size and capacity of arr
    printf("size: %-3zu, capacity: %-3zu\n", hm_arr_size(arr), hm_arr_capacity(arr));
}

int main()
{
    int capacity = 20;
    hm_arr arr;
    // fixed-size
    hm_arr_init(&arr, capacity, free);
    
    for (int i = 0; i < capacity; i++) {
        int* v = (int*)malloc(sizeof(int));
        *v = i;
        hm_arr_insert_tail(&arr, v);
    }
    print_arr_status(&arr);
    
    // clear
    hm_arr_clear(&arr);
    
    print_arr_status(&arr);

    hm_arr_free(&arr);
    return 0;
}
```

<details>
<summary>run result</summary>

```txt
size: 20 , capacity: 20 
size: 0  , capacity: 20 
```

</details>

</details>
<br><br><br>




<a id = "free"></a>

> **Free**

```c
/**
 * Free all contents of the arr
 * 
 * @note The arr can be reused when it is **dynamic-grow** but **fixed-size** cannot
 */
void hm_arr_free(hm_arr* arr);
```

<details>
<summary>try: free</summary>

```c
#include <hm_arr.h>

#include <stdio.h>
#include <stdlib.h>

int main()
{
    int capacity = 20;
    hm_arr arr;
    // fixed-size
    hm_arr_init(&arr, capacity, free);
    
    for (int i = 0; i < capacity; i++) {
        int* v = (int*)malloc(sizeof(int));
        *v = i;
        hm_arr_insert_tail(&arr, v);
    }

    // arr must be freed after use
    hm_arr_free(&arr);
    return 0;
}
```

</details>
<br><br><br>





<a id = "otherthings"></a>

## Other Things

>  [!Tip]
>  **Return a pointer to the pointer of value**: What you pass in is a pointer (a pointer to the value), the internal container will have a space to store this pointer, and what’s returned is a pointer to this space. You can use it to change what it points to, but be careful to manage the memory yourself.
>  




<a id = "othercontainer"></a>

## Other Containers

1. [hm_list](hm_list.md)

2. [hm_map](hm_map.md)

3. [hm_pool](hm_pool.md)

4. [hm_stack](hm_stack.md)

5. [hm_queue](hm_queue.md)

6. [hm_heap](hm_heap.md)

7. [hm_set](hm_set.md)

8. hm_arr

9. [hm_str](hm_str.md)



<br><br><br>
<div align = "right">
    <a href="#head">↑ Top</a>
</div>