# `hm_list` 详细信息

<p align = "center">
    <a href = "../hm_list.md">English</a> | 简体中文
</p>


## 介绍

- 你可以向此链表传递任意值的指针。
- 它提供基本的链表操作。

## 函数

- **初始化**
```c
/**
 * 初始化一个链表
 * @note - 使用 `hm_list_free()` 释放该链表
 * @note - 如果不想让链表释放其值，请将 `hm_free` 函数指针设为 `NULL`
 */
void hm_list_init(hm_list* list, hm_free free);
```

- **插入**
```c
/**
 * 在链表头部插入值
 * @note - 插入失败时返回 `hm_list_ret_error`
 */
hm_list_ret hm_list_insert_head(hm_list* list, void* val);

/**
 * 在链表尾部插入值
 * @note - 插入失败时返回 `hm_list_ret_error`
 */
hm_list_ret hm_list_insert_tail(hm_list* list, void* val);

/**
 * 在指定索引处插入值
 * @note - 索引必须 >= `0` 且 <= `链表大小`
 * @note - 索引无效时返回 `hm_list_ret_warn`
 * @note - 插入失败时返回 `hm_list_ret_error`
 */
hm_list_ret hm_list_insert_index(hm_list* list, void* val, size_t index);
```

- **获取**
```c
/**
 * 根据索引获取值的指针
 * @note - 索引无效时返回`空指针`
 */
void* hm_list_get(hm_list* list, size_t index);
```

- **删除**
```c
/**
 * 删除链表头节点
 * @note - 链表为空时返回 `hm_list_ret_none`
 */
hm_list_ret hm_list_del_head(hm_list* list);

/**
 * 删除链表尾节点
 * @note - 链表为空时返回 `hm_list_ret_none`
 */
hm_list_ret hm_list_del_tail(hm_list* list);

/**
 * 删除指定索引处的节点
 * @note - 索引必须 >= `0` 且 < `链表大小`
 * @note - 索引无效时返回 `hm_list_ret_none`
 */
hm_list_ret hm_list_del_index(hm_list* list, size_t index);
```

- **迭代器**
```c
// 这些函数将会被移除

/**
 * 初始化链表迭代器
 */
void hm_iter_list_init(hm_iter_list* iter, hm_list* list);

/**
 * 检查迭代器是否有下一个元素
 * @note - 有下一个元素时返回 `true`
 */
bool hm_iter_list_has_next(hm_iter_list* iter);

/**
 * 获取下一个元素的值
 * @note - 请先调用 `hm_iter_list_has_next()` 检查是否存在下一个值
 */
void* hm_iter_list_next(hm_iter_list* iter);


// 迭代器的新函数

/**
 * 初始化链表的迭代器
 * @note - 让迭代器指向链表头部
 */
void hm_iter_list_init_head(hm_iter_list* iter, hm_list* list);

/**
 * 初始化链表的迭代器
 * @note - 让迭代器指向链表尾部
 */
void hm_iter_list_init_tail(hm_iter_list* iter, hm_list* list);

/**
 * 初始化链表的迭代器
 * @note - 让迭代器指向链表中指定的索引位置
 * @note - 如果索引越界，迭代器将指向 `NULL`
 */
void hm_iter_list_init_index(hm_iter_list* iter, hm_list* list, size_t index);

/**
 * 检查迭代器的当前指向是否有效
 */
bool hm_iter_list_has_cur(hm_iter_list* iter);

/**
 * 获取迭代器的当前值
 * 在调用 `hm_iter_list_cur()` 之前，请先使用 `hm_iter_list_has_cur()` 进行检查
 */
void* hm_iter_list_cur(hm_iter_list* iter);

/**
 * 将迭代器的指针移动到下一个节点
 */
void hm_iter_list_move_next(hm_iter_list* iter);

/**
 * 将迭代器的指针移动到上一个节点
 */
void hm_iter_list_move_prev(hm_iter_list* iter);
```

- **释放**
```c
/**
 * 释放链表
 * @note - 如果链表初始化时没有赋予释放能力，此函数无法释放节点中的值
 */
void hm_list_free(hm_list* list);
```

- **排序**
```c
/**
 * 对链表排序
 * @note - 传入一个比较函数
 */
void hm_list_sort(hm_list* list, hm_cmp cmp);
```

- **小型函数**
```c
#define hm_list_size(l) ((l)->size)
```

## 提示

- **遍历链表时，请勿修改链表结构(如删除、插入、释放或排序)。**