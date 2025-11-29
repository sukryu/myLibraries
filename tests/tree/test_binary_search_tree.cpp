/**
 * @file test_binary_search_tree.cpp
 * @brief Test suite for BinarySearchTree class
 * @author Jinhyeok
 * @date 2025-11-29
 * @version 1.0.0
 */

#include "tree/binary_search_tree.hpp"
#include <iostream>
#include <cassert>
#include <vector>
#include <string>

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

// Test functions
void test_default_constructor() {
    TEST("Default constructor")
    BinarySearchTree<int> bst;
    assert(bst.empty());
    assert(bst.size() == 0);
    assert(bst.height() == 0);
    END_TEST
}

void test_initializer_list() {
    TEST("Initializer list constructor")
    BinarySearchTree<int> bst = {5, 3, 7, 1, 9};
    assert(bst.size() == 5);
    assert(!bst.empty());
    assert(bst.contains(5));
    assert(bst.contains(3));
    assert(bst.contains(7));
    assert(bst.contains(1));
    assert(bst.contains(9));
    END_TEST
}

void test_copy_constructor() {
    TEST("Copy constructor")
    BinarySearchTree<int> bst1 = {5, 3, 7, 1, 9};
    BinarySearchTree<int> bst2(bst1);
    
    assert(bst2.size() == 5);
    assert(bst2.contains(5));
    assert(bst2.contains(1));
    assert(bst2.contains(9));
    
    // Verify deep copy
    bst2.insert(100);
    assert(bst2.contains(100));
    assert(!bst1.contains(100));
    END_TEST
}

void test_move_constructor() {
    TEST("Move constructor")
    BinarySearchTree<int> bst1 = {5, 3, 7};
    BinarySearchTree<int> bst2(std::move(bst1));
    
    assert(bst2.size() == 3);
    assert(bst2.contains(5));
    assert(bst1.empty());
    END_TEST
}

void test_copy_assignment() {
    TEST("Copy assignment operator")
    BinarySearchTree<int> bst1 = {5, 3, 7};
    BinarySearchTree<int> bst2;
    bst2 = bst1;
    
    assert(bst2.size() == 3);
    assert(bst2.contains(5));
    
    bst2.insert(100);
    assert(!bst1.contains(100));
    END_TEST
}

void test_move_assignment() {
    TEST("Move assignment operator")
    BinarySearchTree<int> bst1 = {5, 3, 7};
    BinarySearchTree<int> bst2;
    bst2 = std::move(bst1);
    
    assert(bst2.size() == 3);
    assert(bst1.empty());
    END_TEST
}

void test_insert_basic() {
    TEST("Basic insert")
    BinarySearchTree<int> bst;
    
    assert(bst.insert(10));
    assert(bst.size() == 1);
    assert(bst.contains(10));
    
    assert(bst.insert(5));
    assert(bst.insert(15));
    assert(bst.size() == 3);
    END_TEST
}

void test_insert_duplicate() {
    TEST("Insert duplicate (should fail)")
    BinarySearchTree<int> bst;
    
    assert(bst.insert(10));
    assert(!bst.insert(10));  // Duplicate
    assert(bst.size() == 1);
    
    assert(bst.insert(5));
    assert(!bst.insert(5));   // Duplicate
    assert(bst.size() == 2);
    END_TEST
}

void test_insert_ordered() {
    TEST("Insert in ascending order")
    BinarySearchTree<int> bst;
    
    for (int i = 1; i <= 10; ++i) {
        assert(bst.insert(i));
    }
    assert(bst.size() == 10);
    // Tree will be skewed, height should be 10
    assert(bst.height() == 10);
    END_TEST
}

void test_insert_balanced() {
    TEST("Insert in balanced order")
    BinarySearchTree<int> bst;
    
    // Insert in order that creates balanced tree
    bst.insert(50);
    bst.insert(25);
    bst.insert(75);
    bst.insert(10);
    bst.insert(30);
    bst.insert(60);
    bst.insert(90);
    
    assert(bst.size() == 7);
    assert(bst.height() == 3);
    END_TEST
}

void test_remove_leaf() {
    TEST("Remove leaf node")
    BinarySearchTree<int> bst = {5, 3, 7, 1, 9};
    
    assert(bst.remove(1));
    assert(!bst.contains(1));
    assert(bst.size() == 4);
    
    assert(bst.remove(9));
    assert(!bst.contains(9));
    assert(bst.size() == 3);
    END_TEST
}

void test_remove_one_child() {
    TEST("Remove node with one child")
    BinarySearchTree<int> bst;
    bst.insert(10);
    bst.insert(5);
    bst.insert(3);  // 5 has only left child
    
    assert(bst.remove(5));
    assert(!bst.contains(5));
    assert(bst.contains(3));
    assert(bst.contains(10));
    assert(bst.size() == 2);
    END_TEST
}

