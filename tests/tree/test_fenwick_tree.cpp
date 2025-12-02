/**
 * @file test_fenwick_tree.cpp
 * @brief Test suite for Fenwick Tree data structures
 * @author Jinhyeok
 * @date 2025-12-02
 * @version 1.0.0
 */

#include "tree/fenwick_tree.hpp"
#include <iostream>
#include <cassert>
#include <vector>
#include <numeric>

using namespace mylib::tree;

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

// ============================================
// FenwickTree Constructor Tests
// ============================================

void test_constructor_from_vector() {
    TEST("Constructor from vector")
    std::vector<int> arr = {1, 3, 5, 7, 9};
    FenwickTree<int> tree(arr);
    
    assert(tree.size() == 5);
    assert(!tree.empty());
    END_TEST
}

void test_constructor_with_size() {
    TEST("Constructor with size")
    FenwickTree<int> tree(10, 5);
    
    assert(tree.size() == 10);
    assert(tree.sum_all() == 50);
    END_TEST
}

void test_constructor_default_zero() {
    TEST("Constructor with default zero")
    FenwickTree<int> tree(5);
    
    assert(tree.sum_all() == 0);
    END_TEST
}

// ============================================
// FenwickTree Prefix Sum Tests
// ============================================

void test_prefix_sum() {
    TEST("Prefix sum queries")
    std::vector<int> arr = {1, 3, 5, 7, 9};
    FenwickTree<int> tree(arr);
    
    assert(tree.prefix_sum(0) == 1);
    assert(tree.prefix_sum(1) == 4);   // 1+3
    assert(tree.prefix_sum(2) == 9);   // 1+3+5
    assert(tree.prefix_sum(3) == 16);  // 1+3+5+7
    assert(tree.prefix_sum(4) == 25);  // 1+3+5+7+9
    END_TEST
}

void test_prefix_sum_single_element() {
    TEST("Prefix sum single element")
    std::vector<int> arr = {42};
    FenwickTree<int> tree(arr);
    
    assert(tree.prefix_sum(0) == 42);
    END_TEST
}

// ============================================
// FenwickTree Range Sum Tests
// ============================================

void test_range_sum() {
    TEST("Range sum queries")
    std::vector<int> arr = {1, 3, 5, 7, 9, 11};
    FenwickTree<int> tree(arr);
    
    assert(tree.range_sum(0, 5) == 36);  // All elements
    assert(tree.range_sum(1, 4) == 24);  // 3+5+7+9
    assert(tree.range_sum(2, 3) == 12);  // 5+7
    assert(tree.range_sum(0, 0) == 1);   // Single element
    END_TEST
}

void test_sum_all() {
    TEST("Sum all elements")
    std::vector<int> arr = {1, 2, 3, 4, 5};
    FenwickTree<int> tree(arr);
    
    assert(tree.sum_all() == 15);
    END_TEST
}

// ============================================
// FenwickTree Update Tests
// ============================================

void test_point_update() {
    TEST("Point update")
    std::vector<int> arr = {1, 3, 5, 7, 9};
    FenwickTree<int> tree(arr);
    
    tree.update(2, 10);  // Add 10 to arr[2]
    assert(tree.sum_all() == 35);  // 1+3+15+7+9
    assert(tree.get(2) == 15);
    END_TEST
}

void test_multiple_updates() {
    TEST("Multiple updates")
    std::vector<int> arr = {1, 2, 3, 4, 5};
    FenwickTree<int> tree(arr);
    
    tree.update(0, 10);
    tree.update(2, 20);
    tree.update(4, 30);
    
    assert(tree.sum_all() == 75);  // 11+2+23+4+35
    END_TEST
}

void test_negative_update() {
    TEST("Negative update (subtraction)")
    std::vector<int> arr = {10, 20, 30};
    FenwickTree<int> tree(arr);
    
    tree.update(1, -5);  // Subtract 5 from arr[1]
    assert(tree.get(1) == 15);
    assert(tree.sum_all() == 55);
    END_TEST
}

// ============================================
// FenwickTree Set Tests
// ============================================

void test_set_value() {
    TEST("Set value")
    std::vector<int> arr = {1, 3, 5, 7, 9};
    FenwickTree<int> tree(arr);
    
    tree.set(2, 100);  // Set arr[2] = 100
    assert(tree.get(2) == 100);
    assert(tree.sum_all() == 120);  // 1+3+100+7+9
    END_TEST
}

