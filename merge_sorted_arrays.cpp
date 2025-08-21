#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>

/**
 * 合并两个有序数组的不同实现方法
 */

// 方法1: 使用双指针法 (Two Pointers)
std::vector<int> mergeSortedArrays_TwoPointers(const std::vector<int>& arr1, const std::vector<int>& arr2) {
    std::vector<int> result;
    result.reserve(arr1.size() + arr2.size()); // 预分配空间提高性能
    
    size_t i = 0, j = 0;
    
    // 比较两个数组的元素，将较小的加入结果数组
    while (i < arr1.size() && j < arr2.size()) {
        if (arr1[i] <= arr2[j]) {
            result.push_back(arr1[i]);
            i++;
        } else {
            result.push_back(arr2[j]);
            j++;
        }
    }
    
    // 将剩余元素添加到结果数组
    while (i < arr1.size()) {
        result.push_back(arr1[i]);
        i++;
    }
    
    while (j < arr2.size()) {
        result.push_back(arr2[j]);
        j++;
    }
    
    return result;
}

// 方法2: 使用STL的merge函数
std::vector<int> mergeSortedArrays_STL(const std::vector<int>& arr1, const std::vector<int>& arr2) {
    std::vector<int> result;
    result.resize(arr1.size() + arr2.size());
    
    std::merge(arr1.begin(), arr1.end(), 
               arr2.begin(), arr2.end(), 
               result.begin());
    
    return result;
}

// 方法3: 就地合并 (适用于arr1有足够空间的情况)
void mergeSortedArrays_InPlace(std::vector<int>& arr1, int m, const std::vector<int>& arr2, int n) {
    // arr1的大小应该是m+n，前m个元素是有效数据
    // 从后往前填充，避免覆盖未处理的元素
    int i = m - 1;  // arr1的最后一个有效元素
    int j = n - 1;  // arr2的最后一个元素
    int k = m + n - 1;  // 合并后数组的最后一个位置
    
    while (i >= 0 && j >= 0) {
        if (arr1[i] > arr2[j]) {
            arr1[k] = arr1[i];
            i--;
        } else {
            arr1[k] = arr2[j];
            j--;
        }
        k--;
    }
    
    // 如果arr2还有剩余元素
    while (j >= 0) {
        arr1[k] = arr2[j];
        j--;
        k--;
    }
    // 如果arr1还有剩余元素，它们已经在正确位置了
}

// 辅助函数：打印数组
void printArray(const std::vector<int>& arr, const std::string& name) {
    std::cout << name << ": [";
    for (size_t i = 0; i < arr.size(); i++) {
        std::cout << arr[i];
        if (i < arr.size() - 1) std::cout << ", ";
    }
    std::cout << "]" << std::endl;
}

// 测试函数
void runTests() {
    std::cout << "=== 测试合并有序数组 ===" << std::endl;
    
    // 测试用例1: 正常情况
    std::vector<int> arr1 = {1, 3, 5, 7, 9};
    std::vector<int> arr2 = {2, 4, 6, 8, 10};
    
    std::cout << "\n测试用例1:" << std::endl;
    printArray(arr1, "数组1");
    printArray(arr2, "数组2");
    
    auto result1 = mergeSortedArrays_TwoPointers(arr1, arr2);
    printArray(result1, "双指针法结果");
    
    auto result2 = mergeSortedArrays_STL(arr1, arr2);
    printArray(result2, "STL merge结果");
    
    // 测试用例2: 一个数组为空
    std::vector<int> arr3 = {};
    std::vector<int> arr4 = {1, 2, 3};
    
    std::cout << "\n测试用例2 (一个数组为空):" << std::endl;
    printArray(arr3, "数组3");
    printArray(arr4, "数组4");
    
    auto result3 = mergeSortedArrays_TwoPointers(arr3, arr4);
    printArray(result3, "合并结果");
    
    // 测试用例3: 数组长度不同
    std::vector<int> arr5 = {1, 5, 9, 10, 15, 20};
    std::vector<int> arr6 = {2, 3, 8, 13};
    
    std::cout << "\n测试用例3 (长度不同):" << std::endl;
    printArray(arr5, "数组5");
    printArray(arr6, "数组6");
    
    auto result4 = mergeSortedArrays_TwoPointers(arr5, arr6);
    printArray(result4, "合并结果");
    
    // 测试用例4: 就地合并
    std::cout << "\n测试用例4 (就地合并):" << std::endl;
    std::vector<int> arr7 = {1, 2, 3, 0, 0, 0}; // 前3个是有效数据，后3个是空位
    std::vector<int> arr8 = {2, 5, 6};
    
    std::cout << "原始arr7: [1, 2, 3, 0, 0, 0] (前3个有效)" << std::endl;
    printArray(arr8, "arr8");
    
    mergeSortedArrays_InPlace(arr7, 3, arr8, 3);
    printArray(arr7, "就地合并结果");
    
    // 验证结果正确性
    std::vector<int> expected = {1, 2, 2, 3, 5, 6};
    assert(arr7 == expected);
    std::cout << "✓ 就地合并测试通过!" << std::endl;
}

int main() {
    runTests();
    
    std::cout << "\n=== 自定义测试 ===" << std::endl;
    std::cout << "你可以修改main函数中的数组来测试其他情况" << std::endl;
    
    // 用户可以在这里添加自己的测试
    std::vector<int> myArr1 = {1, 4, 7};
    std::vector<int> myArr2 = {2, 3, 5, 6, 8};
    
    std::cout << "\n自定义测试:" << std::endl;
    printArray(myArr1, "自定义数组1");
    printArray(myArr2, "自定义数组2");
    
    auto myResult = mergeSortedArrays_TwoPointers(myArr1, myArr2);
    printArray(myResult, "合并结果");
    
    return 0;
}