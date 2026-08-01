<a id = "head"></a>
# `hm_arr` 详细信息

<p align = "center">
    <a href = "../hm_arr.md">English</a> | 简体中文
</p>


## 导航
- [介绍](#intro)
- [函数](#func)
    - [小功能](#smallfunc)
    - [初始化](#init)
    - [插入](#insert)
    - [获取](#get)
    - [删除](#del)
    - [弹出](#pop)
    - [判断](#judge)
    - [缩容](#shrink)
    - [清空](#clear)
    - [释放](#free)
- [注意事项](#tip)
- [其他容器](#othercontainer)


<a id = "intro"></a>

## 介绍
- 你可以向此数组传递任意值的指针
- 它提供基本的数组操作
- 支持动态增长和固定大小两种模式

<a id = "func"></a>

## 函数

<a id = "smallfunc"></a>

> **小功能**

```c
size_t hm_arr_size(hm_arr* arr);
size_t hm_arr_capacity(hm_arr* arr);
```

<br><br><br>


<a id = "init"></a>

> **初始化**

```c
/**
 * 初始化数组 (固定大小模式)
 * 
 * @note 使用参数 **capacity** 设置数组的容量
 * @note 如果不想让数组释放其值, 将 **free_val** 函数指针设为 **NULL**
 * 
 * @return 初始化失败时返回 **hm_arr_ret_error**
 * @return 初始化成功时返回 **hm_arr_ret_suc**
 */
hm_arr_ret hm_arr_init(hm_arr* arr, size_t capacity, hm_free free_val);

/**
 * 初始化数组 (动态增长模式)
 * 
 * @note 使用参数 **start_capacity** 设置数组的初始容量
 * @note 如果不想让数组释放其值, 将 **free_val** 函数指针设为 **NULL**
 * 
 * @return 初始化失败时返回 **hm_arr_ret_error**
 * @return 初始化成功时返回 **hm_arr_ret_suc**
 */
hm_arr_ret hm_arr_init_dynamic_grow(hm_arr* arr, size_t start_capacity, hm_free free_val);
```

<details>
<summary>try: 初始化</summary>

```c

```

</details>
<br><br><br>


<a id = "insert"></a>

> **插入**

```c
/**
 * 在数组头部插入值
 * 
 * @return 数组满时返回 **hm_arr_ret_full**
 * @return 插入成功时返回 **hm_arr_ret_suc**
 * @return 若数组尾动态增长模式并且扩容失败时返回 **hm_arr_ret_error**
 */
hm_arr_ret hm_arr_insert_head(hm_arr* arr, void* val);

/**
 * 在数组尾部插入值
 * 
 * @return 数组满时返回 **hm_arr_ret_full**
 * @return 插入成功时返回 **hm_arr_ret_suc**
 * @return 若数组尾动态增长模式并且扩容失败时返回 **hm_arr_ret_error**
 */
hm_arr_ret hm_arr_insert_tail(hm_arr* arr, void* val);

/**
 * 在指定索引处插入值
 * 
 * @note 索引必须 >= **0** 并且 <= **数组大小**
 * 
 * @return 索引无效时返回 **hm_arr_ret_warn**
 * @return 数组满时返回 **hm_arr_ret_full**
 * @return 插入成功时返回 **hm_arr_ret_suc**
 * @return 若数组尾动态增长模式并且扩容失败时返回 **hm_arr_ret_error**
 */
hm_arr_ret hm_arr_insert_index(hm_arr* arr, void* val, size_t index);
```


<a id = "get"></a>

> **获取**

```c
/**
 * 根据索引获取值的指针
 * 
 * @note 索引必须 >= **0** 并且 < **数组大小**
 * 
 * @return 索引无效时返回 **NULL**
 */
void* hm_arr_get(hm_arr* arr, size_t index);

/**
 * 根据索引获取值的指针的指针
 * 
 * @note 使用这个指针可以改变 **指向值的指针**
 * @note 索引必须 >= **0** 并且 < **数组大小**
 * 
 * @return 索引无效时返回 **NULL**
 */
void** hm_arr_get_pointer(hm_arr* arr, size_t index);
```


<details>
<summary>try: 插入 & 获取</summary>

```c

```

<details>
<summary>运行结果</summary>

```txt

```

</details>

</details>
<br><br><br>



<a id = "del"></a>

> **删除**

```c
/**
 * 删除数组头部的值
 * 
 * @return 数组为空时返回 **hm_arr_ret_none**
 * @return 删除成功时返回 **hm_arr_ret_suc**
 */
hm_arr_ret hm_arr_del_head(hm_arr* arr);

/**
 * 删除数组的尾部的值
 * 
 * @return 数组为空时返回 **hm_arr_ret_none**
 * @return 删除成功时返回 **hm_arr_ret_suc**
 */
hm_arr_ret hm_arr_del_tail(hm_arr* arr);

/**
 * 删除数组指定索引处的值
 * 
 * @note 索引必须 >= **0** 并且 < **数组大小**
 * 
 * @return 索引无效或者数组为空时返回 **hm_arr_ret_none**
 * @return 删除成功时返回 **hm_arr_ret_suc**
 */
hm_arr_ret hm_arr_del_index(hm_arr* arr, size_t index);
```

<details>
<summary>try: 删除</summary>

```c

```

<details>
<summary>运行结果</summary>

```txt

```

</details>

</details>
<br><br><br>




<a id = "pop"></a>

> **弹出**

```c
/**
 * 从数组指定索引处弹出一个值
 * 
 * @note 值会被移除但不会释放它所占的内存(内存权转移)
 * @note 索引必须 >= **0** 并且 < **数组大小**
 * 
 * @return 当索引无效时返回 **NULL**
 */
void* hm_arr_pop(hm_arr* arr, size_t index);
```

<details>
<summary>try: 弹出</summary>

```c

```

<details>
<summary>运行结果</summary>

```txt

```

</details>

</details>
<br><br><br>




<a id = "judge"></a>

> **判断**

```c
/**
 * 判断数组是否已满
 */
bool hm_arr_is_full(hm_arr* arr);

/**
 * 判断数组是否为空
 */
bool hm_arr_is_empty(hm_arr* arr);
```

<details>
<summary>try: 判断</summary>

```c

```

<details>
<summary>运行结果</summary>

```txt

```

</details>

</details>
<br><br><br>




<a id = "shrink"></a>

> **缩容**

```c
/**
 * 如果可以, 对数组进行缩容
 * 
 * @note 只用动态增长的数组有机会缩容
 * 
 * @return 如果缩容成功, 返回 **hm_arr_ret_suc**
 * @return 如果数组无法缩容, 返回 **hm_arr_ret_none**
 * @return 如果缩容失败, 返回 **hm_arr_ret_error**
 */
hm_arr_ret hm_arr_shrink(hm_arr* arr);
```

<details>
<summary>try: 缩容</summary>

```c

```

<details>
<summary>运行结果</summary>

```txt

```

</details>

</details>
<br><br><br>



<a id = "clear"></a>

> **清空**

```c
/**
 * 清空数组
 * 
 * @note 仅释放值 (如果可以), 内部数组仍然保留
 */
void hm_arr_clear(hm_arr* arr);
```

<details>
<summary>try: 清空</summary>

```c

```

<details>
<summary>运行结果</summary>

```txt

```

</details>

</details>
<br><br><br>




<a id = "free"></a>

> **释放**

```c
/**
 * 释放数组的所有内容
 * 
 * @note 动态增长模式的数组可重复使用, 但固定大小模式的数组不可以
 */
void hm_arr_free(hm_arr* arr);
```

<details>
<summary>try: 释放</summary>

```c

```

</details>
<br><br><br>

<a id = "tip"></a>

## 注意事项
- 注意 **数组大小** 和 **数组容量** 的区别, 前者是数组中元素的个数, 后者是可容纳元素的总容量
- **返回值的指针的指针**: 传入的是指针(值的指针), 内部容器会有区域存储这个指针, 返回的是这个区域的指针, 可用于修改指向, 但注意要自行管理内存






<a id = "othercontainer"></a>

## 其他容器

1. [hm_list](hm_list.zh-CN.md)

2. [hm_map](hm_map.zh-CN.md)

3. [hm_pool](hm_pool.zh-CN.md)

4. [hm_stack](hm_stack.zh-CN.md)

5. [hm_queue](hm_queue.zh-CN.md)

6. [hm_heap](hm_heap.zh-CN.md)

7. [hm_set](hm_set.zh-CN.md)

8. hm_arr

<br><br><br>
<div align = "right">
    <a href="#head">↑ Top</a>
</div>
