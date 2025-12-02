/**
 * @file test_segment_tree.cpp
 * @brief Test suite for Segment Tree data structure
 * @author Jinhyeok
 * @date 2025-12-02
 * @version 1.0.0
 */

#include "tree/segment_tree.hpp"
#include <iostream>
#include <cassert>
#include <vector>
#include <algorithm>
#include <numeric>
#include <random>

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
// Constructor Tests
// ============================================

void test_constructor_from_vector() {
    TEST("Constructor from vector")
    std::vector<int> arr = {1, 3, 5, 7, 9};
    SegmentTree<int> tree(arr);
    
    assert(tree.size() == 5);
    assert(!tree.empty());
    END_TEST
}

void test_constructor_with_size() {
    TEST("Constructor with size")
    SegmentTree<int> tree(10, 5);
    
    assert(tree.size() == 10);
    assert(tree.query_all() == 50);  // 10 * 5
    END_TEST
}

void test_empty_array_throws() {
    TEST("Empty array throws exception")
    std::vector<int> arr;
    
    bool caught = false;
    try {
        SegmentTree<int> tree(arr);
    } catch (const std::invalid_argument&) {
        caught = true;
    }
    
    assert(caught);
    END_TEST
}

// ============================================
// Factory Method Tests
// ============================================

void test_create_sum() {
    TEST("Create sum tree")
    std::vector<int> arr = {1, 2, 3, 4, 5};
    auto tree = create_sum_segment_tree(arr);
    
    assert(tree.query_all() == 15);
    END_TEST
}

void test_create_min() {
    TEST("Create min tree")
    std::vector<int> arr = {5, 2, 8, 1, 9};
    auto tree = create_min_segment_tree(arr);
    
    assert(tree.query_all() == 1);
    END_TEST
}

void test_create_max() {
    TEST("Create max tree")
    std::vector<int> arr = {5, 2, 8, 1, 9};
    auto tree = create_max_segment_tree(arr);
    
    assert(tree.query_all() == 9);
    END_TEST
}

void test_create_gcd() {
    TEST("Create GCD tree")
    std::vector<int> arr = {12, 18, 24, 30};
    auto tree = create_gcd_segment_tree(arr);
    
    assert(tree.query_all() == 6);
    END_TEST
}

// ============================================
// Range Query Tests - Sum
// ============================================

void test_range_sum_query() {
    TEST("Range sum query")
    std::vector<int> arr = {1, 3, 5, 7, 9, 11};
    auto tree = create_sum_segment_tree(arr);
    
    assert(tree.query(0, 5) == 36);  // Full range
    assert(tree.query(1, 4) == 24);  // 3+5+7+9
    assert(tree.query(2, 3) == 12);  // 5+7
    END_TEST
}

void test_single_element_query() {
    TEST("Single element query")
    std::vector<int> arr = {1, 3, 5, 7, 9};
    auto tree = create_sum_segment_tree(arr);
    
    assert(tree.query(0) == 1);
    assert(tree.query(2) == 5);
    assert(tree.query(4) == 9);
    END_TEST
}

void test_query_entire_array() {
    TEST("Query entire array")
    std::vector<int> arr = {1, 2, 3, 4, 5};
    auto tree = create_sum_segment_tree(arr);
    
    assert(tree.query_all() == 15);
    assert(tree.query(0, 4) == 15);
    END_TEST
}

// ============================================
// Range Query Tests - Min/Max
// ============================================

void test_range_min_query() {
    TEST("Range minimum query")
    std::vector<int> arr = {5, 2, 8, 1, 9, 3};
    auto tree = create_min_segment_tree(arr);
    
    assert(tree.query(0, 5) == 1);
    assert(tree.query(0, 2) == 2);
    assert(tree.query(3, 5) == 1);
    assert(tree.query(4, 5) == 3);
    END_TEST
}

void test_range_max_query() {
    TEST("Range maximum query")
    std::vector<int> arr = {5, 2, 8, 1, 9, 3};
    auto tree = create_max_segment_tree(arr);
    
    assert(tree.query(0, 5) == 9);
    assert(tree.query(0, 2) == 8);
    assert(tree.query(3, 5) == 9);
    assert(tree.query(0, 1) == 5);
    END_TEST
}

void test_range_gcd_query() {
    TEST("Range GCD query")
    std::vector<int> arr = {12, 18, 24, 30, 36};
    auto tree = create_gcd_segment_tree(arr);
    
    assert(tree.query(0, 4) == 6);
    assert(tree.query(0, 2) == 6);
    assert(tree.query(2, 4) == 6);
    END_TEST
}

// ============================================
// Point Update Tests
// ============================================

