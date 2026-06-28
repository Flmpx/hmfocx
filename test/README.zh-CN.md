# 测试代码格式

<p align = "center">
    <a href = "./README.md">English</a> | 简体中文
</p>

## 介绍
- 本 `README.md` 说明如何为每个容器编写测试。

## **`CMakeLists.txt` 内容**

- 文件位置：`test/{容器名}/`

```cmake
cmake_minimum_required(VERSION 3.28.3)  

project(test_hm_list)       # 项目名格式：`test_hm_{容器名}`

set(hm_list ../../src/hm_list.c)       # 待测的 C 文件

set(hm_test test.c)                # 测试文件(必须叫 `test.c`)

set(hm_func ../hm_test.c)          # 通用测试辅助函数(如打印测试信息)

set(hm_cmp ../../function/cmp/hm_cmp.c)        # 该容器测试所需的额外文件


# 远程和本地测试的可执行文件都输出到 `bin/`
# 需要在 `.github/workflows/cmake-single-platform.yml` 中添加 `-DBUILD_TESTS=ON` 才能正确运行测试
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)


# 以下是常规构建流程

set(CMAKE_BUILD_TYPE Debug)

add_executable(test_hm_list ${hm_list} ${hm_test} ${hm_func} ${hm_cmp})

add_test(NAME test_hm_list COMMAND test_hm_list) # 注册测试，名字格式：`test_hm_{容器名}`
```

- 还需在根目录的 `CMakeLists.txt` 中添加一行，参见以下代码：

```cmake
if (BUILD_TESTS)
    enable_testing()
    # 新增的测试
    add_subdirectory(test/list) 

    # 其他测试
    add_subdirectory(test/map)
endif()
```

## `test.c` 内容

- 位置：`test/{容器名}/`

- **以下以 `hm_list` 为例**

### 文件结构

- 测试包含：`功能测试`、`压力测试`、`边界测试`。

| 部分 | 命名格式 |
| --- | --- |
| `功能测试` | `test_{容器名}_{测试主要内容}` |
| `边界测试` | `test_{测试主要内容}` |
| `压力测试` | `test_{容器名}_{测试主要内容}_stress` |

```c
#include "../../include/hm_list.h"
#include "../../function/cmp/hm_cmp.h"
#include "../hm_test.h"

// 该变量记录总失败次数，可作为返回值判断测试是否通过
int all_failure_num = 0;

// 使用宏替换重复代码
#define HM_TEST_COUNTER \
    all_failure_num += fail_cnt;


// 各个测试函数...

void function_test() {
    test_list_init();                               printf("\n");
    // 每个测试完成后都要加 `printf("\n");`
    // 更多……
}

void boundary_test() {
    test_empty_list_oper();                         printf("\n");
    // 更多……
}

void stress_test() {
    test_list_insert_tail_stress();                 printf("\n");
    // 更多……
}

int main()
{
    // 对测试进行粗略分类
    function_test();
    boundary_test();
    stress_test();
    return all_failure_num;
}
```

### 每个测试函数(在 `test.c` 中)

- `头部信息`格式：`{容器} | {测试类型} | {测试内容} | {其他信息}`

- 以下是一个示例：

```c
void test_list_insert_head() {
    
    int fail_cnt = 0;   // 记录本部分的失败次数
    
    // **开始**
    print_run("LIST | FUNC | INSERT HEAD | TYPE: [INT]");
    
    int fail = 0;
    
    // 测试代码……
    
    // **检查**
    check_res(fail == 0, "失败详情", &fail_cnt);
    
    
    // **结束**
    print_end("LIST | FUNC | INSERT HEAD | TYPE: [INT]", fail_cnt);
    // 记录总失败次数
    HM_TEST_COUNTER
    
}
```

- 压力测试中使用 `print_run_time` 打印耗时信息：

```c
print_run_time("INSERT", start, end, nums[i], nums[i]);
```

- 重要函数简介(详细参数见 [test/hm_test.h](hm_test.h) and [test/hm_test.c](hm_test.c))

| 函数 | 作用 |
| --- | --- |
| `print_run` | 测试开始时打印信息 |
| `check_res` | 检查结果是否正确；若失败则`打印信息`并增加 `fail_cnt` |
| `print_end` | 根据 `fail_cnt` 在结束时打印测试结果 |
| `print_run_time` | 根据传入参数打印`耗时`和`速度` |
| `print_speed_vs` | 根据传入的每组参数打印`耗时`和`速度`, 同时将它们进行对比 |

## 提示