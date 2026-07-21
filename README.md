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



- Some detailed information about container functions

1. [hm_list](docs/hm_list.md)

2. [hm_map](docs/hm_map.md)

3. [hm_pool](docs/hm_pool.md)

4. [hm_stack](docs/hm_stack.md)

5. [hm_queue](docs/hm_queue.md)

6. [hm_heap](docs/hm_heap.md)

7. [hm_set](docs/hm_set.md)