<a id = "head"></a>
# The detailed information about `hm_str`

<p align = "center">
    English | <a href = "./Chinese/hm_str.zh-CN.md">简体中文</a>
</p>

## Navigation
- [Introduction](#intro)
- [Functions](#func)
    - [Small Functions](#smallfunc)
    - [Initialize](#init)
    - [Append](#append)
    - [Get](#get)
    - [Pop](#pop)
    - [Shrink](#shrink)
    - [Clear](#clear)
    - [Free](#free)
- [Other Things](#otherthings)
- [Other Containers](#othercontainer)



<a id = "intro"></a>

## Introduction
- You can use the function(like append) to operate a string
- It provides base str operations


<a id = "func"></a>

## Functions

>  [!Tip]
>  - The capacity of str is equal to the real memory size of string - 1(Because `\0` is existed in string)
>  - For this container, the capacity only have two situation -- `capacity == 0` or `capacity >= 17(min_capacity)`, and the val(string) in str is `NULL` when `capacity == 0`
> 


>  [!Note]  
>  - None of the functions will check if `hm_str*` is `NULL`, so, you have to make sure the pointer is valid
>  - Whether other pointer parameters can be `NULL` depends on the function comments below


<br><br><br>

<a id = "smallfunc"></a>

> **Small Functions**

```c
size_t hm_str_len(hm_str* str);
size_t hm_str_capacity(hm_str* str);
```
<br><br><br>



<a id = "init"></a>

> **Initialize**

```c
/**
 * Initialize str
 */
void hm_str_init(hm_str* str);

/**
 * Reserve capacity and initialize str
 * 
 * @note Use the parameter **capacity** to set the start capacity of this str, the **min_capacity** is 17, **capacity** will be **min_capacity** if **capacity** < **min_capacity**
 * 
 * @return Return **hm_str_ret_suc** when initialize success
 * @return Return **hm_str_ret_error** when initialize failure
 */
hm_str_ret hm_str_init_reserve(hm_str* str, size_t capacity);
```

<details>
<summary>try: init</summary>

```c

```

<details>
<summary>run result</summary>

```txt

```

</details>

</details>
<br><br><br>


<a id = "append"></a>

> **Append**

```c
/**
 * Append string in str
 * 
 * @return Return **hm_str_ret_error** when append fail
 * @return Return **hm_str_ret_suc** when append success
 * 
 * @warning Append itself is prohibited
 * @warning The parameter **sub_str** shouldn't be **NULL**
 */
hm_str_ret hm_str_append(hm_str* str, const char* sub_str);
```

<a id = "get"></a>

> **Get**

```c
/**
 * Get the str at the specified index
 * 
 * @note **Index**  must be >= **0**, and <= **the len of str**
 * 
 * @return Return **NULL** when the **capacity** of str is **0**
 * @return Return **NULL** when **index** is out of bounds
 * 
 * @warning Change the string is prohibited
 */
const char* hm_str_get(hm_str* str, size_t index);
```

<details>
<summary>try: append & get</summary>

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
 * Pop the string from str
 * 
 * @note The string will be removed but not free it(Memory Ownership Transfer)
 * @note Please free this string after use
 */
char* hm_str_pop(hm_str* str);
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


<a id = "shrink"></a>

> **Shrink**

```c
/**
 * Shrink the capacity of str if possible
 * 
 * @return Return **hm_str_ret_suc** when shrink success
 * @return Return **hm_str_ret_none** when the str can't be shrunk
 * @return Return **hm_str_ret_error** when shrink failure
 */
hm_str_ret hm_str_shrink(hm_str* str);
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
 * Clear the str
 */
void hm_str_clear(hm_str* str);
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
 * Free the str
 */
void hm_str_free(hm_str* str);
```

<details>
<summary>try: free</summary>

```c

```

</details>
<br><br><br>





<a id = "otherthings"></a>

## Other Things

>  [!Tip]
>  - The string will be `NULL` after calling `hm_str_free()` or `hm_str_init()`, `hm_str_get()` and `hm_str_pop()` will get `NULL` at this time
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

8. [hm_arr](hm_arr.md)

9. hm_str



<br><br><br>
<div align = "right">
    <a href="#head">↑ Top</a>
</div>