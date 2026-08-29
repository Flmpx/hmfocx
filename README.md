# hmfocx
A library of some basic container   

## Build Methods

### Building in `Linux`
#### Git clone  
```shell
git clone https://github.com/Flmpx/hmfocx.git
```
#### Build with cmake(enter the `build` folder firstly)  
```shell
cmake ..
```
#### Generate the static library(this operation is also done in the `build` folder)  
```shell
make
```
#### Everything has been done  
you can find a file named `libhmfocx.a` in the `bin` folder, this is a static library about this project


### Building in `Windows` (MinGW)
#### The steps are the same as above, just the command is different
```shell
git clone https://github.com/Flmpx/hmfocx.git

cmake .. -G "MinGW Makefiles"

mingw32-make
```

### Use `CMake` Completely

I strongly suggest you to use `cmake` to clone, build and use this library(In that case, you can see comments of a function when you use this function)  

#### Add some code in `CMakeLists.txt` of your project  

```cmake
# ...
include(FetchContent)

FetchContent_Declare(
    hmfocx
    GIT_REPOSITORY https://github.com/Flmpx/hmfocx.git  # or git@github.com:Flmpx/hmfocx.git
    GIT_TAG v0.14.0
)

FetchContent_MakeAvailable(hmfocx)

# ...

target_link_library(your_executable PRIVATE hmfocx)
```

#### Run common the build process of `cmake`  
```shell
mkdir build 
cd build
cmake ..
make
```

## Detail Information About Containers

### The document include next part
- Introduction of containers
- The detained comment about functions of every containers
- `Try` part to will teach you how to calling this function
- Some `Tip`,  `Note` or `Warning` in it

### Document Link

| **Struct Name** | **Fact** |
| ---------------- | ----------- |
| [hm_list](docs/hm_list.md) | List | 
| [hm_map](docs/hm_map.md) | Hash Table(Map) | 
| [hm_pool](docs/hm_pool.md) | Memory Pool |
| [hm_stack](docs/hm_stack.md) | Stack |
| [hm_queue](docs/hm_queue.md) | Queue |
| [hm_heap](docs/hm_heap.md) | Heap |
| [hm_set](docs/hm_set.md) | Hash Set |
| [hm_arr](docs/hm_arr.md) | Array |
| [hm_str](docs/hm_str.md) | String |

## Design

The Container include the main container and it's iterator  

### The Name Of Containers Or Some Variable

- main format -- `hm_{container}_{Other_1}_{Other_2}_ ...`
- All containers' name start with `hm_`, like List's name -- `hm_list`, and this is a flag of this library
- And there have some `enum` or `struct` in every container, like Map's entry name -- `hm_map_entry`, some enum's name see comment of every function

### The Functions' Name Of Every Container

- main format -- `hm_{container}_{action}_{more info}_ ...`
- Like conatiner's name, it must start with `hm_`
- The action include `init`, `insert`, `del`, `shrink`, `clear`, `free` and more...

