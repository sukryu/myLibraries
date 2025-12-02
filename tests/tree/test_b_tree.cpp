/**
 * @file test_b_tree.cpp
 * @brief Test suite for B-Tree data structure
 * @author Jinhyeok
 * @date 2025-12-02
 * @version 1.0.0
 */

#include "tree/b_tree.hpp"
#include <iostream>
#include <cassert>
#include <vector>
#include <algorithm>
#include <random>
#include <set>

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
    BTree<int> tree(3);
    
    assert(tree.empty());
    assert(tree.size() == 0);
    assert(tree.min_degree() == 3);
    assert(tree.max_keys() == 5);
    assert(tree.max_children() == 6);
    END_TEST
}

void test_initializer_list_constructor() {
    TEST("Initializer list constructor")
    BTree<int> tree({10, 20, 5, 15, 25}, 3);
    
    assert(tree.size() == 5);
    assert(tree.search(10));
    assert(tree.search(20));
    assert(tree.search(5));
    assert(tree.verify());
    END_TEST
}

void test_range_constructor() {
    TEST("Range constructor")
    std::vector<int> values = {1, 2, 3, 4, 5};
    BTree<int> tree(values.begin(), values.end(), 2);
    
    assert(tree.size() == 5);
    for (int val : values) {
        assert(tree.search(val));
    }
    END_TEST
}

void test_copy_constructor() {
    TEST("Copy constructor")
    BTree<int> tree1({1, 2, 3, 4, 5}, 2);
    BTree<int> tree2(tree1);
    
    assert(tree1.size() == tree2.size());
    assert(tree2.search(1));
    assert(tree2.search(5));
    assert(tree2.verify());
    END_TEST
}

void test_move_constructor() {
    TEST("Move constructor")
    BTree<int> tree1({1, 2, 3, 4, 5}, 2);
    std::size_t original_size = tree1.size();
    
    BTree<int> tree2(std::move(tree1));
    
    assert(tree2.size() == original_size);
    assert(tree1.size() == 0);
    assert(tree2.search(3));
    END_TEST
}

// ============================================
// Assignment Tests
// ============================================

void test_copy_assignment() {
    TEST("Copy assignment")
    BTree<int> tree1({1, 2, 3}, 2);
    BTree<int> tree2(2);
    
    tree2 = tree1;
    
    assert(tree2.size() == 3);
    assert(tree2.search(2));
    END_TEST
}

void test_move_assignment() {
    TEST("Move assignment")
    BTree<int> tree1({1, 2, 3}, 2);
    BTree<int> tree2(2);
    
    tree2 = std::move(tree1);
    
    assert(tree2.size() == 3);
    assert(tree1.size() == 0);
    END_TEST
}

// ============================================
// Insert Tests
// ============================================

void test_insert_single() {
    TEST("Insert single element")
    BTree<int> tree(3);
    
    assert(tree.insert(10));
    assert(tree.size() == 1);
    assert(tree.search(10));
    END_TEST
}

void test_insert_multiple() {
    TEST("Insert multiple elements")
    BTree<int> tree(3);
    
    for (int i = 1; i <= 10; ++i) {
        assert(tree.insert(i));
    }
    
    assert(tree.size() == 10);
    for (int i = 1; i <= 10; ++i) {
        assert(tree.search(i));
    }
    END_TEST
}

void test_insert_duplicate() {
    TEST("Insert duplicate (should fail)")
    BTree<int> tree(3);
    
    assert(tree.insert(10));
    assert(!tree.insert(10));  // Duplicate
    assert(tree.size() == 1);
    END_TEST
}

void test_insert_ascending() {
    TEST("Insert in ascending order")
    BTree<int> tree(3);
    
    for (int i = 1; i <= 20; ++i) {
        tree.insert(i);
    }
    
    assert(tree.size() == 20);
    assert(tree.verify());
    END_TEST
}

void test_insert_descending() {
    TEST("Insert in descending order")
    BTree<int> tree(3);
    
    for (int i = 20; i >= 1; --i) {
        tree.insert(i);
    }
    
    assert(tree.size() == 20);
    assert(tree.verify());
    END_TEST
}