void test_point_update_sum() {
    TEST("Point update for sum tree")
    std::vector<int> arr = {1, 3, 5, 7, 9};
    auto tree = create_sum_segment_tree(arr);
    
    assert(tree.query_all() == 25);
    
    tree.update(2, 10);  // Change 5 to 10
    assert(tree.query_all() == 30);
    assert(tree.query(2) == 10);
    assert(tree.query(0, 2) == 14);  // 1+3+10
    END_TEST
}

void test_point_update_min() {
    TEST("Point update for min tree")
    std::vector<int> arr = {5, 2, 8, 1, 9};
    auto tree = create_min_segment_tree(arr);
    
    assert(tree.query_all() == 1);
    
    tree.update(3, 10);  // Change 1 to 10
    assert(tree.query_all() == 2);
    assert(tree.query(3, 4) == 9);
    END_TEST
}

void test_point_update_max() {
    TEST("Point update for max tree")
    std::vector<int> arr = {5, 2, 8, 1, 9};
    auto tree = create_max_segment_tree(arr);
    
    assert(tree.query_all() == 9);
    
    tree.update(4, 3);  // Change 9 to 3
    assert(tree.query_all() == 8);
    assert(tree.query(3, 4) == 3);
    END_TEST
}

void test_multiple_updates() {
    TEST("Multiple point updates")
    std::vector<int> arr = {1, 2, 3, 4, 5};
    auto tree = create_sum_segment_tree(arr);
    
    tree.update(0, 10);
    tree.update(2, 30);
    tree.update(4, 50);
    
    assert(tree.query_all() == 96);  // 10+2+30+4+50
    assert(tree.query(0, 2) == 42);  // 10+2+30
    END_TEST
}

// ============================================
// Range Update Tests (Lazy Propagation)
// ============================================

void test_range_update_with_lazy() {
    TEST("Range update with lazy propagation")
    std::vector<int> arr = {1, 2, 3, 4, 5};
    auto tree = create_sum_segment_tree(arr, true);
    
    assert(tree.has_lazy_propagation());
    assert(tree.query_all() == 15);
    
    tree.range_update(1, 3, 10);  // Add 10 to arr[1..3]
    // New: {1, 12, 13, 14, 5}
    
    assert(tree.query_all() == 45);
    assert(tree.query(1, 3) == 39);  // 12+13+14
    END_TEST
}

void test_multiple_range_updates() {
    TEST("Multiple range updates")
    std::vector<int> arr = {1, 2, 3, 4, 5};
    auto tree = create_sum_segment_tree(arr, true);
    
    tree.range_update(0, 2, 5);   // Add 5 to arr[0..2]
    tree.range_update(2, 4, 3);   // Add 3 to arr[2..4]
    
    // After first:  {6, 7, 8, 4, 5}
    // After second: {6, 7, 11, 7, 8}
    
    assert(tree.query_all() == 39);
    assert(tree.query(2) == 11);
    END_TEST
}

void test_range_update_without_lazy_throws() {
    TEST("Range update without lazy throws")
    std::vector<int> arr = {1, 2, 3};
    auto tree = create_sum_segment_tree(arr, false);
    
    bool caught = false;
    try {
        tree.range_update(0, 2, 5);
    } catch (const std::runtime_error&) {
        caught = true;
    }
    
    assert(caught);
    END_TEST
}

// ============================================
// Specialized Tree Tests
// ============================================

void test_range_sum_tree_class() {
    TEST("RangeSumSegmentTree class")
    std::vector<int> arr = {1, 2, 3, 4, 5};
    RangeSumSegmentTree<int> tree(arr);
    
    assert(tree.query_all() == 15);
    tree.range_update(0, 2, 10);
    assert(tree.query_all() == 45);
    END_TEST
}

void test_range_min_tree_class() {
    TEST("RangeMinSegmentTree class")
    std::vector<int> arr = {5, 2, 8, 1, 9};
    RangeMinSegmentTree<int> tree(arr);
    
    assert(tree.query(0, 4) == 1);
    tree.update(3, 10);
    assert(tree.query(0, 4) == 2);
    END_TEST
}

void test_range_max_tree_class() {
    TEST("RangeMaxSegmentTree class")
    std::vector<int> arr = {5, 2, 8, 1, 9};
    RangeMaxSegmentTree<int> tree(arr);
    
    assert(tree.query(0, 4) == 9);
    tree.update(4, 3);
    assert(tree.query(0, 4) == 8);
    END_TEST
}

void test_range_gcd_tree_class() {
    TEST("RangeGCDSegmentTree class")
    std::vector<int> arr = {12, 18, 24};
    RangeGCDSegmentTree<int> tree(arr);
    
    assert(tree.query_all() == 6);
    END_TEST
}

// ============================================
// Edge Cases
// ============================================

