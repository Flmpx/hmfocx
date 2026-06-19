# Changelog

- All the change of this library will write in this file. 
- The format reference [Keep a Changelog](https://keepachangelog.com/en/1.1.0/)
- The version number follows [Semantic Versioning](https://semver.org/)

## [Unreleased]


## [0.2.7] - 2026-06-19

### Added
- Add `hm_list` and `hm_map` to this library.
- Add the function of iterator for `hm_list` and `hm_map`.

### Fixed
- Fix the situation of memory leak in function of `hm_list_del_index` .

## Changed
- Reduce the steps to find the target Node in list when use the function of `hm_list_get`, `hm_list_del_index` and `hm_list_insert_index`.