# 合并有序数组 - C++ 实现

这个项目实现了用C++合并两个有序数组的多种方法。

## 功能特点

- **三种实现方法**：
  - 双指针法 (Two Pointers)
  - STL merge函数
  - 就地合并 (In-place merge)
- **完整测试用例**：覆盖各种边界情况
- **性能优化**：预分配内存空间
- **现代C++**：使用C++17标准

## 编译和运行

### 使用CMake (推荐)

```bash
# 创建构建目录
mkdir build
cd build

# 配置项目
cmake ..

# 编译
make

# 运行
./merge_sorted_arrays

# 或者使用自定义目标直接运行
make run
```

### 故障排除

**如果遇到 `CMAKE_CXX_COMPILER not set` 错误：**

```bash
# 手动指定编译器
cmake -DCMAKE_CXX_COMPILER=g++ ..
# 或者
cmake -DCMAKE_CXX_COMPILER=clang++ ..
```

**如果遇到 `Building for: NMake Makefiles` 或 `nmake not found` 错误：**

```bash
# 明确指定Unix Makefiles生成器
cmake -G "Unix Makefiles" -DCMAKE_CXX_COMPILER=g++ ..

# 或者使用Ninja生成器（如果已安装）
cmake -G "Ninja" -DCMAKE_CXX_COMPILER=g++ ..
```

**推荐的完整命令：**

```bash
mkdir build && cd build
cmake -G "Unix Makefiles" -DCMAKE_CXX_COMPILER=g++ ..
make
./merge_sorted_arrays
```

### CMake构建选项

```bash
# Debug构建
cmake -DCMAKE_BUILD_TYPE=Debug ..
make

# Release构建 (默认)
cmake -DCMAKE_BUILD_TYPE=Release ..
make

# 清理所有构建文件
make clean-all
```

## 算法说明

### 1. 双指针法
- **时间复杂度**: O(m+n)
- **空间复杂度**: O(m+n)
- **适用场景**: 通用情况，创建新数组

### 2. STL merge
- **时间复杂度**: O(m+n)
- **空间复杂度**: O(m+n)
- **适用场景**: 代码简洁，利用标准库优化

### 3. 就地合并
- **时间复杂度**: O(m+n)
- **空间复杂度**: O(1)
- **适用场景**: 第一个数组有足够空间存储合并结果

## 项目结构

```
.
├── CMakeLists.txt           # CMake构建配置
├── merge_sorted_arrays.cpp  # 主程序源码
├── README.md               # 项目说明
└── build/                  # 构建目录 (生成)
```

## 示例输出

程序运行后会显示各种测试用例的结果，验证算法的正确性。你可以修改源码中的自定义测试部分来测试其他数组组合。