void test_set_multiple() {
    TEST("Set multiple values")
    std::vector<int> arr = {1, 2, 3, 4, 5};
    FenwickTree<int> tree(arr);
    
    tree.set(0, 10);
    tree.set(2, 30);
    tree.set(4, 50);
    
    assert(tree.sum_all() == 96);  // 10+2+30+4+50
    END_TEST
}

// ============================================
// FenwickTree Get Tests
// ============================================

void test_get_value() {
    TEST("Get value")
    std::vector<int> arr = {1, 3, 5, 7, 9};
    FenwickTree<int> tree(arr);
    
    assert(tree.get(0) == 1);
    assert(tree.get(2) == 5);
    assert(tree.get(4) == 9);
    END_TEST
}

// ============================================
// FenwickTree Utility Tests
// ============================================

void test_to_vector() {
    TEST("Convert to vector")
    std::vector<int> arr = {1, 3, 5, 7, 9};
    FenwickTree<int> tree(arr);
    
    tree.update(2, 10);
    
    auto result = tree.to_vector();
    assert(result == std::vector<int>({1, 3, 15, 7, 9}));
    END_TEST
}

void test_lower_bound() {
    TEST("Lower bound search")
    std::vector<int> arr = {1, 2, 3, 4, 5};
    FenwickTree<int> tree(arr);
    
    // Prefix sums: 1, 3, 6, 10, 15
    assert(tree.lower_bound(1) == 0);
    assert(tree.lower_bound(3) == 1);
    assert(tree.lower_bound(6) == 2);
    assert(tree.lower_bound(10) == 3);
    END_TEST
}

// ============================================
// RangeUpdateFenwickTree Tests
// ============================================

void test_range_update_constructor() {
    TEST("RangeUpdateFenwickTree constructor")
    std::vector<int> arr = {1, 2, 3, 4, 5};
    RangeUpdateFenwickTree<int> tree(arr);
    
    assert(tree.size() == 5);
    assert(tree.range_sum(0, 4) == 15);
    END_TEST
}

void test_range_update_point() {
    TEST("RangeUpdate: point update")
    std::vector<int> arr = {1, 2, 3, 4, 5};
    RangeUpdateFenwickTree<int> tree(arr);
    
    tree.update(2, 10);  // Add 10 to arr[2]
    assert(tree.get(2) == 13);
    assert(tree.range_sum(0, 4) == 25);
    END_TEST
}

void test_range_update_range() {
    TEST("RangeUpdate: range update")
    std::vector<int> arr = {1, 2, 3, 4, 5};
    RangeUpdateFenwickTree<int> tree(arr);
    
    tree.range_update(1, 3, 10);  // Add 10 to arr[1..3]
    // New: {1, 12, 13, 14, 5}
    
    assert(tree.get(0) == 1);
    assert(tree.get(1) == 12);
    assert(tree.get(2) == 13);
    assert(tree.get(3) == 14);
    assert(tree.get(4) == 5);
    END_TEST
}

void test_range_update_multiple() {
    TEST("RangeUpdate: multiple range updates")
    std::vector<int> arr = {1, 2, 3, 4, 5};
    RangeUpdateFenwickTree<int> tree(arr);
    
    tree.range_update(0, 2, 5);   // Add 5 to arr[0..2]
    tree.range_update(2, 4, 3);   // Add 3 to arr[2..4]
    
    // After first:  {6, 7, 8, 4, 5}
    // After second: {6, 7, 11, 7, 8}
    
    assert(tree.get(0) == 6);
    assert(tree.get(1) == 7);
    assert(tree.get(2) == 11);
    assert(tree.get(3) == 7);
    assert(tree.get(4) == 8);
    END_TEST
}

void test_range_update_set() {
    TEST("RangeUpdate: set value")
    std::vector<int> arr = {1, 2, 3, 4, 5};
    RangeUpdateFenwickTree<int> tree(arr);
    
    tree.set(2, 100);
    assert(tree.get(2) == 100);
    END_TEST
}

void test_range_update_to_vector() {
    TEST("RangeUpdate: to vector")
    std::vector<int> arr = {1, 2, 3, 4, 5};
    RangeUpdateFenwickTree<int> tree(arr);
    
    tree.range_update(1, 3, 10);
    
    auto result = tree.to_vector();
    assert(result == std::vector<int>({1, 12, 13, 14, 5}));
    END_TEST
}

// ============================================
// FenwickTree2D Tests
// ============================================

void test_2d_constructor() {
    TEST("FenwickTree2D constructor")
    std::vector<std::vector<int>> matrix = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };
    FenwickTree2D<int> tree(matrix);
    
    assert(tree.rows() == 3);
    assert(tree.cols() == 3);
    END_TEST
}

