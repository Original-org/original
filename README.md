# Original - C++20 模块化版本

## 简介

Original 是一个 C++ 基础工具库，是早期同名项目 [Original](https://github.com/frozenlemontee/original) 的 C++20 模块化重构版本。本项目使用现代 C++ 模块（C++20 Modules）重新组织代码，目标是提供更加高效、模块清晰的 STL 相关功能实现。

项目目标是实现 STL 的主要内容以及 Boost 的某些相关功能。如有问题可以在 [Issues](https://github.com/Original-org/original/issues) 中提出，也欢迎参与到项目的实现中来。

# Original

Original 是一个基于 C++20 模块化（Modules）重构的基础工具库，目标是提供可重用的类型、迭代器、算法和若干 STL/Boost 风格的实用组件。

本仓库为 `Original-org/original` 的开发分支（默认分支为 `main`，当前分支 `dev`），欢迎通过 Issue 和 PR 参与贡献。

## 快速概览

- 语言：C++20（需要模块支持）
- 构建系统：CMake（建议 >= 3.31）
- 目标：模块化、编译效率、对现代 C++ 特性的友好支持

## 主要特性

- 基础类型与编译期元信息（types / metaInfo）
- 数值与算法工具（number / algorithms / traits）
- 迭代器与适配器（iterator / adaptor）
- 逐步重构中的容器、并发、内存管理和图/矩阵模块（规划中）

## 要求

- CMake 3.31+
- 支持 C++20 Modules 的编译器（例如较新版本的 GCC/Clang/MSVC）

注意：C++20 模块支持在不同编译器/版本间差异较大，Windows 上使用 MSVC（Visual Studio 2022+）通常更容易获得模块支持；在其他平台请参考您编译器的模块支持文档。

## 快速开始（本地构建）

在仓库根目录执行：

```powershell
# 在 Windows PowerShell 中：
cmake -S . -B build
cmake --build build --config Release

# 运行测试：
ctest --test-dir build -V
```

如果您在多平台或指定生成器下构建（例如 Visual Studio、Ninja 等），请根据您的环境选择合适的 CMake 生成器参数。

## 在其它项目中作为依赖（FetchContent 示例）

可通过 CMake 的 FetchContent 将本项目作为子项目引入：

```cmake
include(FetchContent)

FetchContent_Declare(
  original
  GIT_REPOSITORY https://github.com/Original-org/original.git
  GIT_TAG dev
)
FetchContent_MakeAvailable(original)

add_executable(my_app main.cpp)
target_link_libraries(my_app PRIVATE original)
```

## 示例

示例使用模块导入：

```c++
import original;
#include <iostream>

int main() {
    // 示例代码
    constexpr original::Array<original::I32, 5> arr{0_i32, 1_i32, 2_i32, 3_i32, 4_i32};
    auto acc = [&](auto&&... x){
        original::I32 result{};
        (result += x, ...);
        return result;
    };
    const auto sum = original::structural::forAll(arr, acc);
    std::cout << sum.value() << std::endl; // 10
    return 0;
}
```

## 贡献指南

- 欢迎通过 Issue 报告问题或提出功能请求。
- 请保持代码风格一致，尽量包含小而清晰的提交。

## 许可证

本项目遵循仓库根目录中的 `LICENSE` 文件。

## 联系 & 参考

- 项目主页：https://github.com/Original-org/original
- 问题追踪：https://github.com/Original-org/original/issues
