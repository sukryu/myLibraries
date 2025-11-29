/**
 * @file test_dynamic_array.cpp
 * @brief Test suite for DynamicArray class
 * @author Your Name
 * @date 2025-11-02
 * @version 1.0.0
 */

#include "linear/dynamic_array.hpp"
#include <iostream>
#include <cassert>
#include <string>

using namespace mylib::linear;

// Test result counter
int tests_passed = 0;
int tests_failed = 0;

// Helper macro for testing
#define TEST(name) \
    std::cout << "Testing: " << name << "... "; \
    try {

#define END_TEST \
        std::cout << "✓ PASSED" << std::endl; \
        tests_passed++; \
    } catch (const std::exception& e) { \
        std::cout << "✗ FAILED: " << e.what() << std::endl; \
        tests_failed++; \
    }

// Test functions
void test_default_constructor() {
    TEST("Default constructor")
    DynamicArray<int> arr;
    assert(arr.size() == 0);
    assert(arr.empty());
    END_TEST
}

void test_constructor_with_capacity() {
    TEST("Constructor with capacity")
    DynamicArray<int> arr(10);
    assert(arr.capacity() >= 10);
    assert(arr.size() == 0);
    END_TEST
}

void test_constructor_with_value() {
    TEST("Constructor with count and value")
    DynamicArray<int> arr(5, 42);
    assert(arr.size() == 5);
    for (size_t i = 0; i < arr.size(); ++i) {
        assert(arr[i] == 42);
    }
    END_TEST
}

void test_initializer_list() {
    TEST("Initializer list constructor")
    DynamicArray<int> arr = {1, 2, 3, 4, 5};
    assert(arr.size() == 5);
    assert(arr[0] == 1);
    assert(arr[2] == 3);
    assert(arr[4] == 5);
    END_TEST
}

void test_copy_constructor() {
    TEST("Copy constructor")
    DynamicArray<int> arr1 = {1, 2, 3};
    DynamicArray<int> arr2(arr1);
    assert(arr2.size() == 3);
    assert(arr2[0] == 1);
    assert(arr2[1] == 2);
    assert(arr2[2] == 3);
    // Verify deep copy
    arr2[0] = 99;
    assert(arr1[0] == 1);  // arr1 should be unchanged
    END_TEST
}

void test_move_constructor() {
    TEST("Move constructor")
    DynamicArray<int> arr1 = {1, 2, 3};
    DynamicArray<int> arr2(std::move(arr1));
    assert(arr2.size() == 3);
    assert(arr2[0] == 1);
    assert(arr1.size() == 0);  // arr1 should be empty after move
    END_TEST
}

void test_push_back() {
    TEST("push_back")
    DynamicArray<int> arr;
    arr.push_back(10);
    arr.push_back(20);
    arr.push_back(30);
    assert(arr.size() == 3);
    assert(arr[0] == 10);
    assert(arr[1] == 20);
    assert(arr[2] == 30);
    END_TEST
}

void test_pop_back() {
    TEST("pop_back")
    DynamicArray<int> arr = {1, 2, 3, 4, 5};
    arr.pop_back();
    assert(arr.size() == 4);
    arr.pop_back();
    arr.pop_back();
    assert(arr.size() == 2);
    assert(arr[0] == 1);
    assert(arr[1] == 2);
    END_TEST
}

void test_at_method() {
    TEST("at() method with bounds checking")
    DynamicArray<int> arr = {10, 20, 30};
    assert(arr.at(0) == 10);
    assert(arr.at(2) == 30);
    
    // Test out of range exception
    bool exception_thrown = false;
    try {
        arr.at(10);
    } catch (const std::out_of_range&) {
        exception_thrown = true;
    }
    assert(exception_thrown);
    END_TEST
}

void test_front_back() {
    TEST("front() and back()")
    DynamicArray<int> arr = {1, 2, 3, 4, 5};
    assert(arr.front() == 1);
    assert(arr.back() == 5);
    arr.front() = 100;
    arr.back() = 500;
    assert(arr.front() == 100);
    assert(arr.back() == 500);
    END_TEST
}

void test_clear() {
    TEST("clear()")
    DynamicArray<int> arr = {1, 2, 3, 4, 5};
    arr.clear();
    assert(arr.size() == 0);
    assert(arr.empty());
    END_TEST
}

void test_resize() {
    TEST("resize()")
    DynamicArray<int> arr = {1, 2, 3};
    arr.resize(5);
    assert(arr.size() == 5);
    
    arr.resize(2);
    assert(arr.size() == 2);
    assert(arr[0] == 1);
    assert(arr[1] == 2);
    END_TEST
}

