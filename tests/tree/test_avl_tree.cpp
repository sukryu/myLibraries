/**
 * @file test_avl_tree.cpp
 * @brief Test suite for AVLTree class
 * @author Jinhyeok
 * @date 2025-11-29
 * @version 1.0.0
 */

#include "tree/avl_tree.hpp"
#include <iostream>
#include <cassert>
#include <vector>
#include <string>
#include <cmath>

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
    AVLTree<int> avl;
    assert(avl.empty());
    assert(avl.size() == 0);
    assert(avl.height() == 0);
    assert(avl.is_balanced());
    END_TEST
}

void test_initializer_list() {
    TEST("Initializer list constructor")
    AVLTree<int> avl = {5, 3, 7, 1, 9};
    assert(avl.size() == 5);
    assert(!avl.empty());
    assert(avl.contains(5));
    assert(avl.contains(3));
    assert(avl.contains(7));
    assert(avl.is_balanced());
    assert(avl.is_valid());
    END_TEST
}

void test_copy_constructor() {
    TEST("Copy constructor")
    AVLTree<int> avl1 = {5, 3, 7, 1, 9};
    AVLTree<int> avl2(avl1);
    
    assert(avl2.size() == 5);
    assert(avl2.contains(5));
    assert(avl2.is_balanced());
    
    // Verify deep copy
    avl2.insert(100);
    assert(avl2.contains(100));
    assert(!avl1.contains(100));
    END_TEST
}

void test_move_constructor() {
    TEST("Move constructor")
    AVLTree<int> avl1 = {5, 3, 7};
    AVLTree<int> avl2(std::move(avl1));
    
    assert(avl2.size() == 3);
    assert(avl2.contains(5));
    assert(avl2.is_balanced());
    assert(avl1.empty());
    END_TEST
}

void test_copy_assignment() {
    TEST("Copy assignment operator")
    AVLTree<int> avl1 = {5, 3, 7};
    AVLTree<int> avl2;
    avl2 = avl1;
    
    assert(avl2.size() == 3);
    assert(avl2.is_balanced());
    
    avl2.insert(100);
    assert(!avl1.contains(100));
    END_TEST
}

void test_move_assignment() {
    TEST("Move assignment operator")
    AVLTree<int> avl1 = {5, 3, 7};
    AVLTree<int> avl2;
    avl2 = std::move(avl1);
    
    assert(avl2.size() == 3);
    assert(avl2.is_balanced());
    assert(avl1.empty());
    END_TEST
}

void test_insert_basic() {
    TEST("Basic insert")
    AVLTree<int> avl;
    
    assert(avl.insert(10));
    assert(avl.size() == 1);
    assert(avl.contains(10));
    assert(avl.is_balanced());
    
    assert(avl.insert(5));
    assert(avl.insert(15));
    assert(avl.size() == 3);
    assert(avl.is_balanced());
    END_TEST
}

void test_insert_duplicate() {
    TEST("Insert duplicate (should fail)")
    AVLTree<int> avl;
    
    assert(avl.insert(10));
    assert(!avl.insert(10));
    assert(avl.size() == 1);
    
    assert(avl.insert(5));
    assert(!avl.insert(5));
    assert(avl.size() == 2);
    assert(avl.is_balanced());
    END_TEST
}

void test_ll_rotation() {
    TEST("LL rotation (right rotation)")
    AVLTree<int> avl;
    
    // Insert in descending order to trigger LL case
    avl.insert(30);
    avl.insert(20);
    assert(avl.is_balanced());
    
    avl.insert(10);  // Should trigger right rotation
    assert(avl.is_balanced());
    assert(avl.is_valid());
    assert(avl.height() == 2);  // Balanced: height should be 2, not 3
    END_TEST
}

void test_rr_rotation() {
    TEST("RR rotation (left rotation)")
    AVLTree<int> avl;
    
    // Insert in ascending order to trigger RR case
    avl.insert(10);
    avl.insert(20);
    assert(avl.is_balanced());
    
    avl.insert(30);  // Should trigger left rotation
    assert(avl.is_balanced());
    assert(avl.is_valid());
    assert(avl.height() == 2);
    END_TEST
}

