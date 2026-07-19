# `hm_map` 详细信息

<p align = "center">
    <a href = "../hm_map.md">English</a> | 简体中文
</p>

## 介绍

- 你可以向此散列表传递任意值和键的指针。
- 它提供基本的散列表操作。

## 函数

### **小功能**
```c
#define hm_map_size(l) ((l)->size)
#define hm_map_len(l) ((l)->len)

/**
 * 获取散列表的负载因子
 * @note - 如果散列表的容量为0, 那将返回一个负数
 */
double hm_map_get_load_factor(hm_map* map)
```

### **初始化**
```c
/**
 * 初始化 hm_map
 * @note - 该函数不仅需要键和值的`释放`函数，还需要`哈希`和`比较`键的函数
 * @note - 与链表类似，`free_key` 和 `free_val` 是可选的(可为 `NULL`)，但 `hash_key` 和 `cmp_key` 不能为 `NULL`
 */
void hm_map_init(hm_map* map, hm_hash hash_key, hm_cmp cmp_key, hm_free free_key, hm_free free_val);
```

### **插入**
```c
/**
 * 向散列表中插入一个键值对
 * @note - 插入失败时返回 `hm_map_ret_error`
 * @note - 插入成功时返回 `hm_map_ret_suc`
 * @note - 如果键已存在，旧键仍保留在散列表中，函数返回 `hm_map_ret_existed`
 * @note - 因此，如果你的键是动态分配的，建议你自行释放它(仅为建议)
 */
hm_map_ret hm_map_insert(hm_map* map, void* key, void* val);
```

### **获取**
```c
/**
 * 获取散列表中条目的指针
 * @note - 如果键不存在，返回 `NULL`
 */
hm_entry* hm_map_get(hm_map* map, void* key);
```

### **删除**
```c
/**
 * 根据键删除散列表中的条目
 * @note - 如果键不存在，返回 `hm_map_ret_none`
 */
hm_map_ret hm_map_del(hm_map* map, void* key);
```

### **收缩**
```c
/**
 * 如果可以, 对散列表进行缩容
 * @note - 如果无法缩容，返回 `hm_map_ret_none`
 */
hm_map_ret hm_map_shrink(hm_map* map);
```

### **迭代器**
```c
/**
 * 初始化散列表的迭代器
 */
void hm_iter_map_init(hm_iter_map* iter, hm_map* map);

/**
 * 检查迭代器是否有下一个条目
 * @note - 有下一个条目时返回 `true`
 */
bool hm_iter_map_has_next(hm_iter_map* iter);

/**
 * 通过迭代器获取下一个条目的指针
 * @note - 请先调用 `hm_iter_map_has_next()` 检查是否存在下一个条目
 */
hm_entry* hm_iter_map_next(hm_iter_map* iter);
```

### **清空与释放**
```c
/**
 * 释放散列表中的键和值，但保留桶和状态标志数组
 */
void hm_map_clear(hm_map* map);

/**
 * 释放散列表的所有内容
 */
void hm_map_free(hm_map* map);
```


## 提示

- **遍历散列表时，请勿修改散列表结构(如删除、插入、清空或释放)。**