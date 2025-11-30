/**
 * @file test_red_black_tree.cpp
 * @brief Test suite for Red-Black Tree data structure
 * @author Jinhyeok
 * @date 2025-11-30
 * @version 1.0.0
 */

#include "tree/red_black_tree.hpp"
#include <iostream>
#include <cassert>
#include <string>
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
    RedBlackTree<int> tree;
    
    assert(tree.empty());
    assert(tree.size() == 0);
    assert(tree.verify());
    END_TEST
}

void test_initializer_list_constructor() {
    TEST("Initializer list constructor")
    RedBlackTree<int> tree = {5, 3, 7, 1, 9};
    
    assert(tree.size() == 5);
    assert(tree.contains(5));
    assert(tree.contains(3));
    assert(tree.contains(7));
    assert(tree.verify());
    END_TEST
}

void test_iterator_constructor() {
    TEST("Iterator constructor")
    std::vector<int> values = {10, 20, 30, 40, 50};
    RedBlackTree<int> tree(values.begin(), values.end());
    
    assert(tree.size() == 5);
    for (int v : values) {
        assert(tree.contains(v));
    }
    assert(tree.verify());
    END_TEST
}

void test_copy_constructor() {
    TEST("Copy constructor")
    RedBlackTree<int> original = {1, 2, 3, 4, 5};
    RedBlackTree<int> copy(original);
    
    assert(copy.size() == 5);
    assert(copy.verify());
    
    // Modify original
    original.insert(100);
    assert(original.size() == 6);
    assert(copy.size() == 5);
    assert(!copy.contains(100));
    END_TEST
}

void test_move_constructor() {
    TEST("Move constructor")
    RedBlackTree<int> original = {1, 2, 3};
    RedBlackTree<int> moved(std::move(original));
    
    assert(moved.size() == 3);
    assert(moved.verify());
    assert(original.empty());
    END_TEST
}

void test_copy_assignment() {
    TEST("Copy assignment")
    RedBlackTree<int> tree1 = {1, 2, 3};
    RedBlackTree<int> tree2 = {100, 200};
    
    tree2 = tree1;
    
    assert(tree2.size() == 3);
    assert(tree2.contains(1));
    assert(!tree2.contains(100));
    assert(tree2.verify());
    END_TEST
}

void test_move_assignment() {
    TEST("Move assignment")
    RedBlackTree<int> tree1 = {1, 2, 3};
    RedBlackTree<int> tree2 = {100, 200};
    
    tree2 = std::move(tree1);
    
    assert(tree2.size() == 3);
    assert(tree2.contains(1));
    assert(tree2.verify());
    END_TEST
}

// ============================================
// Insert Tests
// ============================================

void test_insert_single() {
    TEST("Insert single element")
    RedBlackTree<int> tree;
    
    auto [it, inserted] = tree.insert(10);
    
    assert(inserted);
    assert(*it == 10);
    assert(tree.size() == 1);
    assert(tree.contains(10));
    assert(tree.verify());
    assert(tree.is_root_black());
    END_TEST
}

void test_insert_multiple() {
    TEST("Insert multiple elements")
    RedBlackTree<int> tree;
    
    tree.insert(10);
    tree.insert(5);
    tree.insert(15);
    tree.insert(3);
    tree.insert(7);
    
    assert(tree.size() == 5);
    assert(tree.verify());
    END_TEST
}

void test_insert_duplicate() {
    TEST("Insert duplicate")
    RedBlackTree<int> tree;
    
    auto [it1, inserted1] = tree.insert(10);
    auto [it2, inserted2] = tree.insert(10);
    
    assert(inserted1);
    assert(!inserted2);
    assert(tree.size() == 1);
    assert(tree.verify());
    END_TEST
}

void test_insert_ascending() {
    TEST("Insert ascending order")
    RedBlackTree<int> tree;
    
    for (int i = 1; i <= 10; ++i) {
        tree.insert(i);
        assert(tree.verify());
    }
    
    assert(tree.size() == 10);
    assert(tree.height() <= 2 * std::log2(11));  // RB tree height bound
    END_TEST
}

