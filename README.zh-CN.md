- **hmfocx**  
- 一些基本容器的库

<p align = "center">
    <a href = "./README.md">English</a> | 简体中文
</p>

- 在 `Linux` 上构建
1. 克隆Git仓库
```shell
git clone https://github.com/Flmpx/hm-focx.git
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


- 在Windows中构建
- 步骤和上面都是一样的, 只是输入的命令不同
```shell
git clone https://github.com/Flmpx/hm-focx.git

cmake .. -G "MinGW Makefiles"

mingw32-make
```



- 关于这个库容器的一些函数的介绍

1. [hm_list](docs/Chinese/hm_list.zh-CN.md)

2. [hm_map](docs/Chinese/hm_map.zh-CN.md)

3. [hm_pool](docs/Chinese/hm_pool.zh-CN.md)