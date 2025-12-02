/**
 * @file test_skip_list.cpp
 * @brief Test suite for Skip List data structure
 * @author Jinhyeok
 * @date 2025-12-02
 * @version 1.0.0
 */

#include "tree/skip_list.hpp"
#include <iostream>
#include <cassert>
#include <vector>
#include <string>
#include <algorithm>
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
// Constructor Tests
// ============================================

void test_default_constructor() {
    TEST("Default constructor")
    SkipList<int> list;
    
    assert(list.empty());
    assert(list.size() == 0);
    assert(list.max_level() == 16);
    END_TEST
}

void test_initializer_list_constructor() {
    TEST("Initializer list constructor")
    SkipList<int> list = {5, 2, 8, 1, 9, 3};
    
    assert(list.size() == 6);
    assert(list.contains(5));
    assert(list.contains(1));
    assert(list.contains(9));
    END_TEST
}

void test_range_constructor() {
    TEST("Range constructor")
    std::vector<int> vec = {4, 2, 7, 1, 9};
    SkipList<int> list(vec.begin(), vec.end());
    
    assert(list.size() == 5);
    for (int val : vec) {
        assert(list.contains(val));
    }
    END_TEST
}

void test_copy_constructor() {
    TEST("Copy constructor")
    SkipList<int> list1 = {1, 2, 3, 4, 5};
    SkipList<int> list2(list1);
    
    assert(list2.size() == 5);
    assert(list2.contains(3));
    
    list2.insert(6);
    assert(list2.contains(6));
    assert(!list1.contains(6));
    END_TEST
}

void test_move_constructor() {
    TEST("Move constructor")
    SkipList<int> list1 = {1, 2, 3, 4, 5};
    SkipList<int> list2(std::move(list1));
    
    assert(list2.size() == 5);
    assert(list1.size() == 0);
    END_TEST
}

void test_custom_parameters() {
    TEST("Custom max_level and probability")
    SkipList<int> list(8, 0.25);
    
    assert(list.max_level() == 8);
    assert(list.empty());
    
    for (int i = 0; i < 100; ++i) {
        list.insert(i);
    }
    
    assert(list.size() == 100);
    END_TEST
}

// ============================================
// Insert Tests
// ============================================

void test_basic_insert() {
    TEST("Basic insert")
    SkipList<int> list;
    
    assert(list.insert(5));
    assert(list.insert(3));
    assert(list.insert(7));
    
    assert(list.size() == 3);
    assert(list.contains(5));
    assert(list.contains(3));
    assert(list.contains(7));
    END_TEST
}

void test_duplicate_insert() {
    TEST("Duplicate insert returns false")
    SkipList<int> list;
    
    assert(list.insert(5));
    assert(!list.insert(5));  // Duplicate
    
    assert(list.size() == 1);
    END_TEST
}

void test_insert_ascending() {
    TEST("Insert in ascending order")
    SkipList<int> list;
    
    for (int i = 1; i <= 10; ++i) {
        list.insert(i);
    }
    
    assert(list.size() == 10);
    for (int i = 1; i <= 10; ++i) {
        assert(list.contains(i));
    }
    END_TEST
}

void test_insert_descending() {
    TEST("Insert in descending order")
    SkipList<int> list;
    
    for (int i = 10; i >= 1; --i) {
        list.insert(i);
    }
    
    assert(list.size() == 10);
    for (int i = 1; i <= 10; ++i) {
        assert(list.contains(i));
    }
    END_TEST
}

void test_insert_random() {
    TEST("Insert random values")
    SkipList<int> list;
    std::vector<int> values = {15, 3, 9, 1, 22, 7, 18, 5, 12, 20};
    
    for (int val : values) {
        list.insert(val);
    }
    
    assert(list.size() == 10);
    for (int val : values) {
        assert(list.contains(val));
    }
    END_TEST
}

// ============================================
// Remove Tests
// ============================================

void test_basic_remove() {
    TEST("Basic remove")
    SkipList<int> list = {1, 2, 3, 4, 5};
    
    assert(list.remove(3));
    assert(list.size() == 4);
    assert(!list.contains(3));
    
    assert(list.contains(2));
    assert(list.contains(4));
    END_TEST
}

