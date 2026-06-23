- **focx-hm**  
- A library of some basic container
  
- Construction in `Linux`
1. git clone
```shell
git clone https://github.com/Flmpx/hm-focx.git
```
2. Build by cmake(enter the `build` folder firstly)
```shell
cmake ..
```
3. Generate static library(operation also in `build` folder)
```shell
make
```
4. Successful  
you can find `./bin/libhmfocx.a`, this is a static library of this project


- Construction in `Windows` (MinGW)
- The steps are the same as above, just the command is different
```shell
git clone https://github.com/Flmpx/hm-focx.git

cmake .. -G "MinGW Makefiles"

mingw32-make
```



- Some detail information of container's functions

1. [hm_list](docs/hm_list.md)

2. [hm_map](docs/hm_map.md)