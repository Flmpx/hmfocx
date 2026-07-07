# `hm_queue` 详细信息

<p align = "center">
    <a href = "../hm_queue.md">English</a> | 简体中文
</p>

## 介绍
- 你可以向此队列传递任意值的指针。
- 它提供基本的队列操作。
- 支持动态增长和固定大小两种模式。

## 函数
- **初始化**
```c
/**
 * 初始化队列（固定大小模式）
 * @note - 使用参数 `capacity` 设置队列的大小
 * @note - 初始化失败时返回 `hm_queue_ret_error`
 * @note - 初始化成功时返回 `hm_queue_ret_suc`
 * @note - 如果不想让队列释放其值，请将 `hm_free` 函数指针设为 `NULL`
 */
hm_queue_ret hm_queue_init(hm_queue* queue, size_t capacity, hm_free free);

/**
 * 初始化队列（动态增长模式）
 * @note - 使用参数 `start_capacity` 设置队列的初始大小
 * @note - 初始化失败时返回 `hm_queue_ret_error`
 * @note - 初始化成功时返回 `hm_queue_ret_suc`
 * @note - 如果不想让队列释放其值，请将 `hm_free` 函数指针设为 `NULL`
 */
hm_queue_ret hm_queue_init_dynamic_grow(hm_queue* queue, size_t start_capacity, hm_free free);
```

- **入队**
```c
/**
 * 将一个值入队
 * @note - 队列满时返回 `hm_queue_ret_full`
 * @note - 入队成功时返回 `hm_queue_ret_suc`
 * @note - 若队列为动态增长模式且扩容失败，返回 `hm_queue_ret_error`
 */
hm_queue_ret hm_queue_enq(hm_queue* queue, void* val);
```

- **出队与查看**
```c
/**
 * 从队列前端出队一个值
 * @note - 队列为空时返回 `NULL`
 */
void* hm_queue_deq(hm_queue* queue);

/**
 * 查看队列前端的值
 * @note - 队列为空时返回 `NULL`
 */
void* hm_queue_peek(hm_queue* queue);
```

- **判断**
```c
/**
 * 检查队列是否已满
 */
bool hm_queue_is_full(hm_queue* queue);

/**
 * 检查队列是否为空
 */
bool hm_queue_is_empty(hm_queue* queue);
```

- **清空与释放**
```c
/**
 * 清空队列
 * @note - 仅释放值（如果可以），内部数组仍然保留
 */
void hm_queue_clear(hm_queue* queue);

/**
 * 释放队列的所有内容
 * @note - 动态增长模式的队列可重复使用，而固定大小模式的队列不可以
 */
void hm_queue_free(hm_queue* queue);
```

- **辅助宏**
```c
#define hm_queue_size(s) ((s)->size)
#define hm_queue_capacity(s) ((s)->capacity)
```

## 提示