void test_remove_two_children() {
    TEST("Remove node with two children")
    BinarySearchTree<int> bst = {10, 5, 15, 3, 7, 12, 20};
    
    // Remove node with two children
    assert(bst.remove(5));
    assert(!bst.contains(5));
    assert(bst.contains(3));
    assert(bst.contains(7));
    assert(bst.size() == 6);
    
    // Remove root with two children
    assert(bst.remove(10));
    assert(!bst.contains(10));
    assert(bst.size() == 5);
    assert(bst.is_valid());
    END_TEST
}

void test_remove_root() {
    TEST("Remove root node")
    BinarySearchTree<int> bst = {10, 5, 15};
    
    assert(bst.remove(10));
    assert(!bst.contains(10));
    assert(bst.size() == 2);
    assert(bst.is_valid());
    END_TEST
}

void test_remove_nonexistent() {
    TEST("Remove non-existent element")
    BinarySearchTree<int> bst = {5, 3, 7};
    
    assert(!bst.remove(100));
    assert(bst.size() == 3);
    END_TEST
}

void test_remove_all() {
    TEST("Remove all elements")
    BinarySearchTree<int> bst = {5, 3, 7, 1, 9};
    
    assert(bst.remove(5));
    assert(bst.remove(3));
    assert(bst.remove(7));
    assert(bst.remove(1));
    assert(bst.remove(9));
    
    assert(bst.empty());
    assert(bst.size() == 0);
    END_TEST
}

void test_contains() {
    TEST("contains()")
    BinarySearchTree<int> bst = {10, 5, 15, 3, 7};
    
    assert(bst.contains(10));
    assert(bst.contains(5));
    assert(bst.contains(15));
    assert(bst.contains(3));
    assert(bst.contains(7));
    assert(!bst.contains(100));
    assert(!bst.contains(0));
    assert(!bst.contains(6));
    END_TEST
}

void test_find() {
    TEST("find()")
    BinarySearchTree<int> bst = {10, 5, 15};
    
    const int* found = bst.find(10);
    assert(found != nullptr);
    assert(*found == 10);
    
    found = bst.find(5);
    assert(found != nullptr);
    assert(*found == 5);
    
    found = bst.find(100);
    assert(found == nullptr);
    END_TEST
}

void test_min() {
    TEST("min()")
    BinarySearchTree<int> bst = {10, 5, 15, 3, 7, 12, 20};
    assert(bst.min() == 3);
    
    bst.remove(3);
    assert(bst.min() == 5);
    
    bst.insert(1);
    assert(bst.min() == 1);
    END_TEST
}

void test_max() {
    TEST("max()")
    BinarySearchTree<int> bst = {10, 5, 15, 3, 7, 12, 20};
    assert(bst.max() == 20);
    
    bst.remove(20);
    assert(bst.max() == 15);
    
    bst.insert(100);
    assert(bst.max() == 100);
    END_TEST
}

void test_min_max_exception() {
    TEST("min()/max() on empty tree throws exception")
    BinarySearchTree<int> bst;
    
    bool exception_thrown = false;
    try {
        bst.min();
    } catch (const std::out_of_range&) {
        exception_thrown = true;
    }
    assert(exception_thrown);
    
    exception_thrown = false;
    try {
        bst.max();
    } catch (const std::out_of_range&) {
        exception_thrown = true;
    }
    assert(exception_thrown);
    END_TEST
}

void test_height() {
    TEST("height()")
    BinarySearchTree<int> bst;
    assert(bst.height() == 0);
    
    bst.insert(10);
    assert(bst.height() == 1);
    
    bst.insert(5);
    bst.insert(15);
    assert(bst.height() == 2);
    
    bst.insert(3);
    assert(bst.height() == 3);
    END_TEST
}

void test_clear() {
    TEST("clear()")
    BinarySearchTree<int> bst = {10, 5, 15, 3, 7, 12, 20};
    
    bst.clear();
    assert(bst.empty());
    assert(bst.size() == 0);
    assert(bst.height() == 0);
    
    // Should be reusable after clear
    bst.insert(100);
    assert(bst.size() == 1);
    assert(bst.contains(100));
    END_TEST
}

void test_swap() {
    TEST("swap()")
    BinarySearchTree<int> bst1 = {1, 2, 3};
    BinarySearchTree<int> bst2 = {10, 20};
    
    bst1.swap(bst2);
    
    assert(bst1.size() == 2);
    assert(bst1.contains(10));
    assert(bst1.contains(20));
    
    assert(bst2.size() == 3);
    assert(bst2.contains(1));
    assert(bst2.contains(2));
    assert(bst2.contains(3));
    END_TEST
}