void test_insert_random() {
    TEST("Insert in random order")
    BTree<int> tree(4);
    
    std::vector<int> values;
    for (int i = 1; i <= 50; ++i) {
        values.push_back(i);
    }
    
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(values.begin(), values.end(), g);
    
    for (int val : values) {
        tree.insert(val);
    }
    
    assert(tree.size() == 50);
    assert(tree.verify());
    
    for (int val : values) {
        assert(tree.search(val));
    }
    END_TEST
}

void test_insert_with_split() {
    TEST("Insert causing node splits")
    BTree<int> tree(2);  // Small degree to force splits
    
    for (int i = 1; i <= 10; ++i) {
        tree.insert(i);
        assert(tree.verify());
    }
    
    assert(tree.size() == 10);
    END_TEST
}

// ============================================
// Search Tests
// ============================================

void test_search_existing() {
    TEST("Search for existing keys")
    BTree<int> tree({10, 20, 30, 40, 50}, 3);
    
    assert(tree.search(10));
    assert(tree.search(30));
    assert(tree.search(50));
    assert(tree.contains(20));
    END_TEST
}

void test_search_non_existing() {
    TEST("Search for non-existing keys")
    BTree<int> tree({10, 20, 30}, 3);
    
    assert(!tree.search(5));
    assert(!tree.search(15));
    assert(!tree.search(100));
    END_TEST
}

void test_search_empty_tree() {
    TEST("Search in empty tree")
    BTree<int> tree(3);
    
    assert(!tree.search(10));
    END_TEST
}

// ============================================
// Remove Tests
// ============================================

void test_remove_from_leaf() {
    TEST("Remove from leaf node")
    BTree<int> tree({1, 2, 3, 4, 5, 6, 7}, 3);
    
    assert(tree.remove(7));
    assert(!tree.search(7));
    assert(tree.size() == 6);
    assert(tree.verify());
    END_TEST
}

void test_remove_from_internal() {
    TEST("Remove from internal node")
    BTree<int> tree(3);
    
    for (int i = 1; i <= 15; ++i) {
        tree.insert(i);
    }
    
    // Remove a key that's likely in internal node
    assert(tree.remove(8));
    assert(!tree.search(8));
    assert(tree.verify());
    END_TEST
}

void test_remove_causing_merge() {
    TEST("Remove causing node merge")
    BTree<int> tree(2);  // Small degree
    
    for (int i = 1; i <= 10; ++i) {
        tree.insert(i);
    }
    
    // Remove several elements to cause merges
    for (int i = 1; i <= 5; ++i) {
        tree.remove(i);
        assert(tree.verify());
    }
    
    assert(tree.size() == 5);
    END_TEST
}

void test_remove_all_elements() {
    TEST("Remove all elements")
    BTree<int> tree({1, 2, 3, 4, 5}, 2);
    
    for (int i = 1; i <= 5; ++i) {
        assert(tree.remove(i));
    }
    
    assert(tree.empty());
    assert(tree.size() == 0);
    END_TEST
}

void test_remove_non_existing() {
    TEST("Remove non-existing key")
    BTree<int> tree({1, 2, 3}, 2);
    
    assert(!tree.remove(10));
    assert(tree.size() == 3);
    END_TEST
}

void test_remove_from_empty() {
    TEST("Remove from empty tree")
    BTree<int> tree(3);
    
    assert(!tree.remove(10));
    END_TEST
}

void test_remove_random_order() {
    TEST("Remove in random order")
    BTree<int> tree(3);
    
    std::vector<int> values;
    for (int i = 1; i <= 30; ++i) {
        values.push_back(i);
        tree.insert(i);
    }
    
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(values.begin(), values.end(), g);
    
    for (int val : values) {
        assert(tree.remove(val));
        assert(!tree.search(val));
        assert(tree.verify());
    }
    
    assert(tree.empty());
    END_TEST
}

// ============================================
// Min/Max Tests
// ============================================

void test_min_max() {
    TEST("Min and max")
    BTree<int> tree({5, 2, 8, 1, 9, 3}, 3);
    
    auto min_val = tree.min();
    auto max_val = tree.max();
    
    assert(min_val.has_value());
    assert(max_val.has_value());
    assert(min_val.value() == 1);
    assert(max_val.value() == 9);
    END_TEST
}

void test_min_max_empty() {
    TEST("Min and max on empty tree")
    BTree<int> tree(3);
    
    assert(!tree.min().has_value());
    assert(!tree.max().has_value());
    END_TEST
}