void test_insert_descending() {
    TEST("Insert descending order")
    RedBlackTree<int> tree;
    
    for (int i = 10; i >= 1; --i) {
        tree.insert(i);
        assert(tree.verify());
    }
    
    assert(tree.size() == 10);
    END_TEST
}

void test_insert_random() {
    TEST("Insert random order")
    RedBlackTree<int> tree;
    std::vector<int> values = {5, 2, 8, 1, 4, 7, 9, 3, 6, 10};
    
    for (int v : values) {
        tree.insert(v);
        assert(tree.verify());
    }
    
    assert(tree.size() == 10);
    END_TEST
}

void test_emplace() {
    TEST("Emplace")
    RedBlackTree<std::string> tree;
    
    auto [it, inserted] = tree.emplace("hello");
    
    assert(inserted);
    assert(*it == "hello");
    assert(tree.verify());
    END_TEST
}

// ============================================
// Erase Tests
// ============================================

void test_erase_leaf() {
    TEST("Erase leaf node")
    RedBlackTree<int> tree = {5, 3, 7, 1, 9};
    
    bool erased = tree.erase(1);
    
    assert(erased);
    assert(tree.size() == 4);
    assert(!tree.contains(1));
    assert(tree.verify());
    END_TEST
}

void test_erase_node_with_one_child() {
    TEST("Erase node with one child")
    RedBlackTree<int> tree = {5, 3, 7, 1};
    
    bool erased = tree.erase(3);
    
    assert(erased);
    assert(tree.size() == 3);
    assert(!tree.contains(3));
    assert(tree.contains(1));
    assert(tree.verify());
    END_TEST
}

void test_erase_node_with_two_children() {
    TEST("Erase node with two children")
    RedBlackTree<int> tree = {5, 3, 7, 1, 4, 6, 9};
    
    bool erased = tree.erase(5);
    
    assert(erased);
    assert(tree.size() == 6);
    assert(!tree.contains(5));
    assert(tree.verify());
    END_TEST
}

void test_erase_root() {
    TEST("Erase root")
    RedBlackTree<int> tree = {5, 3, 7};
    
    bool erased = tree.erase(tree.root());
    
    assert(erased);
    assert(tree.size() == 2);
    assert(tree.verify());
    assert(tree.is_root_black());
    END_TEST
}

void test_erase_nonexistent() {
    TEST("Erase nonexistent")
    RedBlackTree<int> tree = {1, 2, 3};
    
    bool erased = tree.erase(100);
    
    assert(!erased);
    assert(tree.size() == 3);
    assert(tree.verify());
    END_TEST
}

void test_erase_all() {
    TEST("Erase all elements")
    RedBlackTree<int> tree = {5, 3, 7, 1, 9};
    
    tree.erase(5);
    assert(tree.verify());
    tree.erase(3);
    assert(tree.verify());
    tree.erase(7);
    assert(tree.verify());
    tree.erase(1);
    assert(tree.verify());
    tree.erase(9);
    assert(tree.verify());
    
    assert(tree.empty());
    END_TEST
}

void test_erase_iterator() {
    TEST("Erase by iterator")
    RedBlackTree<int> tree = {1, 2, 3, 4, 5};
    
    auto it = tree.find(3);
    auto next = tree.erase(it);
    
    assert(tree.size() == 4);
    assert(!tree.contains(3));
    assert(*next == 4);
    assert(tree.verify());
    END_TEST
}

// ============================================
// Lookup Tests
// ============================================

void test_find() {
    TEST("Find")
    RedBlackTree<int> tree = {5, 3, 7, 1, 9};
    
    auto it = tree.find(3);
    assert(it != tree.end());
    assert(*it == 3);
    
    auto not_found = tree.find(100);
    assert(not_found == tree.end());
    END_TEST
}

void test_contains() {
    TEST("Contains")
    RedBlackTree<int> tree = {10, 20, 30};
    
    assert(tree.contains(10));
    assert(tree.contains(20));
    assert(tree.contains(30));
    assert(!tree.contains(15));
    assert(!tree.contains(0));
    END_TEST
}

void test_count() {
    TEST("Count")
    RedBlackTree<int> tree = {1, 2, 3};
    
    assert(tree.count(1) == 1);
    assert(tree.count(2) == 1);
    assert(tree.count(100) == 0);
    END_TEST
}

