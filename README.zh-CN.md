- **hmfocx**  
- 一些基本容器的库

<p align = "center">
    <a href = "./README.md">English</a> | 简体中文
</p>

- 在 `Linux` 上构建
1. 克隆Git仓库
```shell
git clone https://github.com/Flmpx/hmfocx.git
```
2. 使用cmake进行构建(先进入 `build` 文件夹)
```shell
cmake ..
```
3. 生成静态库文件(这个操作也是在 `build` 文件夹中进行)
```shell
make
```
4. 大功告成  
你会在 `bin` 文件夹中找到一个名叫 `libhmfocx.a` 的文件, 这是关于这个项目的一个静态库文件


- 在Windows中构建 (MinGW)
- 步骤和上面都是一样的, 只是输入的命令不同
```shell
git clone https://github.com/Flmpx/hmfocx.git

cmake .. -G "MinGW Makefiles"

mingw32-make
```

- 但我还是强烈建议使用 `cmake` 来克隆, 构建和使用这个库(这样可以直接使用每个函数的注释)

1. 在你的项目中的 `CMakeLists.txt` 中加入如下代码

```cmake
# ...
include(FetchContent)

FetchContent_Declare(
    hmfocx
    GIT_REPOSITORY https://github.com/Flmpx/hmfocx.git  # 或者 git@github.com:Flmpx/hmfocx.git
    GIT_TAG v0.12.0
)

FetchContent_MakeAvailable(hmfocx)

# ...

target_link_library(your_executable PRIVATE hmfocx)
```

2. 然后进行常规的 `cmake` 的构建过程 


- 关于这个库容器的一些函数的介绍

1. [hm_list](docs/Chinese/hm_list.zh-CN.md)

2. [hm_map](docs/Chinese/hm_map.zh-CN.md)

3. [hm_pool](docs/Chinese/hm_pool.zh-CN.md)

4. [hm_stack](docs/Chinese/hm_stack.zh-CN.md)

5. [hm_queue](docs/Chinese/hm_queue.zh-CN.md)

6. [hm_heap](docs/Chinese/hm_heap.zh-CN.md)

7. [hm_set](docs/Chinese/hm_set.zh-CN.md)

8. [hm_arr](docs/Chinese/hm_arr.zh-CN.md)

9. [hm_str](docs/Chinese/hm_str.zh-CN.md)