void test_single_element_tree() {
    TEST("Single element tree")
    std::vector<int> arr = {42};
    auto tree = create_sum_segment_tree(arr);
    
    assert(tree.size() == 1);
    assert(tree.query(0) == 42);
    assert(tree.query_all() == 42);
    
    tree.update(0, 100);
    assert(tree.query_all() == 100);
    END_TEST
}

void test_two_element_tree() {
    TEST("Two element tree")
    std::vector<int> arr = {5, 10};
    auto tree = create_sum_segment_tree(arr);
    
    assert(tree.query_all() == 15);
    assert(tree.query(0, 0) == 5);
    assert(tree.query(1, 1) == 10);
    END_TEST
}

void test_all_same_elements() {
    TEST("All same elements")
    std::vector<int> arr(10, 5);
    auto tree = create_sum_segment_tree(arr);
    
    assert(tree.query_all() == 50);
    assert(tree.query(0, 4) == 25);
    assert(tree.query(3, 7) == 25);
    END_TEST
}

void test_negative_numbers() {
    TEST("Negative numbers")
    std::vector<int> arr = {-5, 3, -2, 8, -1};
    auto tree = create_sum_segment_tree(arr);
    
    assert(tree.query_all() == 3);
    assert(tree.query(0, 2) == -4);
    
    auto min_tree = create_min_segment_tree(arr);
    assert(min_tree.query(0, 4) == -5);
    END_TEST
}

void test_invalid_range_throws() {
    TEST("Invalid range throws exception")
    std::vector<int> arr = {1, 2, 3};
    auto tree = create_sum_segment_tree(arr);
    
    bool caught = false;
    try {
        tree.query(2, 1);  // left > right
    } catch (const std::out_of_range&) {
        caught = true;
    }
    
    assert(caught);
    END_TEST
}

void test_out_of_range_throws() {
    TEST("Out of range throws exception")
    std::vector<int> arr = {1, 2, 3};
    auto tree = create_sum_segment_tree(arr);
    
    bool caught = false;
    try {
        tree.query(0, 5);  // Beyond array size
    } catch (const std::out_of_range&) {
        caught = true;
    }
    
    assert(caught);
    END_TEST
}

// ============================================
// Conversion Tests
// ============================================

void test_to_vector() {
    TEST("Convert to vector")
    std::vector<int> arr = {1, 2, 3, 4, 5};
    auto tree = create_sum_segment_tree(arr);
    
    tree.update(2, 10);
    
    auto result = tree.to_vector();
    assert(result == std::vector<int>({1, 2, 10, 4, 5}));
    END_TEST
}

void test_to_vector_after_range_update() {
    TEST("Convert to vector after range update")
    std::vector<int> arr = {1, 2, 3, 4, 5};
    auto tree = create_sum_segment_tree(arr, true);
    
    tree.range_update(1, 3, 10);
    
    auto result = tree.to_vector();
    assert(result == std::vector<int>({1, 12, 13, 14, 5}));
    END_TEST
}

// ============================================
// Performance Tests
// ============================================

void test_large_array() {
    TEST("Large array (10000 elements)")
    std::vector<int> arr(10000);
    std::iota(arr.begin(), arr.end(), 1);  // 1, 2, 3, ..., 10000
    
    auto tree = create_sum_segment_tree(arr);
    
    // Sum of 1 to 10000 = 10000 * 10001 / 2 = 50005000
    assert(tree.query_all() == 50005000);
    
    // Query middle range
    int mid_sum = tree.query(2500, 7500);
    assert(mid_sum > 0);
    
    // Update and query
    tree.update(5000, 100000);
    assert(tree.query(5000) == 100000);
    END_TEST
}

void test_many_queries() {
    TEST("Many queries (1000 operations)")
    std::vector<int> arr(100);
    std::iota(arr.begin(), arr.end(), 1);
    
    auto tree = create_sum_segment_tree(arr);
    
    for (int i = 0; i < 1000; ++i) {
        int left = i % 50;
        int right = left + (i % 30);
        if (right < 100) {
            tree.query(left, right);
        }
    }
    
    assert(tree.size() == 100);
    END_TEST
}

void test_many_updates() {
    TEST("Many updates (1000 operations)")
    std::vector<int> arr(100, 1);
    auto tree = create_sum_segment_tree(arr);
    
    for (int i = 0; i < 1000; ++i) {
        tree.update(i % 100, i);
    }
    
    assert(tree.size() == 100);
    END_TEST
}

// ============================================
// Mixed Operations Tests
// ============================================

void test_mixed_operations() {
    TEST("Mixed queries and updates")
    std::vector<int> arr = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    auto tree = create_sum_segment_tree(arr, true);
    
    assert(tree.query_all() == 55);
    
    tree.update(0, 10);
    assert(tree.query(0, 2) == 15);  // 10+2+3
    
    tree.range_update(5, 7, 5);
    // arr[5]=6+5=11, arr[6]=7+5=12, arr[7]=8+5=13
    assert(tree.query(5, 7) == 36);  // 11+12+13
    
    tree.update(9, 1);
    // Total: 10+2+3+4+5+11+12+13+9+1 = 70
    int total = tree.query_all();
    assert(total == 70);
    END_TEST
}