void test_min_max() {
    TEST("Min and max")
    RedBlackTree<int> tree = {5, 2, 8, 1, 9, 3};
    
    assert(tree.min() == 1);
    assert(tree.max() == 9);
    END_TEST
}

void test_lower_bound() {
    TEST("Lower bound")
    RedBlackTree<int> tree = {10, 20, 30, 40, 50};
    
    auto it1 = tree.lower_bound(25);
    assert(*it1 == 30);
    
    auto it2 = tree.lower_bound(30);
    assert(*it2 == 30);
    
    auto it3 = tree.lower_bound(5);
    assert(*it3 == 10);
    
    auto it4 = tree.lower_bound(100);
    assert(it4 == tree.end());
    END_TEST
}

void test_upper_bound() {
    TEST("Upper bound")
    RedBlackTree<int> tree = {10, 20, 30, 40, 50};
    
    auto it1 = tree.upper_bound(25);
    assert(*it1 == 30);
    
    auto it2 = tree.upper_bound(30);
    assert(*it2 == 40);
    
    auto it3 = tree.upper_bound(50);
    assert(it3 == tree.end());
    END_TEST
}

// ============================================
// Iterator Tests
// ============================================

void test_iterator_basic() {
    TEST("Iterator basic")
    RedBlackTree<int> tree = {3, 1, 4, 1, 5, 9, 2, 6};
    
    std::vector<int> result;
    for (const auto& val : tree) {
        result.push_back(val);
    }
    
    // Should be sorted
    assert(std::is_sorted(result.begin(), result.end()));
    END_TEST
}

void test_iterator_increment() {
    TEST("Iterator increment")
    RedBlackTree<int> tree = {1, 2, 3, 4, 5};
    
    auto it = tree.begin();
    assert(*it == 1);
    ++it;
    assert(*it == 2);
    it++;
    assert(*it == 3);
    END_TEST
}

void test_iterator_decrement() {
    TEST("Iterator decrement")
    RedBlackTree<int> tree = {1, 2, 3, 4, 5};
    
    auto it = tree.end();
    --it;
    assert(*it == 5);
    it--;
    assert(*it == 4);
    END_TEST
}

void test_iterator_equality() {
    TEST("Iterator equality")
    RedBlackTree<int> tree = {1, 2, 3};
    
    auto it1 = tree.begin();
    auto it2 = tree.begin();
    auto it3 = tree.end();
    
    assert(it1 == it2);
    assert(it1 != it3);
    END_TEST
}

void test_const_iterator() {
    TEST("Const iterator")
    const RedBlackTree<int> tree = {1, 2, 3};
    
    std::vector<int> result;
    for (auto it = tree.cbegin(); it != tree.cend(); ++it) {
        result.push_back(*it);
    }
    
    assert(result.size() == 3);
    END_TEST
}

// ============================================
// Traversal Tests
// ============================================

void test_inorder() {
    TEST("Inorder traversal")
    RedBlackTree<int> tree = {5, 3, 7, 1, 9};
    
    std::vector<int> result;
    tree.inorder([&result](int val) {
        result.push_back(val);
    });
    
    assert(result == std::vector<int>({1, 3, 5, 7, 9}));
    END_TEST
}

void test_preorder() {
    TEST("Preorder traversal")
    RedBlackTree<int> tree = {2, 1, 3};
    
    std::vector<int> result;
    tree.preorder([&result](int val) {
        result.push_back(val);
    });
    
    assert(result.size() == 3);
    // Root should be first
    assert(result[0] == tree.root());
    END_TEST
}

void test_postorder() {
    TEST("Postorder traversal")
    RedBlackTree<int> tree = {2, 1, 3};
    
    std::vector<int> result;
    tree.postorder([&result](int val) {
        result.push_back(val);
    });
    
    assert(result.size() == 3);
    // Root should be last
    assert(result[2] == tree.root());
    END_TEST
}

void test_level_order() {
    TEST("Level order traversal")
    RedBlackTree<int> tree = {5, 3, 7, 1, 9};
    
    std::vector<int> result;
    tree.level_order([&result](int val) {
        result.push_back(val);
    });
    
    assert(result.size() == 5);
    // Root should be first
    assert(result[0] == tree.root());
    END_TEST
}