void test_lr_rotation() {
    TEST("LR rotation (left-right rotation)")
    AVLTree<int> avl;
    
    // Insert to trigger LR case
    avl.insert(30);
    avl.insert(10);
    avl.insert(20);  // Should trigger LR rotation
    
    assert(avl.is_balanced());
    assert(avl.is_valid());
    assert(avl.height() == 2);
    END_TEST
}

void test_rl_rotation() {
    TEST("RL rotation (right-left rotation)")
    AVLTree<int> avl;
    
    // Insert to trigger RL case
    avl.insert(10);
    avl.insert(30);
    avl.insert(20);  // Should trigger RL rotation
    
    assert(avl.is_balanced());
    assert(avl.is_valid());
    assert(avl.height() == 2);
    END_TEST
}

void test_insert_ascending_order() {
    TEST("Insert in ascending order (stays balanced)")
    AVLTree<int> avl;
    
    // In BST this would create a skewed tree with height 10
    // In AVL it should stay balanced
    for (int i = 1; i <= 10; ++i) {
        avl.insert(i);
        assert(avl.is_balanced());
    }
    
    assert(avl.size() == 10);
    assert(avl.is_valid());
    
    // Height should be O(log n), not n
    // For 10 elements, max height is 4 (ceil(log2(10+1)) = 4)
    assert(avl.height() <= 4);
    END_TEST
}

void test_insert_descending_order() {
    TEST("Insert in descending order (stays balanced)")
    AVLTree<int> avl;
    
    for (int i = 10; i >= 1; --i) {
        avl.insert(i);
        assert(avl.is_balanced());
    }
    
    assert(avl.size() == 10);
    assert(avl.is_valid());
    assert(avl.height() <= 4);
    END_TEST
}

void test_remove_leaf() {
    TEST("Remove leaf node")
    AVLTree<int> avl = {5, 3, 7, 1, 9};
    
    assert(avl.remove(1));
    assert(!avl.contains(1));
    assert(avl.size() == 4);
    assert(avl.is_balanced());
    assert(avl.is_valid());
    
    assert(avl.remove(9));
    assert(!avl.contains(9));
    assert(avl.size() == 3);
    assert(avl.is_balanced());
    END_TEST
}

void test_remove_one_child() {
    TEST("Remove node with one child")
    AVLTree<int> avl;
    avl.insert(10);
    avl.insert(5);
    avl.insert(15);
    avl.insert(3);
    
    assert(avl.remove(5));
    assert(!avl.contains(5));
    assert(avl.contains(3));
    assert(avl.is_balanced());
    assert(avl.is_valid());
    END_TEST
}

void test_remove_two_children() {
    TEST("Remove node with two children")
    AVLTree<int> avl = {10, 5, 15, 3, 7, 12, 20};
    
    assert(avl.remove(5));
    assert(!avl.contains(5));
    assert(avl.contains(3));
    assert(avl.contains(7));
    assert(avl.is_balanced());
    assert(avl.is_valid());
    END_TEST
}

void test_remove_root() {
    TEST("Remove root node")
    AVLTree<int> avl = {10, 5, 15, 3, 7, 12, 20};
    
    assert(avl.remove(10));
    assert(!avl.contains(10));
    assert(avl.size() == 6);
    assert(avl.is_balanced());
    assert(avl.is_valid());
    END_TEST
}

void test_remove_triggers_rebalance() {
    TEST("Remove triggers rebalancing")
    AVLTree<int> avl;
    
    // Build a tree that will need rebalancing after removal
    avl.insert(20);
    avl.insert(10);
    avl.insert(30);
    avl.insert(5);
    avl.insert(15);
    avl.insert(25);
    avl.insert(35);
    avl.insert(3);
    avl.insert(7);
    
    // Remove nodes from right side to trigger rebalance
    avl.remove(35);
    assert(avl.is_balanced());
    
    avl.remove(30);
    assert(avl.is_balanced());
    
    avl.remove(25);
    assert(avl.is_balanced());
    assert(avl.is_valid());
    END_TEST
}

