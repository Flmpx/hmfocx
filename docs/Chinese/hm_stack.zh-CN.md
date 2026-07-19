# `hm_stack` 详细信息

<p align = "center">
    <a href = "../hm_stack.md">English</a> | 简体中文
</p>


## 介绍
- 你可以向此栈传递任意值的指针。
- 它提供基本的栈操作。
- 支持动态增长和固定大小两种模式。

## 函数


### **小功能**
```c
#define hm_stack_size(s) ((s)->top)
#define hm_stack_capacity(s) ((s)->capacity)
```

### **初始化**
```c
/**
 * 初始化栈（固定大小模式）
 * @note - 使用参数 `capacity` 设置栈的大小
 * @note - 初始化失败时返回 `hm_stack_ret_error`
 * @note - 初始化成功时返回 `hm_stack_ret_suc`
 * @note - 如果不想让栈释放其值，请将 `hm_free` 函数指针设为 `NULL`
 */
hm_stack_ret hm_stack_init(hm_stack* stack, size_t capacity, hm_free free);

/**
 * 初始化栈（动态增长模式）
 * @note - 使用参数 `start_capacity` 设置栈的初始大小
 * @note - 初始化失败时返回 `hm_stack_ret_error`
 * @note - 初始化成功时返回 `hm_stack_ret_suc`
 * @note - 如果不想让栈释放其值，请将 `hm_free` 函数指针设为 `NULL`
 */
hm_stack_ret hm_stack_init_dynamic_grow(hm_stack* stack, size_t start_capacity, hm_free free);
```

### **入栈**
```c
/**
 * 将一个值压入栈
 * @note - 栈满时返回 `hm_stack_ret_full`
 * @note - 入栈成功时返回 `hm_stack_ret_suc`
 * @note - 若栈为动态增长模式且扩容失败，返回 `hm_stack_ret_error`
 */
hm_stack_ret hm_stack_push(hm_stack* stack, void* val);
```

### **出栈与查看**
```c
/**
 * 从栈中弹出一个值
 * @note - 栈为空时返回 `NULL`
 */
void* hm_stack_pop(hm_stack* stack);

/**
 * 查看栈顶的值
 * @note - 栈为空时返回 `NULL`
 */
void* hm_stack_peek(hm_stack* stack);
```

### **判断**
```c
/**
 * 检查栈是否已满
 */
bool hm_stack_is_full(hm_stack* stack);

/**
 * 检查栈是否为空
 */
bool hm_stack_is_empty(hm_stack* stack);
```

### **清空与释放**
```c
/**
 * 清空栈
 * @note - 仅释放值（如果可以），内部数组仍然保留
 */
void hm_stack_clear(hm_stack* stack);

/**
 * 释放栈的所有内容
 * @note - 动态增长模式的栈可重复使用，但固定大小模式的栈不可以
 */
void hm_stack_free(hm_stack* stack);
```

## 提示