void test_remove_nonexistent() {
    TEST("Remove nonexistent element")
    SkipList<int> list = {1, 2, 3};
    
    assert(!list.remove(5));
    assert(list.size() == 3);
    END_TEST
}

void test_remove_all() {
    TEST("Remove all elements")
    SkipList<int> list = {1, 2, 3, 4, 5};
    
    for (int i = 1; i <= 5; ++i) {
        assert(list.remove(i));
    }
    
    assert(list.empty());
    END_TEST
}

void test_remove_first_last() {
    TEST("Remove first and last elements")
    SkipList<int> list = {1, 2, 3, 4, 5};
    
    assert(list.remove(1));  // First
    assert(list.remove(5));  // Last
    
    assert(list.size() == 3);
    assert(list.contains(2));
    assert(list.contains(4));
    END_TEST
}

// ============================================
// Find/Contains Tests
// ============================================

void test_find_existing() {
    TEST("Find existing elements")
    SkipList<int> list = {5, 2, 8, 1, 9};
    
    assert(list.find(5));
    assert(list.find(1));
    assert(list.find(9));
    END_TEST
}

void test_find_nonexistent() {
    TEST("Find nonexistent elements")
    SkipList<int> list = {1, 2, 3};
    
    assert(!list.find(0));
    assert(!list.find(4));
    assert(!list.find(10));
    END_TEST
}

void test_contains() {
    TEST("Contains method")
    SkipList<int> list = {10, 20, 30};
    
    assert(list.contains(10));
    assert(list.contains(20));
    assert(!list.contains(15));
    END_TEST
}

// ============================================
// Iterator Tests
// ============================================

void test_iterator_traversal() {
    TEST("Iterator traversal")
    SkipList<int> list = {5, 2, 8, 1, 9, 3};
    
    std::vector<int> result;
    for (const auto& val : list) {
        result.push_back(val);
    }
    
    // Should be in sorted order
    assert(result == std::vector<int>({1, 2, 3, 5, 8, 9}));
    END_TEST
}

void test_iterator_empty() {
    TEST("Iterator on empty list")
    SkipList<int> list;
    
    int count = 0;
    for (const auto& val : list) {
        ++count;
        (void)val;
    }
    
    assert(count == 0);
    END_TEST
}

void test_iterator_single_element() {
    TEST("Iterator with single element")
    SkipList<int> list = {42};
    
    std::vector<int> result;
    for (const auto& val : list) {
        result.push_back(val);
    }
    
    assert(result == std::vector<int>({42}));
    END_TEST
}

void test_const_iterator() {
    TEST("Const iterator")
    const SkipList<int> list = {1, 2, 3, 4, 5};
    
    std::vector<int> result;
    for (auto it = list.cbegin(); it != list.cend(); ++it) {
        result.push_back(*it);
    }
    
    assert(result.size() == 5);
    assert(result[0] == 1);
    assert(result[4] == 5);
    END_TEST
}

// ============================================
// Lower/Upper Bound Tests
// ============================================

void test_lower_bound() {
    TEST("Lower bound")
    SkipList<int> list = {1, 3, 5, 7, 9};
    
    assert(list.lower_bound(0).value() == 1);
    assert(list.lower_bound(3).value() == 3);
    assert(list.lower_bound(4).value() == 5);
    assert(list.lower_bound(9).value() == 9);
    assert(!list.lower_bound(10).has_value());
    END_TEST
}

void test_upper_bound() {
    TEST("Upper bound")
    SkipList<int> list = {1, 3, 5, 7, 9};
    
    assert(list.upper_bound(0).value() == 1);
    assert(list.upper_bound(1).value() == 3);
    assert(list.upper_bound(5).value() == 7);
    assert(list.upper_bound(8).value() == 9);
    assert(!list.upper_bound(9).has_value());
    END_TEST
}

// ============================================
// Assignment Tests
// ============================================

void test_copy_assignment() {
    TEST("Copy assignment")
    SkipList<int> list1 = {1, 2, 3};
    SkipList<int> list2;
    
    list2 = list1;
    
    assert(list2.size() == 3);
    assert(list2.contains(2));
    
    list2.insert(4);
    assert(!list1.contains(4));
    END_TEST
}