void test_min_max_single() {
    TEST("Min and max with single element")
    BTree<int> tree(3);
    tree.insert(42);
    
    assert(tree.min().value() == 42);
    assert(tree.max().value() == 42);
    END_TEST
}

// ============================================
// Traversal Tests
// ============================================

void test_inorder_traversal() {
    TEST("Inorder traversal (sorted order)")
    BTree<int> tree({5, 2, 8, 1, 9, 3, 7, 4, 6}, 3);
    
    std::vector<int> result;
    tree.inorder([&result](int val) {
        result.push_back(val);
    });
    
    // Should be sorted
    std::vector<int> expected = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    assert(result == expected);
    END_TEST
}

void test_level_order_traversal() {
    TEST("Level order traversal")
    BTree<int> tree({1, 2, 3, 4, 5}, 2);
    
    std::vector<int> result;
    tree.level_order([&result](int val) {
        result.push_back(val);
    });
    
    // All elements should be present
    assert(result.size() == 5);
    
    // Check all values exist
    std::set<int> values(result.begin(), result.end());
    for (int i = 1; i <= 5; ++i) {
        assert(values.count(i) == 1);
    }
    END_TEST
}

void test_to_vector() {
    TEST("Convert to vector")
    BTree<int> tree({5, 2, 8, 1, 9}, 3);
    
    auto vec = tree.to_vector();
    
    // Should be sorted
    assert(vec == std::vector<int>({1, 2, 5, 8, 9}));
    assert(std::is_sorted(vec.begin(), vec.end()));
    END_TEST
}

// ============================================
// Property Tests
// ============================================

void test_height() {
    TEST("Tree height")
    BTree<int> tree(3);
    
    assert(tree.height() == 0);
    
    tree.insert(1);
    assert(tree.height() == 1);
    
    for (int i = 2; i <= 10; ++i) {
        tree.insert(i);
    }
    
    // Height should grow logarithmically
    assert(tree.height() <= 3);
    END_TEST
}

void test_verify_properties() {
    TEST("Verify B-Tree properties")
    BTree<int> tree(3);
    
    // Insert and verify after each insertion
    for (int i = 1; i <= 50; ++i) {
        tree.insert(i);
        assert(tree.verify());
    }
    
    // Remove and verify after each removal
    for (int i = 1; i <= 25; ++i) {
        tree.remove(i);
        assert(tree.verify());
    }
    END_TEST
}

void test_different_degrees() {
    TEST("Different minimum degrees")
    
    for (std::size_t t = 2; t <= 10; ++t) {
        BTree<int> tree(t);
        
        for (int i = 1; i <= 50; ++i) {
            tree.insert(i);
        }
        
        assert(tree.size() == 50);
        assert(tree.verify());
    }
    END_TEST
}

// ============================================
// Utility Tests
// ============================================

void test_clear() {
    TEST("Clear tree")
    BTree<int> tree({1, 2, 3, 4, 5}, 3);
    
    tree.clear();
    
    assert(tree.empty());
    assert(tree.size() == 0);
    assert(!tree.search(1));
    END_TEST
}

void test_swap() {
    TEST("Swap trees")
    BTree<int> tree1({1, 2, 3}, 2);
    BTree<int> tree2({10, 20, 30}, 3);
    
    std::size_t size1 = tree1.size();
    std::size_t size2 = tree2.size();
    
    tree1.swap(tree2);
    
    assert(tree1.size() == size2);
    assert(tree2.size() == size1);
    assert(tree1.search(10));
    assert(tree2.search(1));
    END_TEST
}

// ============================================
// Edge Cases
// ============================================

void test_single_element() {
    TEST("Single element operations")
    BTree<int> tree(3);
    
    tree.insert(42);
    assert(tree.size() == 1);
    assert(tree.search(42));
    
    tree.remove(42);
    assert(tree.empty());
    END_TEST
}

void test_two_elements() {
    TEST("Two elements operations")
    BTree<int> tree(2);
    
    tree.insert(1);
    tree.insert(2);
    
    assert(tree.size() == 2);
    assert(tree.min().value() == 1);
    assert(tree.max().value() == 2);
    END_TEST
}

