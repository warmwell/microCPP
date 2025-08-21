# CMakeLists.txt for merging sorted arrays program

cmake_minimum_required(VERSION 3.10)

# 强制使用Unix Makefiles生成器（适用于Linux/Unix系统）
if(UNIX AND NOT CMAKE_GENERATOR)
    set(CMAKE_GENERATOR "Unix Makefiles" CACHE STRING "CMake generator" FORCE)
endif()

# 设置编译器（在project之前）
if(NOT CMAKE_CXX_COMPILER)
    find_program(CMAKE_CXX_COMPILER NAMES g++ clang++ c++)
    if(NOT CMAKE_CXX_COMPILER)
        message(FATAL_ERROR "No C++ compiler found!")
    endif()
endif()

# 项目名称和版本
project(MergeSortedArrays VERSION 1.0.0 LANGUAGES CXX)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置编译选项
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

# 设置不同构建类型的编译选项
set(CMAKE_CXX_FLAGS_DEBUG "-g -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")

# 如果没有指定构建类型，默认为Release
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# 定义可执行文件
add_executable(merge_sorted_arrays merge_sorted_arrays.cpp)

# 设置可执行文件输出目录
set_target_properties(merge_sorted_arrays PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

# 添加自定义目标用于运行程序
add_custom_target(run
    COMMAND merge_sorted_arrays
    DEPENDS merge_sorted_arrays
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Running merge sorted arrays program"
)

# 添加自定义目标用于清理
add_custom_target(clean-all
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target clean
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/CMakeFiles
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/CMakeCache.txt
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/cmake_install.cmake
    COMMENT "Cleaning all build files"
)

# 打印构建信息
message(STATUS "Project: ${PROJECT_NAME}")
message(STATUS "Version: ${PROJECT_VERSION}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")