void test_2d_prefix_sum() {
    TEST("FenwickTree2D prefix sum")
    std::vector<std::vector<int>> matrix = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };
    FenwickTree2D<int> tree(matrix);
    
    assert(tree.prefix_sum(0, 0) == 1);
    assert(tree.prefix_sum(1, 1) == 12);  // 1+2+4+5
    assert(tree.prefix_sum(2, 2) == 45);  // All elements
    END_TEST
}

void test_2d_range_sum() {
    TEST("FenwickTree2D range sum")
    std::vector<std::vector<int>> matrix = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };
    FenwickTree2D<int> tree(matrix);
    
    assert(tree.range_sum(0, 0, 1, 1) == 12);  // 1+2+4+5
    assert(tree.range_sum(1, 1, 2, 2) == 28);  // 5+6+8+9
    assert(tree.range_sum(0, 0, 2, 2) == 45);  // All
    assert(tree.range_sum(1, 0, 1, 2) == 15);  // 4+5+6
    END_TEST
}

void test_2d_update() {
    TEST("FenwickTree2D update")
    std::vector<std::vector<int>> matrix = {
        {1, 2, 3},
        {4, 5, 6}
    };
    FenwickTree2D<int> tree(matrix);
    
    tree.update(1, 1, 10);  // Add 10 to matrix[1][1]
    assert(tree.get(1, 1) == 15);
    assert(tree.range_sum(0, 0, 1, 2) == 31);
    END_TEST
}

void test_2d_set() {
    TEST("FenwickTree2D set")
    std::vector<std::vector<int>> matrix = {
        {1, 2},
        {3, 4}
    };
    FenwickTree2D<int> tree(matrix);
    
    tree.set(0, 0, 100);
    assert(tree.get(0, 0) == 100);
    assert(tree.range_sum(0, 0, 1, 1) == 109);  // 100+2+3+4
    END_TEST
}

void test_2d_to_matrix() {
    TEST("FenwickTree2D to matrix")
    std::vector<std::vector<int>> matrix = {
        {1, 2, 3},
        {4, 5, 6}
    };
    FenwickTree2D<int> tree(matrix);
    
    tree.update(1, 1, 10);
    
    auto result = tree.to_matrix();
    assert(result[1][1] == 15);
    assert(result[0][0] == 1);
    END_TEST
}

// ============================================
// Edge Cases
// ============================================

void test_single_element() {
    TEST("Single element")
    std::vector<int> arr = {42};
    FenwickTree<int> tree(arr);
    
    assert(tree.get(0) == 42);
    assert(tree.sum_all() == 42);
    
    tree.update(0, 10);
    assert(tree.get(0) == 52);
    END_TEST
}

void test_two_elements() {
    TEST("Two elements")
    std::vector<int> arr = {5, 10};
    FenwickTree<int> tree(arr);
    
    assert(tree.range_sum(0, 1) == 15);
    assert(tree.get(0) == 5);
    assert(tree.get(1) == 10);
    END_TEST
}

void test_negative_numbers() {
    TEST("Negative numbers")
    std::vector<int> arr = {-5, 3, -2, 8, -1};
    FenwickTree<int> tree(arr);
    
    assert(tree.sum_all() == 3);
    assert(tree.range_sum(0, 2) == -4);
    assert(tree.get(0) == -5);
    END_TEST
}

void test_all_zeros() {
    TEST("All zeros")
    FenwickTree<int> tree(10);
    
    assert(tree.sum_all() == 0);
    tree.update(5, 10);
    assert(tree.sum_all() == 10);
    END_TEST
}

void test_out_of_range_throws() {
    TEST("Out of range throws exception")
    std::vector<int> arr = {1, 2, 3};
    FenwickTree<int> tree(arr);
    
    bool caught = false;
    try {
        tree.prefix_sum(5);
    } catch (const std::out_of_range&) {
        caught = true;
    }
    
    assert(caught);
    END_TEST
}

void test_invalid_range_throws() {
    TEST("Invalid range throws exception")
    std::vector<int> arr = {1, 2, 3};
    FenwickTree<int> tree(arr);
    
    bool caught = false;
    try {
        tree.range_sum(2, 1);  // left > right
    } catch (const std::out_of_range&) {
        caught = true;
    }
    
    assert(caught);
    END_TEST
}

// ============================================
// Performance Tests
// ============================================

