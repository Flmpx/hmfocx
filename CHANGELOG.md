# Changelog

- All the change of this library will write in this file. 
- The format reference [Keep a Changelog](https://keepachangelog.com/en/1.1.0/)
- The version number follows [Semantic Versioning](https://semver.org/)

## [Unreleased]

### Added
- Add new container for this library -- `hm_queue`, it includes some basic operation, such as `dequeue` and `enqueue`, other detailed information see [hm_queue](docs/hm_queue.md)

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