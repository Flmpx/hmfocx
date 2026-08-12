- **hmfocx**  
- A library of some basic container  

<p align = "center">
    English | <a href = "./README.zh-CN.md">简体中文</a>
</p>

- Building in `Linux`
1. Git clone
```shell
git clone https://github.com/Flmpx/hmfocx.git
```
2. Build with cmake(enter the `build` folder firstly)
```shell
cmake ..
```
3. Generate the static library(this operation is also done in the `build` folder)
```shell
make
```
4. Everything has been done  
you can find a file named `libhmfocx.a` in the `bin` folder, this is a static library about this project


- Building in `Windows` (MinGW)
- The steps are the same as above, just the command is different
```shell
git clone https://github.com/Flmpx/hmfocx.git

cmake .. -G "MinGW Makefiles"

mingw32-make
```

- But I strongly suggest you to use `cmake` to clone, build and use this library(In that case, you can see comments of a function when you use this function)

1. Add some code in `CMakeLists.txt` of your project

```cmake
# ...
include(FetchContent)

FetchContent_Declare(
    hmfocx
    GIT_REPOSITORY https://github.com/Flmpx/hmfocx.git  # or git@github.com:Flmpx/hmfocx.git
    GIT_TAG v0.12.0
)

FetchContent_MakeAvailable(hmfocx)

# ...

target_link_library(your_executable PRIVATE hmfocx)
```

2. Run common the build process of `cmake`


- Some detailed information about container functions

1. [hm_list](docs/hm_list.md)

2. [hm_map](docs/hm_map.md)

3. [hm_pool](docs/hm_pool.md)

4. [hm_stack](docs/hm_stack.md)

5. [hm_queue](docs/hm_queue.md)

6. [hm_heap](docs/hm_heap.md)

7. [hm_set](docs/hm_set.md)

8. [hm_arr](docs/hm_arr.md)

9. [hm_str](docs/hm_str.md)