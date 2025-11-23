# IFLOW.md - csd-cpp 项目指南

## 项目概述

`csd-cpp` 是一个用于处理规范符号数字（Canonical Signed Digit, CSD）表示的 C++ 库。CSD 是一种特殊的数字表示方法，其中每个数字只能是 -1、0 或 1，并且不允许有连续的非零数字。这种表示法的优势在于其唯一性，并且具有最少的非零数字数量。CSD 通常用于数字信号处理应用中，例如滤波器设计，因为它允许使用简单的加法器和减法器来高效地实现算术运算。

该项目提供了一个功能齐全的库，用于在十进制数和 CSD 字符串之间进行转换。主要功能包括：

- `to_csd`: 将十进制数转换为 CSD 字符串
- `to_csd_i`: 将整数转换为 CSD 字符串
- `to_decimal` / `to_decimal_i`: 将 CSD 字符串转换回十进制数
- `to_csdnnz` / `to_csdnnz_i`: 允许指定非零数字的最大数量的 CSD 转换
- `longest_repeated_substring`: 查找最长的重复非重叠子字符串（lcsre 模块）

该项目基于现代 CMake 实践，支持依赖管理、测试套件、代码格式化和文档生成。

## 技术栈与架构

- **语言**: C++11（库代码使用 C++11，standalone 可执行文件使用 C++17）
- **构建系统**: CMake 3.14+（主要），xmake（支持）
- **依赖管理**: CPM.cmake
- **测试框架**: doctest
- **命令行解析**: cxxopts
- **代码格式化**: clang-format, cmake-format
- **文档生成**: Doxygen

项目结构清晰，分为以下几个主要部分：
- `include/csd/`: 头文件，包含所有公开 API（csd 和 lcsre 模块）
- `source/`: 实现文件
- `test/`: 测试代码
- `standalone/`: 独立可执行文件示例
- `cmake/`: CMake 模块和配置
- `documentation/`: 文档配置
- `bench/`: 基准测试
- `experiments/`: 实验性代码
- `all/`: 用于一次性构建所有内容的 CMake 配置

## 构建与运行

### 构建独立目标
```bash
cmake -S standalone -B build/standalone
cmake --build build/standalone
./build/standalone/Csd --help
```

### 构建和运行测试套件
```bash
cmake -S test -B build/test
cmake --build build/test
CTEST_OUTPUT_ON_FAILURE=1 cmake --build build/test --target test

# 或直接运行测试可执行文件：
./build/test/CsdTests
```

### 运行代码格式化
```bash
cmake -S test -B build/test
# 查看更改
cmake --build build/test --target format
# 应用更改
cmake --build build/test --target fix-format
```

### 构建文档
```bash
cmake -S documentation -B build/doc
cmake --build build/doc --target GenerateDocs
# 查看文档
open build/doc/doxygen/html/index.html
```

### 一次性构建所有内容
```bash
cmake -S all -B build
cmake --build build
```

### 使用 xmake 构建（可选）
```bash
xmake f -m release -y
xmake
xmake run test_csd
```

### 基准测试
项目包含基准测试功能，位于 `bench/` 目录中。

### 实验性功能
`experiments/` 目录包含实验性代码，如 `csd_multiplier.cpp`。

## 开发规范

### 编码风格
- 项目使用 clang-format 和 cmake-format 来强制执行代码格式化标准
- 遵循现代 C++ 最佳实践
- 使用 C++11 标准（库代码）和 C++17 标准（standalone 可执行文件）

### 测试实践
- 所有功能都应有相应的测试用例
- 使用 doctest 框架编写单元测试
- 测试代码位于 `test/source/` 目录中
- 包含 CSD 转换功能和 lcsre（最长重复子字符串）功能的测试
- 支持代码覆盖率收集
- 包含压力测试用例，确保在大数据集或边界条件下正常工作

### 贡献指南
1. 使用项目模板进行初始化
2. 遵循项目的 CMake 结构
3. 为新功能添加测试
4. 保持代码格式一致
5. 在源代码中包含适当的文档注释
6. 使用 Doxygen 风格的注释为公共 API 编写文档

## 依赖项

- **fmt**: 用于格式化（版本 10.2.1）
- **spdlog**: 用于日志记录（版本 1.12.0）
- **doctest**: 用于测试
- **cxxopts**: 用于命令行解析
- **CPM.cmake**: 用于依赖管理

## 附加工具

- **Sanitizers**: 可通过 `-DUSE_SANITIZER` 选项启用
- **静态分析器**: 支持 clang-tidy、iwyu、cppcheck，可通过 `-DUSE_STATIC_ANALYZER` 选项启用
- **Ccache**: 可通过 `-DUSE_CCACHE` 选项启用
- **代码覆盖率**: 可通过 `-DENABLE_TEST_COVERAGE` 选项启用测试覆盖率收集

## 项目特点

- 现代 CMake 实践
- 适用于任何规模的头文件库项目
- 库代码和可执行代码的清晰分离
- 集成测试套件
- 通过 GitHub Actions 的持续集成
- 通过 codecov 的代码覆盖率
- 通过 clang-format 和 cmake-format 强制执行代码格式化
- 通过 CPM.cmake 的可重现依赖管理
- 通过 PackageProject.cmake 的可安装目标和自动版本控制
- 通过 Doxygen 和 GitHub Pages 的自动文档和部署
- 支持 sanitizer 工具等附加功能
- 支持 xmake 构建系统（可选）
- 包含压力测试和边界条件测试