void test_remove_nonexistent() {
    TEST("Remove non-existent element")
    AVLTree<int> avl = {5, 3, 7};
    
    assert(!avl.remove(100));
    assert(avl.size() == 3);
    assert(avl.is_balanced());
    END_TEST
}

void test_remove_all() {
    TEST("Remove all elements")
    AVLTree<int> avl = {5, 3, 7, 1, 9};
    
    assert(avl.remove(5));
    assert(avl.is_balanced());
    assert(avl.remove(3));
    assert(avl.is_balanced());
    assert(avl.remove(7));
    assert(avl.is_balanced());
    assert(avl.remove(1));
    assert(avl.is_balanced());
    assert(avl.remove(9));
    
    assert(avl.empty());
    assert(avl.is_balanced());
    END_TEST
}

void test_contains() {
    TEST("contains()")
    AVLTree<int> avl = {10, 5, 15, 3, 7};
    
    assert(avl.contains(10));
    assert(avl.contains(5));
    assert(avl.contains(15));
    assert(!avl.contains(100));
    assert(!avl.contains(0));
    END_TEST
}

void test_find() {
    TEST("find()")
    AVLTree<int> avl = {10, 5, 15};
    
    const int* found = avl.find(10);
    assert(found != nullptr);
    assert(*found == 10);
    
    found = avl.find(100);
    assert(found == nullptr);
    END_TEST
}

void test_min() {
    TEST("min()")
    AVLTree<int> avl = {10, 5, 15, 3, 7, 12, 20};
    assert(avl.min() == 3);
    
    avl.remove(3);
    assert(avl.min() == 5);
    END_TEST
}

void test_max() {
    TEST("max()")
    AVLTree<int> avl = {10, 5, 15, 3, 7, 12, 20};
    assert(avl.max() == 20);
    
    avl.remove(20);
    assert(avl.max() == 15);
    END_TEST
}

void test_min_max_exception() {
    TEST("min()/max() on empty tree throws exception")
    AVLTree<int> avl;
    
    bool exception_thrown = false;
    try {
        avl.min();
    } catch (const std::out_of_range&) {
        exception_thrown = true;
    }
    assert(exception_thrown);
    
    exception_thrown = false;
    try {
        avl.max();
    } catch (const std::out_of_range&) {
        exception_thrown = true;
    }
    assert(exception_thrown);
    END_TEST
}

void test_height() {
    TEST("height()")
    AVLTree<int> avl;
    assert(avl.height() == 0);
    
    avl.insert(10);
    assert(avl.height() == 1);
    
    avl.insert(5);
    avl.insert(15);
    assert(avl.height() == 2);
    END_TEST
}

void test_balance_factor() {
    TEST("balance_factor()")
    AVLTree<int> avl;
    assert(avl.balance_factor() == 0);
    
    avl.insert(10);
    assert(avl.balance_factor() == 0);
    
    avl.insert(5);
    assert(avl.balance_factor() == 1);  // Left heavy
    
    avl.insert(15);
    assert(avl.balance_factor() == 0);  // Balanced
    END_TEST
}

void test_clear() {
    TEST("clear()")
    AVLTree<int> avl = {10, 5, 15, 3, 7, 12, 20};
    
    avl.clear();
    assert(avl.empty());
    assert(avl.size() == 0);
    assert(avl.height() == 0);
    assert(avl.is_balanced());
    
    // Reuse after clear
    avl.insert(100);
    assert(avl.size() == 1);
    assert(avl.contains(100));
    END_TEST
}

void test_swap() {
    TEST("swap()")
    AVLTree<int> avl1 = {1, 2, 3};
    AVLTree<int> avl2 = {10, 20};
    
    avl1.swap(avl2);
    
    assert(avl1.size() == 2);
    assert(avl1.contains(10));
    assert(avl2.size() == 3);
    assert(avl2.contains(1));
    assert(avl1.is_balanced());
    assert(avl2.is_balanced());
    END_TEST
}

