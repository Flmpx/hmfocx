<a id = "head"></a>
# The detailed information about `hm_arr`

<p align = "center">
    English | <a href = "./Chinese/hm_arr.zh-CN.md">简体中文</a>
</p>

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
- [Tips](#tip)
- [Other Containers](#othercontainer)



<a id = "intro"></a>

## Introduction
- You can pass a pointer to any value into this arr
- It provides basic arr operations
- It supports **dynamic-grow** and **fixed-size** modes

<a id = "func"></a>

## Functions

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
 * 
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

```

<details>
<summary>run result</summary>

```txt

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

```

<details>
<summary>run result</summary>

```txt

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

```

<details>
<summary>run result</summary>

```txt

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

```

<details>
<summary>run result</summary>

```txt

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

```

<details>
<summary>run result</summary>

```txt

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

```

<details>
<summary>run result</summary>

```txt

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

```

<details>
<summary>run result</summary>

```txt

```

</details>

</details>
<br><br><br>





<a id = "tip"></a>

## Tips
- **Return a pointer to the pointer of value**: What you pass in is a pointer (a pointer to the value), the internal container will have a space to store this pointer, and what’s returned is a pointer to this space. You can use it to change what it points to, but be careful to manage the memory yourself.




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



<br><br><br>
<div align = "right">
    <a href="#head">↑ Top</a>
</div>