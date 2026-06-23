# Changelog

- All the change of this library will write in this file. 
- The format reference [Keep a Changelog](https://keepachangelog.com/en/1.1.0/)
- The version number follows [Semantic Versioning](https://semver.org/)

## [Unreleased]

## [0.4.2] -2026-06-23

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