void test_inorder_traversal() {
    TEST("inorder traversal (sorted output)")
    AVLTree<int> avl = {5, 3, 7, 1, 9, 4, 6};
    
    std::vector<int> result;
    avl.inorder([&result](const int& val) {
        result.push_back(val);
    });
    
    // Should be sorted
    assert(result.size() == 7);
    for (size_t i = 1; i < result.size(); ++i) {
        assert(result[i] > result[i - 1]);
    }
    END_TEST
}

void test_preorder_traversal() {
    TEST("preorder traversal")
    AVLTree<int> avl = {5, 3, 7};
    
    std::vector<int> result;
    avl.preorder([&result](const int& val) {
        result.push_back(val);
    });
    
    assert(result.size() == 3);
    assert(result[0] == 5);  // Root first
    END_TEST
}

void test_postorder_traversal() {
    TEST("postorder traversal")
    AVLTree<int> avl = {5, 3, 7};
    
    std::vector<int> result;
    avl.postorder([&result](const int& val) {
        result.push_back(val);
    });
    
    assert(result.size() == 3);
    assert(result[2] == 5);  // Root last
    END_TEST
}

void test_levelorder_traversal() {
    TEST("levelorder traversal")
    AVLTree<int> avl = {5, 3, 7, 1, 4, 6, 9};
    
    std::vector<int> result;
    avl.levelorder([&result](const int& val) {
        result.push_back(val);
    });
    
    assert(result.size() == 7);
    // First element should be root
    assert(result[0] == 5);
    END_TEST
}

void test_traversal_empty() {
    TEST("Traversal on empty tree")
    AVLTree<int> avl;
    
    std::vector<int> result;
    avl.inorder([&result](const int& val) {
        result.push_back(val);
    });
    assert(result.empty());
    END_TEST
}

void test_is_balanced() {
    TEST("is_balanced()")
    AVLTree<int> avl;
    assert(avl.is_balanced());
    
    // Insert many elements - should always stay balanced
    for (int i = 1; i <= 100; ++i) {
        avl.insert(i);
        assert(avl.is_balanced());
    }
    END_TEST
}

void test_is_valid() {
    TEST("is_valid()")
    AVLTree<int> avl = {10, 5, 15, 3, 7, 12, 20};
    assert(avl.is_valid());
    
    AVLTree<int> empty_avl;
    assert(empty_avl.is_valid());
    END_TEST
}

void test_large_dataset() {
    TEST("Large dataset (stress test)")
    AVLTree<int> avl;
    const int count = 10000;
    
    // Insert elements
    for (int i = 0; i < count; ++i) {
        avl.insert(i);
    }
    
    assert(avl.size() == count);
    assert(avl.is_balanced());
    assert(avl.is_valid());
    
    // Height should be O(log n)
    // For 10000 elements, log2(10000) ≈ 13.3, so height should be around 14-20
    assert(avl.height() <= 20);
    
    // Verify all elements exist
    for (int i = 0; i < count; ++i) {
        assert(avl.contains(i));
    }
    
    // Remove half
    for (int i = 0; i < count / 2; ++i) {
        assert(avl.remove(i));
        assert(avl.is_balanced());
    }
    
    assert(avl.size() == count / 2);
    assert(avl.is_valid());
    END_TEST
}

void test_avl_vs_bst_height_comparison() {
    TEST("AVL height vs BST worst case")
    AVLTree<int> avl;
    
    // Insert 1000 elements in ascending order
    // BST would have height 1000, AVL should have ~10
    for (int i = 1; i <= 1000; ++i) {
        avl.insert(i);
    }
    
    assert(avl.size() == 1000);
    assert(avl.is_balanced());
    
    // log2(1000) ≈ 10, so height should be around 10-15
    size_t height = avl.height();
    assert(height <= 15);
    assert(height >= 10);
    END_TEST
}

void test_with_doubles() {
    TEST("AVLTree<double>")
    AVLTree<double> avl = {3.14, 1.41, 2.71, 1.73};
    
    assert(avl.size() == 4);
    assert(avl.contains(3.14));
    assert(avl.is_balanced());
    assert(avl.is_valid());
    END_TEST
}