void test_inorder_traversal() {
    TEST("inorder traversal")
    BinarySearchTree<int> bst = {5, 3, 7, 1, 9, 4, 6};
    
    std::vector<int> result;
    bst.inorder([&result](const int& val) {
        result.push_back(val);
    });
    
    // Should be sorted
    assert(result.size() == 7);
    assert(result[0] == 1);
    assert(result[1] == 3);
    assert(result[2] == 4);
    assert(result[3] == 5);
    assert(result[4] == 6);
    assert(result[5] == 7);
    assert(result[6] == 9);
    END_TEST
}

void test_preorder_traversal() {
    TEST("preorder traversal")
    BinarySearchTree<int> bst = {5, 3, 7, 1, 4};
    
    std::vector<int> result;
    bst.preorder([&result](const int& val) {
        result.push_back(val);
    });
    
    // Root first, then left subtree, then right subtree
    assert(result.size() == 5);
    assert(result[0] == 5);  // Root
    assert(result[1] == 3);  // Left child
    assert(result[2] == 1);  // Left-left
    assert(result[3] == 4);  // Left-right
    assert(result[4] == 7);  // Right child
    END_TEST
}

void test_postorder_traversal() {
    TEST("postorder traversal")
    BinarySearchTree<int> bst = {5, 3, 7, 1, 4};
    
    std::vector<int> result;
    bst.postorder([&result](const int& val) {
        result.push_back(val);
    });
    
    // Left subtree, right subtree, then root
    assert(result.size() == 5);
    assert(result[0] == 1);  // Left-left
    assert(result[1] == 4);  // Left-right
    assert(result[2] == 3);  // Left child
    assert(result[3] == 7);  // Right child
    assert(result[4] == 5);  // Root
    END_TEST
}

void test_levelorder_traversal() {
    TEST("levelorder traversal")
    BinarySearchTree<int> bst = {5, 3, 7, 1, 4, 6, 9};
    
    std::vector<int> result;
    bst.levelorder([&result](const int& val) {
        result.push_back(val);
    });
    
    // Level by level
    assert(result.size() == 7);
    assert(result[0] == 5);  // Level 0
    assert(result[1] == 3);  // Level 1
    assert(result[2] == 7);  // Level 1
    assert(result[3] == 1);  // Level 2
    assert(result[4] == 4);  // Level 2
    assert(result[5] == 6);  // Level 2
    assert(result[6] == 9);  // Level 2
    END_TEST
}

void test_successor() {
    TEST("successor()")
    BinarySearchTree<int> bst = {20, 10, 30, 5, 15, 25, 35};
    
    const int* succ = bst.successor(20);
    assert(succ != nullptr && *succ == 25);
    
    succ = bst.successor(10);
    assert(succ != nullptr && *succ == 15);
    
    succ = bst.successor(5);
    assert(succ != nullptr && *succ == 10);
    
    succ = bst.successor(35);
    assert(succ == nullptr);  // No successor for max
    
    succ = bst.successor(15);
    assert(succ != nullptr && *succ == 20);
    END_TEST
}

void test_predecessor() {
    TEST("predecessor()")
    BinarySearchTree<int> bst = {20, 10, 30, 5, 15, 25, 35};
    
    const int* pred = bst.predecessor(20);
    assert(pred != nullptr && *pred == 15);
    
    pred = bst.predecessor(30);
    assert(pred != nullptr && *pred == 25);
    
    pred = bst.predecessor(35);
    assert(pred != nullptr && *pred == 30);
    
    pred = bst.predecessor(5);
    assert(pred == nullptr);  // No predecessor for min
    
    pred = bst.predecessor(25);
    assert(pred != nullptr && *pred == 20);
    END_TEST
}

void test_is_valid() {
    TEST("is_valid()")
    BinarySearchTree<int> bst = {10, 5, 15, 3, 7, 12, 20};
    assert(bst.is_valid());
    
    BinarySearchTree<int> empty_bst;
    assert(empty_bst.is_valid());
    
    BinarySearchTree<int> single;
    single.insert(42);
    assert(single.is_valid());
    END_TEST
}

void test_large_dataset() {
    TEST("Large dataset (stress test)")
    BinarySearchTree<int> bst;
    const int count = 1000;
    
    // Insert elements
    for (int i = 0; i < count; ++i) {
        // Insert in mixed order for better balance
        int val = (i * 17) % count;
        bst.insert(val);
    }
    
    assert(bst.size() == count);
    assert(bst.is_valid());
    
    // Verify all elements exist
    for (int i = 0; i < count; ++i) {
        assert(bst.contains(i));
    }
    
    // Remove half
    for (int i = 0; i < count / 2; ++i) {
        assert(bst.remove(i));
    }
    
    assert(bst.size() == count / 2);
    assert(bst.is_valid());
    END_TEST
}