void test_to_vector() {
    TEST("To vector")
    RedBlackTree<int> tree = {5, 2, 8, 1, 3};
    
    auto vec = tree.to_vector();
    
    assert(vec == std::vector<int>({1, 2, 3, 5, 8}));
    END_TEST
}

// ============================================
// Property Tests
// ============================================

void test_height() {
    TEST("Height")
    RedBlackTree<int> tree;
    
    assert(tree.height() == 0);
    
    tree.insert(1);
    assert(tree.height() == 1);
    
    tree.insert(2);
    tree.insert(3);
    // Height should be logarithmic
    assert(tree.height() <= 3);
    END_TEST
}

void test_black_height() {
    TEST("Black height")
    RedBlackTree<int> tree = {5, 3, 7, 1, 9};
    
    std::size_t bh = tree.black_height();
    assert(bh >= 1);
    END_TEST
}

void test_verify_properties() {
    TEST("Verify RB properties")
    RedBlackTree<int> tree;
    
    // Insert many elements and verify after each
    for (int i = 1; i <= 100; ++i) {
        tree.insert(i);
        assert(tree.verify());
        assert(tree.is_root_black());
    }
    END_TEST
}

// ============================================
// Utility Tests
// ============================================

void test_clear() {
    TEST("Clear")
    RedBlackTree<int> tree = {1, 2, 3, 4, 5};
    
    tree.clear();
    
    assert(tree.empty());
    assert(tree.size() == 0);
    assert(tree.verify());
    END_TEST
}

void test_swap() {
    TEST("Swap")
    RedBlackTree<int> tree1 = {1, 2, 3};
    RedBlackTree<int> tree2 = {10, 20};
    
    tree1.swap(tree2);
    
    assert(tree1.size() == 2);
    assert(tree1.contains(10));
    assert(tree2.size() == 3);
    assert(tree2.contains(1));
    END_TEST
}

// ============================================
// Edge Cases
// ============================================

void test_single_element() {
    TEST("Single element")
    RedBlackTree<int> tree;
    tree.insert(42);
    
    assert(tree.size() == 1);
    assert(tree.min() == 42);
    assert(tree.max() == 42);
    assert(tree.verify());
    assert(tree.is_root_black());
    
    tree.erase(42);
    assert(tree.empty());
    END_TEST
}

void test_two_elements() {
    TEST("Two elements")
    RedBlackTree<int> tree = {1, 2};
    
    assert(tree.size() == 2);
    assert(tree.verify());
    
    tree.erase(1);
    assert(tree.size() == 1);
    assert(tree.verify());
    END_TEST
}

void test_custom_comparator() {
    TEST("Custom comparator (descending)")
    RedBlackTree<int, std::greater<int>> tree;
    
    tree.insert(1);
    tree.insert(2);
    tree.insert(3);
    
    // With greater<>, min should return largest
    assert(tree.min() == 3);
    assert(tree.max() == 1);
    
    std::vector<int> result;
    for (const auto& val : tree) {
        result.push_back(val);
    }
    assert(result == std::vector<int>({3, 2, 1}));
    END_TEST
}

void test_string_tree() {
    TEST("String tree")
    RedBlackTree<std::string> tree = {"banana", "apple", "cherry"};
    
    assert(tree.size() == 3);
    assert(tree.min() == "apple");
    assert(tree.max() == "cherry");
    assert(tree.verify());
    END_TEST
}

// ============================================
// Large Scale Tests
// ============================================

void test_large_insertion() {
    TEST("Large insertion (1000 elements)")
    RedBlackTree<int> tree;
    
    for (int i = 0; i < 1000; ++i) {
        tree.insert(i);
    }
    
    assert(tree.size() == 1000);
    assert(tree.verify());
    
    // Height should be O(log n)
    assert(tree.height() <= 2 * std::log2(1001) + 1);
    END_TEST
}

void test_large_deletion() {
    TEST("Large deletion")
    RedBlackTree<int> tree;
    
    for (int i = 0; i < 500; ++i) {
        tree.insert(i);
    }
    
    for (int i = 0; i < 250; ++i) {
        tree.erase(i);
        assert(tree.verify());
    }
    
    assert(tree.size() == 250);
    END_TEST
}