void test_move_assignment() {
    TEST("Move assignment")
    SkipList<int> list1 = {1, 2, 3, 4, 5};
    SkipList<int> list2;
    
    list2 = std::move(list1);
    
    assert(list2.size() == 5);
    assert(list1.size() == 0);
    END_TEST
}

// ============================================
// Utility Tests
// ============================================

void test_clear() {
    TEST("Clear list")
    SkipList<int> list = {1, 2, 3, 4, 5};
    
    list.clear();
    
    assert(list.empty());
    assert(list.size() == 0);
    assert(!list.contains(3));
    END_TEST
}

void test_swap() {
    TEST("Swap lists")
    SkipList<int> list1 = {1, 2, 3};
    SkipList<int> list2 = {10, 20};
    
    list1.swap(list2);
    
    assert(list1.size() == 2);
    assert(list2.size() == 3);
    assert(list1.contains(10));
    assert(list2.contains(2));
    END_TEST
}

void test_to_vector() {
    TEST("Convert to vector")
    SkipList<int> list = {5, 2, 8, 1, 9};
    
    auto vec = list.to_vector();
    
    assert(vec == std::vector<int>({1, 2, 5, 8, 9}));
    END_TEST
}

void test_level_distribution() {
    TEST("Level distribution")
    SkipList<int> list;
    
    for (int i = 0; i < 100; ++i) {
        list.insert(i);
    }
    
    auto dist = list.level_distribution();
    
    // Level 0 should have all elements
    assert(dist[0] == 100);
    
    // Higher levels should have fewer elements
    if (dist.size() > 1) {
        assert(dist[1] < dist[0]);
    }
    END_TEST
}

// ============================================
// Edge Cases
// ============================================

void test_single_element() {
    TEST("Single element operations")
    SkipList<int> list;
    
    list.insert(42);
    assert(list.size() == 1);
    assert(list.contains(42));
    
    list.remove(42);
    assert(list.empty());
    END_TEST
}

void test_two_elements() {
    TEST("Two element operations")
    SkipList<int> list = {10, 20};
    
    assert(list.size() == 2);
    
    list.remove(10);
    assert(list.size() == 1);
    assert(list.contains(20));
    END_TEST
}

void test_negative_numbers() {
    TEST("Negative numbers")
    SkipList<int> list = {-5, 3, -2, 8, -1};
    
    assert(list.size() == 5);
    assert(list.contains(-5));
    assert(list.contains(-1));
    
    auto vec = list.to_vector();
    assert(vec[0] == -5);
    assert(vec[1] == -2);
    END_TEST
}

void test_duplicate_operations() {
    TEST("Multiple duplicate attempts")
    SkipList<int> list;
    
    assert(list.insert(5));
    assert(!list.insert(5));
    assert(!list.insert(5));
    
    assert(list.size() == 1);
    END_TEST
}

// ============================================
// Custom Comparator Tests
// ============================================

void test_descending_order() {
    TEST("Descending order (custom comparator)")
    SkipList<int, std::greater<int>> list;
    
    list.insert(5);
    list.insert(2);
    list.insert(8);
    
    auto vec = list.to_vector();
    assert(vec == std::vector<int>({8, 5, 2}));
    END_TEST
}

void test_string_skip_list() {
    TEST("String skip list")
    SkipList<std::string> list;
    
    list.insert("apple");
    list.insert("banana");
    list.insert("cherry");
    
    assert(list.contains("banana"));
    assert(!list.contains("grape"));
    
    auto vec = list.to_vector();
    assert(vec[0] == "apple");
    assert(vec[2] == "cherry");
    END_TEST
}

// ============================================
// Performance Tests
// ============================================

void test_large_insertion() {
    TEST("Large insertion (1000 elements)")
    SkipList<int> list;
    
    for (int i = 0; i < 1000; ++i) {
        list.insert(i);
    }
    
    assert(list.size() == 1000);
    assert(list.contains(500));
    assert(list.contains(999));
    END_TEST
}

void test_many_operations() {
    TEST("Many operations (insert, find, remove)")
    SkipList<int> list;
    
    // Insert
    for (int i = 0; i < 500; ++i) {
        list.insert(i * 2);
    }
    
    // Find
    for (int i = 0; i < 500; ++i) {
        assert(list.contains(i * 2));
    }
    
    // Remove half
    for (int i = 0; i < 250; ++i) {
        assert(list.remove(i * 2));
    }
    
    assert(list.size() == 250);
    END_TEST
}