void test_alternating_operations() {
    TEST("Alternating operations")
    std::vector<int> arr(10, 1);
    auto tree = create_sum_segment_tree(arr, true);
    
    for (int i = 0; i < 5; ++i) {
        tree.update(i, i * 2);
        int sum = tree.query(0, i);
        assert(sum >= 0);
        
        if (tree.has_lazy_propagation()) {
            tree.range_update(i, i + 1, 1);
        }
    }
    
    assert(tree.size() == 10);
    END_TEST
}

// ============================================
// Custom Operation Tests
// ============================================

void test_custom_multiply_operation() {
    TEST("Custom multiply operation")
    std::vector<int> arr = {2, 3, 4, 5};
    
    using FuncType = std::function<int(const int&, const int&)>;
    SegmentTree<int, FuncType> tree(arr, 
        [](int a, int b) { return a * b; },  // Multiply merge
        1);                                   // Identity is 1 for multiplication
    
    assert(tree.query_all() == 120);  // 2*3*4*5
    assert(tree.query(0, 1) == 6);    // 2*3
    assert(tree.query(2, 3) == 20);   // 4*5
    END_TEST
}

// ============================================
// Stress Tests
// ============================================

void test_stress_random_operations() {
    TEST("Stress test with random operations")
    std::vector<int> arr(50);
    std::iota(arr.begin(), arr.end(), 1);
    
    auto tree = create_sum_segment_tree(arr, true);
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 49);
    
    for (int i = 0; i < 100; ++i) {
        int left = dist(gen);
        int right = dist(gen);
        if (left > right) std::swap(left, right);
        
        if (i % 3 == 0) {
            tree.query(left, right);
        } else if (i % 3 == 1) {
            tree.update(left, dist(gen));
        } else {
            tree.range_update(left, right, dist(gen) % 10);
        }
    }
    
    assert(tree.size() == 50);
    END_TEST
}

// ============================================
// Main Test Runner
// ============================================

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "Segment Tree Test Suite" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;
    
    // Constructor tests
    std::cout << "--- Constructor Tests ---" << std::endl;
    test_constructor_from_vector();
    test_constructor_with_size();
    test_empty_array_throws();
    
    // Factory method tests
    std::cout << std::endl << "--- Factory Method Tests ---" << std::endl;
    test_create_sum();
    test_create_min();
    test_create_max();
    test_create_gcd();
    
    // Range query tests - Sum
    std::cout << std::endl << "--- Range Query Tests (Sum) ---" << std::endl;
    test_range_sum_query();
    test_single_element_query();
    test_query_entire_array();
    
    // Range query tests - Min/Max
    std::cout << std::endl << "--- Range Query Tests (Min/Max/GCD) ---" << std::endl;
    test_range_min_query();
    test_range_max_query();
    test_range_gcd_query();
    
    // Point update tests
    std::cout << std::endl << "--- Point Update Tests ---" << std::endl;
    test_point_update_sum();
    test_point_update_min();
    test_point_update_max();
    test_multiple_updates();
    
    // Range update tests
    std::cout << std::endl << "--- Range Update Tests (Lazy Propagation) ---" << std::endl;
    test_range_update_with_lazy();
    test_multiple_range_updates();
    test_range_update_without_lazy_throws();
    
    // Specialized tree tests
    std::cout << std::endl << "--- Specialized Tree Tests ---" << std::endl;
    test_range_sum_tree_class();
    test_range_min_tree_class();
    test_range_max_tree_class();
    test_range_gcd_tree_class();
    
    // Edge cases
    std::cout << std::endl << "--- Edge Cases ---" << std::endl;
    test_single_element_tree();
    test_two_element_tree();
    test_all_same_elements();
    test_negative_numbers();
    test_invalid_range_throws();
    test_out_of_range_throws();
    
    // Conversion tests
    std::cout << std::endl << "--- Conversion Tests ---" << std::endl;
    test_to_vector();
    test_to_vector_after_range_update();
    
    // Performance tests
    std::cout << std::endl << "--- Performance Tests ---" << std::endl;
    test_large_array();
    test_many_queries();
    test_many_updates();
    
    // Mixed operations
    std::cout << std::endl << "--- Mixed Operations ---" << std::endl;
    test_mixed_operations();
    test_alternating_operations();
    
    // Custom operations
    std::cout << std::endl << "--- Custom Operations ---" << std::endl;
    test_custom_multiply_operation();
    
    // Stress tests
    std::cout << std::endl << "--- Stress Tests ---" << std::endl;
    test_stress_random_operations();
    
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