void test_random_operations() {
    TEST("Random operations")
    RedBlackTree<int> tree;
    std::set<int> reference;
    
    std::mt19937 rng(42);
    std::uniform_int_distribution<int> dist(0, 1000);
    
    // Random insertions
    for (int i = 0; i < 500; ++i) {
        int val = dist(rng);
        tree.insert(val);
        reference.insert(val);
    }
    
    assert(tree.size() == reference.size());
    assert(tree.verify());
    
    // Verify all elements
    for (int val : reference) {
        assert(tree.contains(val));
    }
    END_TEST
}

void test_stress_insert_delete() {
    TEST("Stress insert/delete")
    RedBlackTree<int> tree;
    
    // Insert 1-100
    for (int i = 1; i <= 100; ++i) {
        tree.insert(i);
    }
    
    // Delete even numbers
    for (int i = 2; i <= 100; i += 2) {
        tree.erase(i);
        assert(tree.verify());
    }
    
    assert(tree.size() == 50);
    
    // Insert even numbers back
    for (int i = 2; i <= 100; i += 2) {
        tree.insert(i);
        assert(tree.verify());
    }
    
    assert(tree.size() == 100);
    END_TEST
}

// ============================================
// Comparison with std::set
// ============================================

void test_compare_with_std_set() {
    TEST("Compare with std::set")
    RedBlackTree<int> tree;
    std::set<int> stdset;
    
    std::vector<int> values = {50, 25, 75, 10, 30, 60, 80, 5, 15, 27, 35};
    
    for (int v : values) {
        tree.insert(v);
        stdset.insert(v);
    }
    
    // Compare sizes
    assert(tree.size() == stdset.size());
    
    // Compare iteration order
    auto tree_it = tree.begin();
    auto set_it = stdset.begin();
    
    while (tree_it != tree.end() && set_it != stdset.end()) {
        assert(*tree_it == *set_it);
        ++tree_it;
        ++set_it;
    }
    END_TEST
}

// Main test runner
int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "Red-Black Tree Test Suite" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;

    // Constructor tests
    std::cout << "--- Constructor Tests ---" << std::endl;
    test_default_constructor();
    test_initializer_list_constructor();
    test_iterator_constructor();
    test_copy_constructor();
    test_move_constructor();
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
    test_emplace();

    // Erase tests
    std::cout << std::endl << "--- Erase Tests ---" << std::endl;
    test_erase_leaf();
    test_erase_node_with_one_child();
    test_erase_node_with_two_children();
    test_erase_root();
    test_erase_nonexistent();
    test_erase_all();
    test_erase_iterator();

    // Lookup tests
    std::cout << std::endl << "--- Lookup Tests ---" << std::endl;
    test_find();
    test_contains();
    test_count();
    test_min_max();
    test_lower_bound();
    test_upper_bound();

    // Iterator tests
    std::cout << std::endl << "--- Iterator Tests ---" << std::endl;
    test_iterator_basic();
    test_iterator_increment();
    test_iterator_decrement();
    test_iterator_equality();
    test_const_iterator();

    // Traversal tests
    std::cout << std::endl << "--- Traversal Tests ---" << std::endl;
    test_inorder();
    test_preorder();
    test_postorder();
    test_level_order();
    test_to_vector();

    // Property tests
    std::cout << std::endl << "--- Property Tests ---" << std::endl;
    test_height();
    test_black_height();
    test_verify_properties();

    // Utility tests
    std::cout << std::endl << "--- Utility Tests ---" << std::endl;
    test_clear();
    test_swap();

    // Edge cases
    std::cout << std::endl << "--- Edge Cases ---" << std::endl;
    test_single_element();
    test_two_elements();
    test_custom_comparator();
    test_string_tree();

    // Large scale tests
    std::cout << std::endl << "--- Large Scale Tests ---" << std::endl;
    test_large_insertion();
    test_large_deletion();
    test_random_operations();
    test_stress_insert_delete();

    // Comparison tests
    std::cout << std::endl << "--- Comparison Tests ---" << std::endl;
    test_compare_with_std_set();

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