void test_resize_with_value() {
    TEST("resize() with default value")
    DynamicArray<int> arr = {1, 2, 3};
    arr.resize(6, 99);
    assert(arr.size() == 6);
    assert(arr[3] == 99);
    assert(arr[4] == 99);
    assert(arr[5] == 99);
    END_TEST
}

void test_reserve() {
    TEST("reserve()")
    DynamicArray<int> arr;
    arr.reserve(100);
    assert(arr.capacity() >= 100);
    assert(arr.size() == 0);
    
    // Adding elements shouldn't cause reallocation
    for (int i = 0; i < 50; ++i) {
        arr.push_back(i);
    }
    assert(arr.capacity() >= 100);
    END_TEST
}

void test_shrink_to_fit() {
    TEST("shrink_to_fit()")
    DynamicArray<int> arr;
    arr.reserve(100);
    arr.push_back(1);
    arr.push_back(2);
    arr.shrink_to_fit();
    assert(arr.capacity() == arr.size());
    END_TEST
}

void test_copy_assignment() {
    TEST("Copy assignment operator")
    DynamicArray<int> arr1 = {1, 2, 3};
    DynamicArray<int> arr2;
    arr2 = arr1;
    assert(arr2.size() == 3);
    assert(arr2[1] == 2);
    arr2[0] = 99;
    assert(arr1[0] == 1);  // Verify deep copy
    END_TEST
}

void test_move_assignment() {
    TEST("Move assignment operator")
    DynamicArray<int> arr1 = {1, 2, 3};
    DynamicArray<int> arr2;
    arr2 = std::move(arr1);
    assert(arr2.size() == 3);
    assert(arr2[0] == 1);
    assert(arr1.size() == 0);
    END_TEST
}

void test_swap() {
    TEST("swap()")
    DynamicArray<int> arr1 = {1, 2, 3};
    DynamicArray<int> arr2 = {10, 20};
    arr1.swap(arr2);
    assert(arr1.size() == 2);
    assert(arr1[0] == 10);
    assert(arr2.size() == 3);
    assert(arr2[0] == 1);
    END_TEST
}

void test_large_dataset() {
    TEST("Large dataset (stress test)")
    DynamicArray<int> arr;
    const int count = 10000;
    
    // Add many elements
    for (int i = 0; i < count; ++i) {
        arr.push_back(i);
    }
    assert(arr.size() == count);
    
    // Verify values
    for (int i = 0; i < count; ++i) {
        assert(arr[i] == i);
    }
    
    // Remove many elements
    for (int i = 0; i < count / 2; ++i) {
        arr.pop_back();
    }
    assert(arr.size() == count / 2);
    END_TEST
}

void test_with_doubles() {
    TEST("DynamicArray<double>")
    DynamicArray<double> arr = {1.1, 2.2, 3.3};
    assert(arr.size() == 3);
    assert(arr[0] > 1.0 && arr[0] < 1.2);
    arr.push_back(4.4);
    assert(arr.size() == 4);
    END_TEST
}

void test_capacity_growth() {
    TEST("Capacity growth strategy")
    DynamicArray<int> arr;
    size_t prev_capacity = 0;
    
    for (int i = 0; i < 100; ++i) {
        arr.push_back(i);
        if (arr.capacity() != prev_capacity) {
            // Capacity should grow
            assert(arr.capacity() > prev_capacity);
            prev_capacity = arr.capacity();
        }
    }
    END_TEST
}

// Main test runner
int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "DynamicArray Test Suite" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;

    // Run all tests
    test_default_constructor();
    test_constructor_with_capacity();
    test_constructor_with_value();
    test_initializer_list();
    test_copy_constructor();
    test_move_constructor();
    test_push_back();
    test_pop_back();
    test_at_method();
    test_front_back();
    test_clear();
    test_resize();
    test_resize_with_value();
    test_reserve();
    test_shrink_to_fit();
    test_copy_assignment();
    test_move_assignment();
    test_swap();
    test_large_dataset();
    test_with_doubles();
    test_capacity_growth();

    // Print summary
    std::cout << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Test Results" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "✓ Passed: " << tests_passed << std::endl;
    std::cout << "✗ Failed: " << tests_failed << std::endl;
    std::cout << "Total: " << (tests_passed + tests_failed) << std::endl;
    std::cout << "========================================" << std::endl;

    return tests_failed > 0 ? 1 : 0;
}