void test_single_element() {
    TEST("Single element operations")
    AVLTree<int> avl;
    
    avl.insert(42);
    assert(avl.size() == 1);
    assert(avl.height() == 1);
    assert(avl.min() == 42);
    assert(avl.max() == 42);
    assert(avl.is_balanced());
    
    avl.remove(42);
    assert(avl.empty());
    assert(avl.is_balanced());
    END_TEST
}

void test_remove_and_reinsert() {
    TEST("Remove and reinsert elements")
    AVLTree<int> avl = {10, 5, 15};
    
    avl.remove(5);
    assert(!avl.contains(5));
    assert(avl.is_balanced());
    
    avl.insert(5);
    assert(avl.contains(5));
    assert(avl.is_balanced());
    assert(avl.is_valid());
    END_TEST
}

void test_multiple_rotations() {
    TEST("Multiple rotations during insertions")
    AVLTree<int> avl;
    
    // This sequence triggers multiple different rotations
    int values[] = {50, 25, 75, 10, 30, 60, 80, 5, 15, 27, 55, 1};
    
    for (int v : values) {
        avl.insert(v);
        assert(avl.is_balanced());
        assert(avl.is_valid());
    }
    
    assert(avl.size() == 12);
    END_TEST
}

void test_zigzag_insertion() {
    TEST("Zigzag insertion pattern")
    AVLTree<int> avl;
    
    // Alternating high-low pattern
    avl.insert(50);
    avl.insert(10);
    avl.insert(90);
    avl.insert(20);
    avl.insert(80);
    avl.insert(30);
    avl.insert(70);
    
    assert(avl.size() == 7);
    assert(avl.is_balanced());
    assert(avl.is_valid());
    END_TEST
}

void test_remove_sequence() {
    TEST("Remove in specific sequence")
    AVLTree<int> avl;
    
    // Build tree
    for (int i = 1; i <= 15; ++i) {
        avl.insert(i);
    }
    
    // Remove in middle-out pattern
    avl.remove(8);
    assert(avl.is_balanced());
    avl.remove(4);
    assert(avl.is_balanced());
    avl.remove(12);
    assert(avl.is_balanced());
    avl.remove(2);
    assert(avl.is_balanced());
    avl.remove(14);
    assert(avl.is_balanced());
    
    assert(avl.is_valid());
    END_TEST
}

void test_clear_and_rebuild() {
    TEST("Clear and rebuild tree")
    AVLTree<int> avl;
    
    for (int i = 1; i <= 100; ++i) {
        avl.insert(i);
    }
    assert(avl.size() == 100);
    
    avl.clear();
    assert(avl.empty());
    
    for (int i = 100; i >= 1; --i) {
        avl.insert(i);
    }
    assert(avl.size() == 100);
    assert(avl.is_balanced());
    assert(avl.is_valid());
    END_TEST
}

// Main test runner
int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "AVLTree Test Suite" << std::endl;
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

    // Rotation tests (AVL-specific)
    test_ll_rotation();
    test_rr_rotation();
    test_lr_rotation();
    test_rl_rotation();
    test_insert_ascending_order();
    test_insert_descending_order();

    // Remove tests
    test_remove_leaf();
    test_remove_one_child();
    test_remove_two_children();
    test_remove_root();
    test_remove_triggers_rebalance();
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
    test_balance_factor();
    test_clear();
    test_swap();

    // Traversal tests
    test_inorder_traversal();
    test_preorder_traversal();
    test_postorder_traversal();
    test_levelorder_traversal();
    test_traversal_empty();

    // Validation tests
    test_is_balanced();
    test_is_valid();

    // Edge case tests
    test_single_element();
    test_remove_and_reinsert();
    test_multiple_rotations();
    test_zigzag_insertion();
    test_remove_sequence();
    test_clear_and_rebuild();

    // Stress tests
    test_large_dataset();
    test_avl_vs_bst_height_comparison();
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