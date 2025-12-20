# Original - C++20 模块化版本

## 简介

Original 是一个 C++ 基础工具库，是早期同名项目 [Original](https://github.com/frozenlemontee/original) 的 C++20 模块化重构版本。本项目使用现代 C++ 模块（C++20 Modules）重新组织代码，目标是提供更加高效、模块清晰的 STL 相关功能实现。

项目目标是实现 STL 的主要内容以及 Boost 的某些相关功能。如有问题可以在 [Issues](https://github.com/Original-org/original/issues) 中提出，也欢迎参与到项目的实现中来。


## 环境要求

为了编译和使用 Original，请确保开发环境满足以下最低版本要求：

- **C++ 标准**: C++20（带 Modules 支持）
- **CMake**: 3.31 或更高版本
- **编译器**（任选其一）：
    - **GCC**: 14.0 或更高版本
    - **Clang**: 17.0 或更高版本
    - **MSVC**（Visual Studio 2022）：17.10 (版本 14.44.35207) 或更高版本

> 注意：所有编译器都需要支持 C++20 模块特性。如使用 GCC，请确保支持 `-fmodules-ts` 标志。

## 项目架构

本项目采用模块化设计，核心结构如下：

```
src/
├── original.ixx                # 主模块入口
├── metaInfo.ixx               # 元信息模块
├── environment.ixx            # 环境配置模块
├── macros.h                   # 宏定义
├── original.cpp               # 模块实现单元
└── basic/                      # 基础模块
    ├── basic.ixx              # 基础模块入口
    ├── types.ixx              # 类型定义
    ├── number/                # 数值相关
    │   ├── number.ixx         # 数值操作模块
    │   ├── numberImpl.ixx      # 实现细节
    │   ├── traits.ixx         # 类型特性
    │   ├── algorithms.ixx     # 数值算法
    │   └── casts.ixx          # 类型转换
    └── iterator/              # 迭代器相关
        ├── iterator.ixx       # 迭代器模块
        ├── iteratorImpl.ixx    # 实现细节
        ├── traits.ixx         # 迭代器特性
        └── adaptor.ixx        # 迭代器适配器
```

## 安装

### 方法：使用 CMake 远程拉取（推荐）

这里以项目 `hello_original` 为例：

```text
├─CMakeLists.txt
└─main.cpp
```

配置 `CMakeLists.txt`：

```cmake
cmake_minimum_required(VERSION 3.31)
project(hello_original)

set(CMAKE_CXX_STANDARD 23)

set(BUILD_TESTING OFF CACHE BOOL "Disable tests in the fetched project")

include(FetchContent)

FetchContent_Declare(
        original
        GIT_REPOSITORY https://github.com/Original-org/original.git
        GIT_TAG dev
)

FetchContent_MakeAvailable(original)

add_executable(hello_original main.cpp)

target_link_libraries(hello_original PRIVATE original)
```


### 使用示例

`main.cpp`：

```c++
import original;

using namespace original::literals;

#include <iostream>

int main() {
    // 示例代码
    original::I32 arr[5] = {0_i32, 1_i32, 2_i32, 3_i32, 4_i32};
    original::DefaultIterator<original::I32> it{arr};
    for (original::Diff i = 0_diff; i < 5_diff; ++i)
    {
        std::cout << it[i] << " " << std::endl;
    }
    return 0;
}
```

## 模块进度

### Core（基础模块：类型 / 迭代器 / 数值等）

#### 类型系统
- **types**（基础类型定义和操作）
- 完善的编译期类型特性

#### 数值操作
- **number**（数值处理和计算）
- **traits**（数值类型特性）
- **algorithms**（数值相关算法）
- **casts**（安全的类型转换）

#### 迭代器系统
- **iterator**（迭代器抽象）
- **traits**（迭代器类型特性）
- **adaptor**（迭代器适配器）

#### 元信息
- **metaInfo**（编译期元信息）
- **environment**（运行环境配置）

### 后续计划模块

以下模块在早期版本中已设计，目前正在重构为 C++20 模块形式：

<details>
<summary><strong>容器模块（计划）</strong></summary>

- **array**（定长数组）
- **vector**（动态数组）
- **list**（链表）
- **hashMap**（哈希映射）
- **treeMap**（树映射）
- **set**（集合）

</details>

<details>
<summary><strong>算法模块（计划）</strong></summary>

- **query**（查询算法）
- **modify**（修改算法）
- **sort**（排序算法）
- **filter**（过滤操作）

</details>

<details>
<summary><strong>内存管理（计划）</strong></summary>

- **pointer**（智能指针）
- **allocator**（内存分配器）
- **singleton**（单例模式）

</details>

<details>
<summary><strong>Vibrant 异步模块（计划）</strong></summary>

- **thread**（线程管理）
- **async**（异步操作）
- **sync**（同步原语）
- **coroutine**（协程支持）

</details>

<details>
<summary><strong>matrix（计划实现）</strong></summary>

- 张量结构（Tensor）
- 基本线性代数运算（矩阵、向量等）

</details>

<details>
<summary><strong>graph（计划实现）</strong></summary>

- 图结构（邻接表、邻接矩阵）
- 图算法（DFS/BFS、最短路、最小生成树等）

</details>

## 编译和测试

### 编译项目

```shell
cmake -B build -S .
cmake --build build
```

### 运行测试

```shell
cd build
ctest
```

## 主要变化（相比早期版本）

1. **C++20 模块化设计**：全面使用 C++20 Modules 替代传统头文件
2. **改进的模块组织**：清晰的模块层次和依赖关系
3. **编译效率提升**：模块化带来的编译时间优化
4. **现代 C++ 特性**：充分利用 C++20 的新特性
5. **更好的代码隔离**：模块私有实现细节与公开接口的清晰分离

## 许可证

本项目遵循 LICENSE 文件中的许可条款。

## 贡献

欢迎提交 Issue 和 Pull Request！如果您对项目有任何建议或发现了 bug，请在 [Issues](https://github.com/Original-org/original/issues) 中提出。
