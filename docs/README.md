
# Document

## The document include next part
- Introduction of containers
- The detained comment about functions of every containers
- `Try` part to teach you how to calling this function
- Some `Tip`,  `Note` or `Warning` in it

## Document Link

| **Struct Name** | **Fact** |
| ---------------- | ----------- |
| [hm_list](hm_list.md) | List | 
| [hm_map](hm_map.md) | Hash Table(Map) | 
| [hm_pool](hm_pool.md) | Memory Pool |
| [hm_stack](hm_stack.md) | Stack |
| [hm_queue](hm_queue.md) | Queue |
| [hm_heap](hm_heap.md) | Heap |
| [hm_set](hm_set.md) | Hash Set |
| [hm_arr](hm_arr.md) | Array |
| [hm_str](hm_str.md) | String |

## Some Tips About All Containers

>  [!Tip]  
>  - Most of the functions **do not** check if the pointer of container is `NULL`, so, you have to make sure the pointer is valid
>  - **Exception:** Pass `NULL` in `hm_{containier}_free()` is allowed
>  - Whether other pointer parameters can be `NULL` depends on the function comments

>  [!Tip]
>  - The `get` function will return `Level 1 Pointer`(map's entry include the Level 1 Pointer of key and val)
>  - The `get_pointer/get_entry/get_node` function will return `Level 2 Pointer`
>  

>  [!Tip]
>  - Use `get` function can change the data that you pass in container
>  - Use `get_pointer/get_entry/get_node` function can change the pointer of the data, it can replace the whole data, so, you have the responsibility to manage the old memory data