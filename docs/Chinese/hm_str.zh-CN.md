<a id = "head"></a>
# `hm_str` 详细信息

<p align = "center">
    <a href = "../hm_str.md">English</a> | 简体中文
</p>


## 导航
- [介绍](#intro)
- [函数](#func)
    - [小功能](#smallfunc)
    - [初始化](#init)
    - [拼接](#append)
    - [获取](#get)
    - [弹出](#pop)
    - [缩容](#shrink)
    - [清空](#clear)
    - [释放](#free)
- [一点小事](#otherthings)
- [其他容器](#othercontainer)


<a id = "intro"></a>

## 介绍
- 你可以使用这些函数来对一个字符串进行操作(比如 append功能)
- 它提供基本的字符串操作

<a id = "func"></a>

## 函数

>  [!Tip]
>  - 字符串的 capacity 等于 字符串所占的真实内存大小-1(`\0` 还要占一个字节)
>  - 但是字符串中的 `capacity` 有且仅有两种情况 -- `capacity == 0` 或者 `capacity >= 17(min_capacity)`, 故当 capacity 为0的时候, 里面的字符串是 `NULL`


>  [!Note]  
>  - 所有的函数都不会检查 `hm_str*` 是否为空指针, 所以你要确保这个指针是有效的
>  - 其他关于指针的参数是否可以为空指针, 具体看下面的函数注释


<br><br><br>

<a id = "smallfunc"></a>

> **小功能**

```c
size_t hm_str_len(hm_str* str);
size_t hm_str_capacity(hm_str* str);
```
<br><br><br>



<a id = "init"></a>

> **初始化**

```c
/**
 * 初始化字符串
 */
void hm_str_init(hm_str* str);

/**
 * 初始化字符串并预分配空间
 * 
 * @note 使用参数 **capacity** 来设置字符串的初始容量, 最小容量是17, 如果 **capacity** < **min_capacity**, 那容量就等于 **min_capacity**
 * 
 * @return 初始化失败时返回 **hm_str_ret_error**
 * @return 初始化成功时返回 **hm_str_ret_suc**
 */
hm_str_ret hm_str_init_reserve(hm_str* str, size_t capacity);
```

<details>
<summary>try: 初始化</summary>

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

> **拼接**

```c
/**
 * 在字符串末尾拼接一个字符串
 * 
 * @return 拼接失败时返回 **hm_str_ret_error**
 * @return 拼接成功时返回 **hm_str_ret_suc**
 * 
 * @warning 不可以把自己拼接自己
 * @warning 参数 **sub_str** 不可以为 **NULL**
 */
hm_str_ret hm_str_append(hm_str* str, const char* sub_str);
```

<a id = "get"></a>

> **获取**

```c
/**
 * 获取指定下标获取字符串
 * 
 * @note 索引必须 >= **0** 并且 < **字符串长度**
 * 
 * @return 字符串的容量为 **0** 时返回 **NULL**
 * @return 索引无效时返回 **NULL**
 * 
 * @warning 不可以修改字符串
 */
const char* hm_str_get(hm_str* str, size_t index);
```

<details>
<summary>try: 拼接 & 获取</summary>

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

> **弹出**

```c
/**
 * 弹出字符串
 * 
 * @note 字符串会被删除, 但不会释放掉它所占的内存(内存权转移)
 * @note 使用完弹出的字符串后记得释放掉
 */
char* hm_str_pop(hm_str* str);
```

<details>
<summary>try: 弹出</summary>

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

> **缩容**

```c
/**
 * 如果可以, 对字符串进行缩容
 * 
 * @return 如果缩容成功, 返回 **hm_str_ret_suc**
 * @return 如果字符串无法缩容, 返回 **hm_str_ret_none**
 * @return 如果缩容失败, 返回 **hm_str_ret_error**
 */
hm_str_ret hm_str_shrink(hm_str* str);
```

<details>
<summary>try: 缩容</summary>

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

> **清空**

```c
/**
 * 清空字符串
 */
void hm_str_clear(hm_str* str);
```

<details>
<summary>try: 清空</summary>

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

> **释放**

```c
/**
 * 释放字符串
 */
void hm_str_free(hm_str* str);
```

<details>
<summary>try: 释放</summary>

```c

```

</details>
<br><br><br>




<a id = "otherthings"></a>

## 一点小事

>  [!Tip]
>  - 经过 `hm_str_free()` 或 `hm_str_init()` 后的字符串的指针是会变成 `NULL` 的, 这个时候 `hm_str_get()` 或者 `hm_str_pop()`, 得到的就是 `NULL`
>  





<a id = "othercontainer"></a>

## 其他容器

1. [hm_list](hm_list.zh-CN.md)

2. [hm_map](hm_map.zh-CN.md)

3. [hm_pool](hm_pool.zh-CN.md)

4. [hm_stack](hm_stack.zh-CN.md)

5. [hm_queue](hm_queue.zh-CN.md)

6. [hm_heap](hm_heap.zh-CN.md)

7. [hm_set](hm_set.zh-CN.md)

8. [hm_arr](hm_arr.zh-CN.md)

9. hm_str

<br><br><br>
<div align = "right">
    <a href="#head">↑ Top</a>
</div>