void test_with_doubles() {
    TEST("BinarySearchTree<double>")
    BinarySearchTree<double> bst = {3.14, 1.41, 2.71, 1.73};
    
    assert(bst.size() == 4);
    assert(bst.contains(3.14));
    assert(bst.contains(1.41));
    assert(bst.min() < 1.42);
    assert(bst.max() > 3.13);
    END_TEST
}

void test_single_element() {
    TEST("Single element operations")
    BinarySearchTree<int> bst;
    
    bst.insert(42);
    assert(bst.size() == 1);
    assert(bst.height() == 1);
    assert(bst.min() == 42);
    assert(bst.max() == 42);
    assert(bst.contains(42));
    
    bst.remove(42);
    assert(bst.empty());
    END_TEST
}

void test_traversal_empty() {
    TEST("Traversal on empty tree")
    BinarySearchTree<int> bst;
    
    std::vector<int> result;
    bst.inorder([&result](const int& val) {
        result.push_back(val);
    });
    assert(result.empty());
    
    bst.preorder([&result](const int& val) {
        result.push_back(val);
    });
    assert(result.empty());
    
    bst.postorder([&result](const int& val) {
        result.push_back(val);
    });
    assert(result.empty());
    
    bst.levelorder([&result](const int& val) {
        result.push_back(val);
    });
    assert(result.empty());
    END_TEST
}

void test_inorder_sorted_output() {
    TEST("Inorder traversal produces sorted output")
    BinarySearchTree<int> bst = {50, 30, 70, 20, 40, 60, 80, 10, 25, 35, 45};
    
    std::vector<int> result;
    bst.inorder([&result](const int& val) {
        result.push_back(val);
    });
    
    // Verify sorted
    for (size_t i = 1; i < result.size(); ++i) {
        assert(result[i] > result[i - 1]);
    }
    END_TEST
}

void test_remove_and_reinsert() {
    TEST("Remove and reinsert elements")
    BinarySearchTree<int> bst = {10, 5, 15};
    
    bst.remove(5);
    assert(!bst.contains(5));
    
    bst.insert(5);
    assert(bst.contains(5));
    assert(bst.size() == 3);
    assert(bst.is_valid());
    END_TEST
}

void test_skewed_tree_left() {
    TEST("Left-skewed tree")
    BinarySearchTree<int> bst;
    
    // Insert in descending order
    for (int i = 10; i >= 1; --i) {
        bst.insert(i);
    }
    
    assert(bst.size() == 10);
    assert(bst.height() == 10);  // Completely skewed
    assert(bst.min() == 1);
    assert(bst.max() == 10);
    assert(bst.is_valid());
    END_TEST
}

void test_skewed_tree_right() {
    TEST("Right-skewed tree")
    BinarySearchTree<int> bst;
    
    // Insert in ascending order
    for (int i = 1; i <= 10; ++i) {
        bst.insert(i);
    }
    
    assert(bst.size() == 10);
    assert(bst.height() == 10);  // Completely skewed
    assert(bst.min() == 1);
    assert(bst.max() == 10);
    assert(bst.is_valid());
    END_TEST
}

// Main test runner
int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "BinarySearchTree Test Suite" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;

    // Constructor tests
    test_default_constructor();
    test_initializer_list();
    test_copy_constructor();
    test_move_constructor();
    test_copy_assignment();
    test_move_assignment();

    // Insert tests
    test_insert_basic();
    test_insert_duplicate();
    test_insert_ordered();
    test_insert_balanced();

    // Remove tests
    test_remove_leaf();
    test_remove_one_child();
    test_remove_two_children();
    test_remove_root();
    test_remove_nonexistent();
    test_remove_all();

    // Lookup tests
    test_contains();
    test_find();
    test_min();
    test_max();
    test_min_max_exception();

    // Capacity tests
    test_height();
    test_clear();
    test_swap();

    // Traversal tests
    test_inorder_traversal();
    test_preorder_traversal();
    test_postorder_traversal();
    test_levelorder_traversal();
    test_traversal_empty();
    test_inorder_sorted_output();

    // Advanced operation tests
    test_successor();
    test_predecessor();
    test_is_valid();

    // Edge case tests
    test_single_element();
    test_remove_and_reinsert();
    test_skewed_tree_left();
    test_skewed_tree_right();

    // Stress tests
    test_large_dataset();
    test_with_doubles();

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