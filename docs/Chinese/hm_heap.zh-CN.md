# `hm_heap` 详细信息

<p align = "center">
    <a href = "../hm_heap.md">English</a> | 简体中文
</p>

## 介绍
- 你可以向此堆传递任意值的指针。
- 它提供基本的堆操作。
- 支持动态增长和固定大小两种模式。
- 支持通过自定义 `cmp` 函数进行排序。
- 支持通过传入的数组构建堆。

## 函数
- **初始化**
```c
/**
 * 初始化堆（固定大小模式）
 * @note - 使用参数 capacity 设置堆的容量
 * @note - 初始化失败时返回 hm_heap_ret_error
 * @note - 初始化成功时返回 hm_heap_ret_suc
 * @note - 如果不想让堆释放其值，请将 hm_free 函数指针设为 NULL
 * @note - hm_cmp 函数指针不能为 NULL
 */
hm_heap_ret hm_heap_init(hm_heap* heap, size_t capacity, hm_free free, hm_cmp cmp);

/**
 * 初始化堆（动态增长模式）
 * @note - 使用参数 start_capacity 设置堆的初始容量
 * @note - 初始化失败时返回 hm_heap_ret_error
 * @note - 初始化成功时返回 hm_heap_ret_suc
 * @note - 如果不想让堆释放其值，请将 hm_free 函数指针设为 NULL
 * @note - hm_cmp 函数指针不能为 NULL
 */
hm_heap_ret hm_heap_init_dynamic_grow(hm_heap* heap, size_t start_capacity, hm_free free, hm_cmp cmp);
```

- **插入**
```c
/**
 * 向堆中插入一个值
 * @note - 堆满时返回 hm_heap_ret_full
 * @note - 插入成功时返回 hm_heap_ret_suc
 * @note - 若堆为动态增长模式且扩容失败，返回 hm_heap_ret_error
 */
hm_heap_ret hm_heap_insert(hm_heap* heap, void* val);
```

- **取出与查看**
```c
/**
 * 从堆中取出一个值
 * @note - 堆为空时返回 NULL
 */
void* hm_heap_extract(hm_heap* heap);

/**
 * 查看堆顶的值
 * @note - 堆为空时返回 NULL
 */
void* hm_heap_peek(hm_heap* heap);
```

- **判断**
```c
/**
 * 检查堆是否已满
 */
bool hm_heap_is_full(hm_heap* heap);

/**
 * 检查堆是否为空
 */
bool hm_heap_is_empty(hm_heap* heap);
```

- **构建**
```c
/**
 * 通过传入的值数组和相关参数构建堆（固定大小模式）
 * @warning - vals 应位于系统的堆内存中
 */
hm_heap_ret hm_heap_build(hm_heap* heap, void** vals, size_t size, size_t capacity, hm_free free, hm_cmp cmp);

/**
 * 通过传入的值数组和相关参数构建堆（动态增长模式）
 * @warning - vals 应位于系统的堆内存中
 */
hm_heap_ret hm_heap_build_dynamic_grow(hm_heap* heap, void** vals, size_t size, size_t capacity, hm_free free, hm_cmp cmp);

/**
 * 使用新的 cmp 函数重建堆
 */
void hm_heap_rebuild(hm_heap* heap, hm_cmp new_cmp);
```

- **清空与释放**
```c
/**
 * 清空堆
 * @note - 仅释放值（如果可以），内部数组仍然保留
 */
void hm_heap_clear(hm_heap* heap);

/**
 * 释放堆的所有内容
 * @note - 动态增长模式的堆可重复使用，而固定大小模式的堆不可以
 */
void hm_heap_free(hm_heap* heap);
```

## 提示