void test_random_operations() {
    TEST("Random operations")
    SkipList<int> list;
    std::vector<int> values;
    
    // Insert random values
    for (int i = 0; i < 100; ++i) {
        int val = rand() % 1000;
        if (list.insert(val)) {
            values.push_back(val);
        }
    }
    
    // Verify all inserted values exist
    for (int val : values) {
        assert(list.contains(val));
    }
    
    // Remove some random values
    for (std::size_t i = 0; i < values.size() / 2; ++i) {
        list.remove(values[i]);
    }
    
    assert(list.size() == values.size() - values.size() / 2);
    END_TEST
}

// ============================================
// Probabilistic Properties Tests
// ============================================

void test_level_distribution_properties() {
    TEST("Level distribution follows geometric distribution")
    SkipList<int> list(16, 0.5);
    
    for (int i = 0; i < 1000; ++i) {
        list.insert(i);
    }
    
    auto dist = list.level_distribution();
    
    // Each level should have approximately half of the previous level
    // Allow for statistical variation
    for (std::size_t i = 1; i < dist.size(); ++i) {
        double ratio = static_cast<double>(dist[i]) / dist[i-1];
        // Ratio should be around 0.5, but allow wide margin
        assert(ratio >= 0.2 && ratio <= 0.8);
    }
    END_TEST
}

void test_expected_levels() {
    TEST("Expected number of levels")
    SkipList<int> list(16, 0.5);
    
    for (int i = 0; i < 1000; ++i) {
        list.insert(i);
    }
    
    // For 1000 elements with p=0.5, expected max level is ~log2(1000) ≈ 10
    assert(list.current_level() >= 5);
    assert(list.current_level() <= 15);
    END_TEST
}

// ============================================
// Main Test Runner
// ============================================

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "Skip List Test Suite" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;
    
    // Constructor tests
    std::cout << "--- Constructor Tests ---" << std::endl;
    test_default_constructor();
    test_initializer_list_constructor();
    test_range_constructor();
    test_copy_constructor();
    test_move_constructor();
    test_custom_parameters();
    
    // Insert tests
    std::cout << std::endl << "--- Insert Tests ---" << std::endl;
    test_basic_insert();
    test_duplicate_insert();
    test_insert_ascending();
    test_insert_descending();
    test_insert_random();
    
    // Remove tests
    std::cout << std::endl << "--- Remove Tests ---" << std::endl;
    test_basic_remove();
    test_remove_nonexistent();
    test_remove_all();
    test_remove_first_last();
    
    // Find/Contains tests
    std::cout << std::endl << "--- Find/Contains Tests ---" << std::endl;
    test_find_existing();
    test_find_nonexistent();
    test_contains();
    
    // Iterator tests
    std::cout << std::endl << "--- Iterator Tests ---" << std::endl;
    test_iterator_traversal();
    test_iterator_empty();
    test_iterator_single_element();
    test_const_iterator();
    
    // Lower/Upper bound tests
    std::cout << std::endl << "--- Lower/Upper Bound Tests ---" << std::endl;
    test_lower_bound();
    test_upper_bound();
    
    // Assignment tests
    std::cout << std::endl << "--- Assignment Tests ---" << std::endl;
    test_copy_assignment();
    test_move_assignment();
    
    // Utility tests
    std::cout << std::endl << "--- Utility Tests ---" << std::endl;
    test_clear();
    test_swap();
    test_to_vector();
    test_level_distribution();
    
    // Edge cases
    std::cout << std::endl << "--- Edge Cases ---" << std::endl;
    test_single_element();
    test_two_elements();
    test_negative_numbers();
    test_duplicate_operations();
    
    // Custom comparator tests
    std::cout << std::endl << "--- Custom Comparator Tests ---" << std::endl;
    test_descending_order();
    test_string_skip_list();
    
    // Performance tests
    std::cout << std::endl << "--- Performance Tests ---" << std::endl;
    test_large_insertion();
    test_many_operations();
    test_random_operations();
    
    // Probabilistic properties
    std::cout << std::endl << "--- Probabilistic Properties ---" << std::endl;
    test_level_distribution_properties();
    test_expected_levels();
    
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