void test_large_array() {
    TEST("Large array (10000 elements)")
    std::vector<int> arr(10000);
    std::iota(arr.begin(), arr.end(), 1);  // 1, 2, ..., 10000
    
    FenwickTree<int> tree(arr);
    
    // Sum of 1 to 10000 = 10000 * 10001 / 2 = 50005000
    assert(tree.sum_all() == 50005000);
    
    // Update and query
    tree.update(5000, 100000);
    int new_sum = tree.sum_all();
    assert(new_sum == 50105000);
    END_TEST
}

void test_many_operations() {
    TEST("Many operations (1000 queries and updates)")
    std::vector<int> arr(100);
    std::iota(arr.begin(), arr.end(), 1);
    
    FenwickTree<int> tree(arr);
    
    for (int i = 0; i < 1000; ++i) {
        if (i % 2 == 0) {
            tree.update(i % 100, 1);
        } else {
            tree.range_sum(0, i % 100);
        }
    }
    
    assert(tree.size() == 100);
    END_TEST
}

void test_2d_large_matrix() {
    TEST("2D large matrix (100x100)")
    std::vector<std::vector<int>> matrix(100, std::vector<int>(100, 1));
    FenwickTree2D<int> tree(matrix);
    
    assert(tree.range_sum(0, 0, 99, 99) == 10000);
    
    tree.update(50, 50, 99);
    assert(tree.range_sum(0, 0, 99, 99) == 10099);
    END_TEST
}

// ============================================
// Comparison with Naive Implementation
// ============================================

void test_correctness_vs_naive() {
    TEST("Correctness vs naive implementation")
    std::vector<int> arr = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3};
    FenwickTree<int> tree(arr);
    
    // Test all prefix sums
    int sum = 0;
    for (std::size_t i = 0; i < arr.size(); ++i) {
        sum += arr[i];
        assert(tree.prefix_sum(i) == sum);
    }
    
    // Test all range sums
    for (std::size_t i = 0; i < arr.size(); ++i) {
        for (std::size_t j = i; j < arr.size(); ++j) {
            int expected = 0;
            for (std::size_t k = i; k <= j; ++k) {
                expected += arr[k];
            }
            assert(tree.range_sum(i, j) == expected);
        }
    }
    END_TEST
}

// ============================================
// Main Test Runner
// ============================================

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "Fenwick Tree Test Suite" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;
    
    // Constructor tests
    std::cout << "--- FenwickTree Constructor Tests ---" << std::endl;
    test_constructor_from_vector();
    test_constructor_with_size();
    test_constructor_default_zero();
    
    // Prefix sum tests
    std::cout << std::endl << "--- Prefix Sum Tests ---" << std::endl;
    test_prefix_sum();
    test_prefix_sum_single_element();
    
    // Range sum tests
    std::cout << std::endl << "--- Range Sum Tests ---" << std::endl;
    test_range_sum();
    test_sum_all();
    
    // Update tests
    std::cout << std::endl << "--- Update Tests ---" << std::endl;
    test_point_update();
    test_multiple_updates();
    test_negative_update();
    
    // Set tests
    std::cout << std::endl << "--- Set Tests ---" << std::endl;
    test_set_value();
    test_set_multiple();
    
    // Get tests
    std::cout << std::endl << "--- Get Tests ---" << std::endl;
    test_get_value();
    
    // Utility tests
    std::cout << std::endl << "--- Utility Tests ---" << std::endl;
    test_to_vector();
    test_lower_bound();
    
    // RangeUpdateFenwickTree tests
    std::cout << std::endl << "--- RangeUpdateFenwickTree Tests ---" << std::endl;
    test_range_update_constructor();
    test_range_update_point();
    test_range_update_range();
    test_range_update_multiple();
    test_range_update_set();
    test_range_update_to_vector();
    
    // FenwickTree2D tests
    std::cout << std::endl << "--- FenwickTree2D Tests ---" << std::endl;
    test_2d_constructor();
    test_2d_prefix_sum();
    test_2d_range_sum();
    test_2d_update();
    test_2d_set();
    test_2d_to_matrix();
    
    // Edge cases
    std::cout << std::endl << "--- Edge Cases ---" << std::endl;
    test_single_element();
    test_two_elements();
    test_negative_numbers();
    test_all_zeros();
    test_out_of_range_throws();
    test_invalid_range_throws();
    
    // Performance tests
    std::cout << std::endl << "--- Performance Tests ---" << std::endl;
    test_large_array();
    test_many_operations();
    test_2d_large_matrix();
    
    // Correctness tests
    std::cout << std::endl << "--- Correctness Tests ---" << std::endl;
    test_correctness_vs_naive();
    
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