void test_large_dataset() {
    TEST("Large dataset (1000 elements)")
    BTree<int> tree(5);
    
    for (int i = 1; i <= 1000; ++i) {
        tree.insert(i);
    }
    
    assert(tree.size() == 1000);
    assert(tree.verify());
    
    // Verify some random elements
    assert(tree.search(1));
    assert(tree.search(500));
    assert(tree.search(1000));
    END_TEST
}

void test_stress_insert_remove() {
    TEST("Stress test: insert and remove")
    BTree<int> tree(3);
    
    // Insert 100 elements
    for (int i = 1; i <= 100; ++i) {
        tree.insert(i);
    }
    
    // Remove every other element
    for (int i = 2; i <= 100; i += 2) {
        tree.remove(i);
    }
    
    assert(tree.size() == 50);
    assert(tree.verify());
    
    // Verify odd elements remain
    for (int i = 1; i <= 100; i += 2) {
        assert(tree.search(i));
    }
    
    // Verify even elements removed
    for (int i = 2; i <= 100; i += 2) {
        assert(!tree.search(i));
    }
    END_TEST
}

// ============================================
// Custom Comparator Tests
// ============================================

void test_custom_comparator_descending() {
    TEST("Custom comparator (descending)")
    BTree<int, std::greater<int>> tree(3);
    
    tree.insert(5);
    tree.insert(2);
    tree.insert(8);
    tree.insert(1);
    
    auto vec = tree.to_vector();
    
    // Should be in descending order
    assert(vec[0] > vec[1]);
    assert(vec[1] > vec[2]);
    END_TEST
}

// ============================================
// String Tests
// ============================================

void test_string_tree() {
    TEST("B-Tree with strings")
    BTree<std::string> tree(3);
    
    tree.insert("apple");
    tree.insert("banana");
    tree.insert("cherry");
    tree.insert("date");
    
    assert(tree.size() == 4);
    assert(tree.search("banana"));
    assert(tree.verify());
    
    auto vec = tree.to_vector();
    assert(std::is_sorted(vec.begin(), vec.end()));
    END_TEST
}

// ============================================
// Main Test Runner
// ============================================

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "B-Tree Test Suite" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;
    
    // Constructor tests
    std::cout << "--- Constructor Tests ---" << std::endl;
    test_default_constructor();
    test_initializer_list_constructor();
    test_range_constructor();
    test_copy_constructor();
    test_move_constructor();
    
    // Assignment tests
    std::cout << std::endl << "--- Assignment Tests ---" << std::endl;
    test_copy_assignment();
    test_move_assignment();
    
    // Insert tests
    std::cout << std::endl << "--- Insert Tests ---" << std::endl;
    test_insert_single();
    test_insert_multiple();
    test_insert_duplicate();
    test_insert_ascending();
    test_insert_descending();
    test_insert_random();
    test_insert_with_split();
    
    // Search tests
    std::cout << std::endl << "--- Search Tests ---" << std::endl;
    test_search_existing();
    test_search_non_existing();
    test_search_empty_tree();
    
    // Remove tests
    std::cout << std::endl << "--- Remove Tests ---" << std::endl;
    test_remove_from_leaf();
    test_remove_from_internal();
    test_remove_causing_merge();
    test_remove_all_elements();
    test_remove_non_existing();
    test_remove_from_empty();
    test_remove_random_order();
    
    // Min/Max tests
    std::cout << std::endl << "--- Min/Max Tests ---" << std::endl;
    test_min_max();
    test_min_max_empty();
    test_min_max_single();
    
    // Traversal tests
    std::cout << std::endl << "--- Traversal Tests ---" << std::endl;
    test_inorder_traversal();
    test_level_order_traversal();
    test_to_vector();
    
    // Property tests
    std::cout << std::endl << "--- Property Tests ---" << std::endl;
    test_height();
    test_verify_properties();
    test_different_degrees();
    
    // Utility tests
    std::cout << std::endl << "--- Utility Tests ---" << std::endl;
    test_clear();
    test_swap();
    
    // Edge cases
    std::cout << std::endl << "--- Edge Cases ---" << std::endl;
    test_single_element();
    test_two_elements();
    test_large_dataset();
    test_stress_insert_remove();
    
    // Custom types
    std::cout << std::endl << "--- Custom Types ---" << std::endl;
    test_custom_comparator_descending();
    test_string_tree();
    
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