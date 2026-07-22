# Changelog

- All the change of this library will write in this file. 
- The format reference [Keep a Changelog](https://keepachangelog.com/en/1.1.0/)
- The version number follows [Semantic Versioning](https://semver.org/)

## [Unreleased]

## [0.10.0] - 2026-07-22

### Added
- Add the new function for `hm_list` -- `hm_list_get_node`
    - It handle the problem -- You can't change the pointer of the `hm_listnode`

### Changed
- **refactor:** Change the behavior when insert the same key in `hm_map` & `hm_set` --- The insert function(`hm_map_insert` & `hm_set_insert`) still remain the old entry(map --> key & val, set --> key) in `hm_map` or `hm_set` 
    - You should handle this special sitution(insert same key)
- **refactor:** Use function instead of macro to achieve `small functions` of every container
    - The old code that using macro can use this new version `small functions` whitout any fix
- Change the return code values in `hm_heap`, `hm_queue` and `hm_set`

### Improved
- Improve the speed of `hm_map_clear` and `hm_set_clear` when `free_key` and `free_val` is `NULL`

## [0.9.0] - 2026-07-22

### Added
- Add new container for this library -- `hm_set`, it including some basic operations, such as `insert`, `get` and `del`, other detailed information see [hm_set](docs/hm_set.md)

### Changed
- **Breaking:** Change the entry and it's status variable of `hm_map` -- `hm_entry`  --> `hm_map_entry` & `hm_entry_status` --> `hm_map_entry_status`
    - The reason is that this entry variable of `hm_set` is `hm_set_entry` and `hm_set_entry_status` and make them easier to take apart, so change it 
- **Breaking:** Change the iterator's name of valibale anf functions in `hm_list`, `hm_map` and `hm_set` -- `hm_iter_{container}_*` --> `hm_{container}_iter_*`
    - The reason is that the name of most variable and functions of ervery container is start with `hm_{container}_*`, so change it


## [0.8.0] - 2026-07-16

### Added
- Add new container for this library -- `hm_heap`, it includes some basic operations, such as `insert` and `extract`, other detailed information see [hm_heap](docs/hm_heap.md)

### Fixed
- Fix bug in `hm_heap_build`, `hm_heap_build_dynamic_grow` and `hm_heap_rebuild`
    - Bug: It will enter a infinite loop when call these function

## [0.7.0] - 2026-07-09

### Added
- Add new container for this library -- `hm_queue`, it includes some basic operation, such as `dequeue` and `enqueue`, other detailed information see [hm_queue](docs/hm_queue.md)

### Fixed
- Fixed bug in `hm_queue_deq`
    - It should take the remainder using `queue->capacity`, not `queue->front`

## [0.6.0] - 2026-07-06

### Fixed
-  Add the check the overflow before expand the length of map

-  Fix the function `hm_stack_init_dynamic_grow` missing variable name
    - It will use the `free` function in `stdlib.h` in funciton `hm_stack_init_dynamic_grow`, not the `pass-in free` function

### Added 
- Add new container for this library -- `hm_stack`, it includes some basic operation, such as `push` and `pop`, other detailed information see [hm_stack](docs/hm_stack.md)
- Add the new function for map -- `hm_map_get_load_factor`
- Add the new iterator's function for list, These function is more flexible
    -  **Init:** `hm_iter_list_init_head` `hm_iter_list_init_tail`  `hm_iter_list_init_index`
    -  **Cur:** `hm_iter_list_has_cur` `hm_iter_list_cur`
    -  **Move:** `hm_iter_list_move_next` `hm_iter_list_move_prev`

### Deprecated
- The old list iterator's function will be removed
    -  `hm_iter_list_init()`
    -  `hm_iter_list_has_next()`
    -  `hm_iter_list_next`

## [0.5.0] - 2026-06-28

### Added
- Add the memory pool(fixed-size block) container, it includes some basic operation, such as `allocate` and `free` block, other detailed information see [hm_pool](docs/hm_pool.md)

### Fixed 
- Fix the wrong behavior in function `hm_pool_block_allocate` when the pass-in `block_size` or `blocks_per_page` is too big

- Fix the function `hm_pool_get_pages` can't get the number of page correctly

- Fix the wrong free logic in function `hm_pool_free`

## Changed
- `hm_pool_block_allocate` will return `NULL` if `blocks_per_page` or `block_size` is `zero`

- **Breaking** Change `hm_cmp` and `hm_hash` signatures from `void*` to use `const void*`



## [0.4.2] - 2026-06-23

### Changed
- **refactor:** extracted the `node-finding` logic from `hm_list_get`, `hm_list_del_index` and `hm_list_insert_index` into a standalone function `hm_list_get_node`, eliminating duplicate code.

## [0.4.1] - 2026-06-23

### Fixed
- Handle `SIZE_MAX` return from `max_prime()` during the `hm_map` resize to avoid using non-prime capacity



## [0.4.0] - 2026-06-22

### Changed
- **Breaking:** Change `hm_cmp` and `hm_hash` signatures from `void*` to use `const void*`
    - Users must update their signature accordingly, the code below is an example
    ```c
    // old version
    int my_cmp(void* ptr1, void* ptr2);
    size_t my_hash(void* ptr);

    // new version
    int my_cmp(const void* ptr1, const void* ptr2);
    size_t my_hash(const void* ptr);
    ```


## [0.3.0] - 2026-06-22

### Added
- Add `hm_list_sort` function that can sort a list to this library

## [0.2.7] - 2026-06-19

### Added
- Add `hm_list` and `hm_map` to this library.
- Add the function of iterator for `hm_list` and `hm_map`.

### Fixed
- Fix the situation of memory leak in function of `hm_list_del_index` .

## Changed
- Reduce the steps to find the target Node in list when use the function of `hm_list_get`, `hm_list_del_index` and `hm_list_insert_index`.