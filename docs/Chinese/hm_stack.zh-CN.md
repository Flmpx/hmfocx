# `hm_stack` 详细信息

<p align = "center">
    <a href = "../hm_stack.md">English</a> | 简体中文
</p>



## 导航
- [介绍](#intro)
- [函数](#func)
    - [小功能](#smallfunc)
    - [初始化](#init)
    - [入栈](#push)
    - [出栈与查看](#poppeek)
    - [判断](#judge)
    - [清空与释放](#clearfree)
- [提示](#tip)



<a id = "intro"></a>

## 介绍
- 你可以向此栈传递任意值的指针。
- 它提供基本的栈操作。
- 支持动态增长和固定大小两种模式。



<a id = "func"></a>

## 函数


<a id = "smallfunc"></a>

> **小功能**
```c
#define hm_stack_size(s) ((s)->top)
#define hm_stack_capacity(s) ((s)->capacity)
```
<br><br><br>


<a id = "init"></a>

> **初始化**
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
<details>
<summary>try: 初始化</summary>

```c
#include <hm_stack.h>

#include <stdio.h>
#include <stdlib.h>

int main()
{
    int capacity = 50;
    hm_stack stack;
    // 固定大小模式
    hm_stack_init(&stack, capacity, free);
    hm_stack_free(&stack);
    
    // 动态增长模式
    hm_stack_init_dynamic_grow(&stack, capacity, free);
    hm_stack_free(&stack);

    return 0;
}
```
</details>
<br><br><br>


<a id = "push"></a>

> **入栈**
```c
/**
 * 将一个值压入栈
 * @note - 栈满时返回 `hm_stack_ret_full`
 * @note - 入栈成功时返回 `hm_stack_ret_suc`
 * @note - 若栈为动态增长模式且扩容失败，返回 `hm_stack_ret_error`
 */
hm_stack_ret hm_stack_push(hm_stack* stack, void* val);
```


<a id = "poppeek"></a>

> **出栈与查看**
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
</details>


<details>
<summary>try: 入栈 & 查看 & 出栈</summary>

```c
#include <hm_stack.h>

#include <stdio.h>
#include <stdlib.h>

int main()
{
    int capacity = 20;
    hm_stack stack;
    // 固定大小模式
    hm_stack_init(&stack, capacity, free);
    
    // 入栈
    for (int i = 0; i < capacity; i++) {
        int* v = (int*)malloc(sizeof(int));
        *v = i;
        hm_stack_push(&stack, v);
    }
    
    // 查看
    int* val = hm_stack_peek(&stack);
    printf("%d\n", *val);

    // 出栈
    for (int i = 0; i < capacity; i++) {
        int* v = hm_stack_pop(&stack);
        printf("%d ", *v);
        free(v);
    }

    hm_stack_free(&stack);
    return 0;
}
```

<details>
<summary>运行结果</summary>

```txt
19
19 18 17 16 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0 
```
</details>

</details>
<br><br><br>


<a id = "judge"></a>

> **判断**
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
<details>
<summary>try: 判断</summary>

```c
#include <hm_stack.h>

#include <stdio.h>
#include <stdlib.h>

int main()
{
    int capacity = 20;
    hm_stack stack;
    // 固定大小模式
    hm_stack_init(&stack, capacity, free);
    
    if (hm_stack_is_empty(&stack)) {
        printf("stack is empty\n");
    }

    // 入栈
    int i = 0;
    while (!hm_stack_is_full(&stack)) {
        int* v = (int*)malloc(sizeof(int));
        *v = i;
        hm_stack_push(&stack, v);
        i++;
    }

    if (hm_stack_is_full(&stack)) {
        printf("stack is full\n");
    }

    hm_stack_free(&stack);
    return 0;
}
```

<details>
<summary>运行结果</summary>

```txt
stack is empty
stack is full
```
</details>

</details>
<br><br><br>



<a id = "clearfree"></a>

> **清空与释放**
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
<details>
<summary>try: 清空</summary>

```c
#include <hm_stack.h>

#include <stdio.h>
#include <stdlib.h>

void print_stack_status(hm_stack* stack) {
    // 打印栈的 size 和 capacity
    printf("size: %-3d, capacity: %-3d\n", hm_stack_size(stack), hm_stack_capacity(stack));
}

int main()
{
    int capacity = 20;
    hm_stack stack;
    // 固定大小模式
    hm_stack_init(&stack, capacity, free);
    
    for (int i = 0; i < capacity; i++) {
        int* v = (int*)malloc(sizeof(int));
        *v = i;
        hm_stack_push(&stack, v);
    }
    print_stack_status(&stack);
    
    // 清空
    hm_stack_clear(&stack);
    
    print_stack_status(&stack);

    hm_stack_free(&stack);
    return 0;
}
```

<details>
<summary>运行结果</summary>

```txt
size: 20 , capacity: 20 
size: 0  , capacity: 20 
```
</details>

</details>


<details>
<summary>try: 释放</summary>

```c
#include <hm_stack.h>

#include <stdio.h>
#include <stdlib.h>

int main()
{
    int capacity = 20;
    hm_stack stack;
    // 固定大小模式
    hm_stack_init(&stack, capacity, free);
    
    for (int i = 0; i < capacity; i++) {
        int* v = (int*)malloc(sizeof(int));
        *v = i;
        hm_stack_push(&stack, v);
    }

    // 在使用完栈后必须释放掉
    hm_stack_free(&stack);
    return 0;
}
```

</details>
<br><br><br>


<a id = "tip"></a>

## 提示
