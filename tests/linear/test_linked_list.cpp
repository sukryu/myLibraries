/**
 * @file test_linked_list.cpp
 * @brief Test suite for LinkedList class
 * @author Jinhyeok
 * @date 2025-11-29
 * @version 1.0.0
 */

#include "linear/linked_list.hpp"
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
    LinkedList<int> list;
    assert(list.empty());
    assert(list.size() == 0);
    END_TEST
}

void test_constructor_with_value() {
    TEST("Constructor with count and value")
    LinkedList<int> list(5, 42);
    assert(list.size() == 5);
    for (size_t i = 0; i < list.size(); ++i) {
        assert(list[i] == 42);
    }
    END_TEST
}

void test_initializer_list() {
    TEST("Initializer list constructor")
    LinkedList<int> list = {1, 2, 3, 4, 5};
    assert(list.size() == 5);
    assert(list[0] == 1);
    assert(list[2] == 3);
    assert(list[4] == 5);
    END_TEST
}

void test_copy_constructor() {
    TEST("Copy constructor")
    LinkedList<int> list1 = {1, 2, 3};
    LinkedList<int> list2(list1);
    assert(list2.size() == 3);
    assert(list2[0] == 1);
    assert(list2[1] == 2);
    assert(list2[2] == 3);
    
    // Verify deep copy
    list2[0] = 99;
    assert(list1[0] == 1);
    END_TEST
}

void test_move_constructor() {
    TEST("Move constructor")
    LinkedList<int> list1 = {1, 2, 3};
    LinkedList<int> list2(std::move(list1));
    assert(list2.size() == 3);
    assert(list2[0] == 1);
    assert(list1.empty());
    END_TEST
}

void test_push_front() {
    TEST("push_front")
    LinkedList<int> list;
    list.push_front(3);
    list.push_front(2);
    list.push_front(1);
    assert(list.size() == 3);
    assert(list[0] == 1);
    assert(list[1] == 2);
    assert(list[2] == 3);
    END_TEST
}

void test_push_back() {
    TEST("push_back")
    LinkedList<int> list;
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);
    assert(list.size() == 3);
    assert(list[0] == 1);
    assert(list[1] == 2);
    assert(list[2] == 3);
    END_TEST
}

void test_pop_front() {
    TEST("pop_front")
    LinkedList<int> list = {1, 2, 3, 4, 5};
    list.pop_front();
    assert(list.size() == 4);
    assert(list.front() == 2);
    list.pop_front();
    assert(list.front() == 3);
    END_TEST
}

void test_pop_back() {
    TEST("pop_back")
    LinkedList<int> list = {1, 2, 3, 4, 5};
    list.pop_back();
    assert(list.size() == 4);
    assert(list.back() == 4);
    list.pop_back();
    assert(list.back() == 3);
    END_TEST
}

void test_front_back() {
    TEST("front() and back()")
    LinkedList<int> list = {1, 2, 3, 4, 5};
    assert(list.front() == 1);
    assert(list.back() == 5);
    list.front() = 100;
    list.back() = 500;
    assert(list.front() == 100);
    assert(list.back() == 500);
    END_TEST
}

void test_at_method() {
    TEST("at() method with bounds checking")
    LinkedList<int> list = {10, 20, 30};
    assert(list.at(0) == 10);
    assert(list.at(2) == 30);
    
    bool exception_thrown = false;
    try {
        list.at(10);
    } catch (const std::out_of_range&) {
        exception_thrown = true;
    }
    assert(exception_thrown);
    END_TEST
}

void test_insert() {
    TEST("insert() at various positions")
    LinkedList<int> list = {1, 2, 4, 5};
    list.insert(2, 3);  // Insert 3 at index 2
    assert(list.size() == 5);
    assert(list[2] == 3);
    
    list.insert(0, 0);  // Insert at front
    assert(list[0] == 0);
    
    list.insert(list.size(), 6);  // Insert at back
    assert(list.back() == 6);
    END_TEST
}

void test_erase() {
    TEST("erase() at various positions")
    LinkedList<int> list = {1, 2, 3, 4, 5};
    list.erase(2);  // Remove element at index 2
    assert(list.size() == 4);
    assert(list[2] == 4);
    
    list.erase(0);  // Remove first
    assert(list.front() == 2);
    
    list.erase(list.size() - 1);  // Remove last
    assert(list.back() == 4);
    END_TEST
}

void test_clear() {
    TEST("clear()")
    LinkedList<int> list = {1, 2, 3, 4, 5};
    list.clear();
    assert(list.empty());
    assert(list.size() == 0);
    END_TEST
}

void test_resize() {
    TEST("resize()")
    LinkedList<int> list = {1, 2, 3};
    list.resize(5);
    assert(list.size() == 5);
    
    list.resize(2);
    assert(list.size() == 2);
    assert(list[0] == 1);
    assert(list[1] == 2);
    END_TEST
}

void test_resize_with_value() {
    TEST("resize() with default value")
    LinkedList<int> list = {1, 2, 3};
    list.resize(6, 99);
    assert(list.size() == 6);
    assert(list[3] == 99);
    assert(list[4] == 99);
    assert(list[5] == 99);
    END_TEST
}

void test_reverse() {
    TEST("reverse()")
    LinkedList<int> list = {1, 2, 3, 4, 5};
    list.reverse();
    assert(list[0] == 5);
    assert(list[1] == 4);
    assert(list[2] == 3);
    assert(list[3] == 2);
    assert(list[4] == 1);
    
    // Reverse again to get back original
    list.reverse();
    assert(list[0] == 1);
    assert(list[4] == 5);
    END_TEST
}

void test_remove() {
    TEST("remove() specific value")
    LinkedList<int> list = {1, 2, 3, 2, 4, 2, 5};
    size_t removed = list.remove(2);
    assert(removed == 3);
    assert(list.size() == 4);
    assert(!list.contains(2));
    END_TEST
}

void test_find() {
    TEST("find() element")
    LinkedList<int> list = {10, 20, 30, 40, 50};
    assert(list.find(30) == 2);
    assert(list.find(10) == 0);
    assert(list.find(50) == 4);
    assert(list.find(99) == list.size());  // Not found
    END_TEST
}

void test_contains() {
    TEST("contains() element")
    LinkedList<int> list = {1, 2, 3, 4, 5};
    assert(list.contains(3));
    assert(list.contains(1));
    assert(list.contains(5));
    assert(!list.contains(10));
    END_TEST
}

void test_copy_assignment() {
    TEST("Copy assignment operator")
    LinkedList<int> list1 = {1, 2, 3};
    LinkedList<int> list2;
    list2 = list1;
    assert(list2.size() == 3);
    assert(list2[1] == 2);
    
    list2[0] = 99;
    assert(list1[0] == 1);
    END_TEST
}

void test_move_assignment() {
    TEST("Move assignment operator")
    LinkedList<int> list1 = {1, 2, 3};
    LinkedList<int> list2;
    list2 = std::move(list1);
    assert(list2.size() == 3);
    assert(list2[0] == 1);
    assert(list1.empty());
    END_TEST
}

void test_swap() {
    TEST("swap()")
    LinkedList<int> list1 = {1, 2, 3};
    LinkedList<int> list2 = {10, 20};
    list1.swap(list2);
    assert(list1.size() == 2);
    assert(list1[0] == 10);
    assert(list2.size() == 3);
    assert(list2[0] == 1);
    END_TEST
}

void test_exception_empty_front() {
    TEST("Exception on empty list front()")
    LinkedList<int> list;
    bool exception_thrown = false;
    try {
        list.front();
    } catch (const std::out_of_range&) {
        exception_thrown = true;
    }
    assert(exception_thrown);
    END_TEST
}

void test_exception_empty_back() {
    TEST("Exception on empty list back()")
    LinkedList<int> list;
    bool exception_thrown = false;
    try {
        list.back();
    } catch (const std::out_of_range&) {
        exception_thrown = true;
    }
    assert(exception_thrown);
    END_TEST
}

void test_exception_empty_pop_front() {
    TEST("Exception on empty list pop_front()")
    LinkedList<int> list;
    bool exception_thrown = false;
    try {
        list.pop_front();
    } catch (const std::out_of_range&) {
        exception_thrown = true;
    }
    assert(exception_thrown);
    END_TEST
}

void test_exception_empty_pop_back() {
    TEST("Exception on empty list pop_back()")
    LinkedList<int> list;
    bool exception_thrown = false;
    try {
        list.pop_back();
    } catch (const std::out_of_range&) {
        exception_thrown = true;
    }
    assert(exception_thrown);
    END_TEST
}

void test_mixed_operations() {
    TEST("Mixed push/pop operations")
    LinkedList<int> list;
    list.push_back(1);
    list.push_front(0);
    list.push_back(2);
    list.pop_front();
    list.push_front(-1);
    list.pop_back();
    
    assert(list.size() == 2);
    assert(list[0] == -1);
    assert(list[1] == 1);
    END_TEST
}

void test_large_dataset() {
    TEST("Large dataset (stress test)")
    LinkedList<int> list;
    const int count = 1000;
    
    // Add many elements
    for (int i = 0; i < count; ++i) {
        list.push_back(i);
    }
    assert(list.size() == count);
    
    // Verify values
    for (int i = 0; i < count; ++i) {
        assert(list[i] == i);
    }
    
    // Remove from front
    for (int i = 0; i < count / 2; ++i) {
        list.pop_front();
    }
    assert(list.size() == count / 2);
    END_TEST
}

void test_with_doubles() {
    TEST("LinkedList<double>")
    LinkedList<double> list = {1.1, 2.2, 3.3};
    assert(list.size() == 3);
    assert(list[0] > 1.0 && list[0] < 1.2);
    list.push_back(4.4);
    assert(list.size() == 4);
    END_TEST
}

void test_single_element() {
    TEST("Single element operations")
    LinkedList<int> list;
    list.push_back(42);
    assert(list.size() == 1);
    assert(list.front() == 42);
    assert(list.back() == 42);
    list.pop_back();
    assert(list.empty());
    END_TEST
}

void test_alternating_ends() {
    TEST("Alternating front/back operations")
    LinkedList<int> list;
    list.push_back(2);
    list.push_front(1);
    list.push_back(3);
    list.push_front(0);
    
    assert(list[0] == 0);
    assert(list[1] == 1);
    assert(list[2] == 2);
    assert(list[3] == 3);
    END_TEST
}

void test_clear_and_reuse() {
    TEST("Clear and reuse list")
    LinkedList<int> list = {1, 2, 3, 4, 5};
    list.clear();
    assert(list.empty());
    
    list.push_back(10);
    list.push_back(20);
    assert(list.size() == 2);
    assert(list[0] == 10);
    assert(list[1] == 20);
    END_TEST
}

// Main test runner
int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "LinkedList Test Suite" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;

    // Run all tests
    test_default_constructor();
    test_constructor_with_value();
    test_initializer_list();
    test_copy_constructor();
    test_move_constructor();
    test_push_front();
    test_push_back();
    test_pop_front();
    test_pop_back();
    test_front_back();
    test_at_method();
    test_insert();
    test_erase();
    test_clear();
    test_resize();
    test_resize_with_value();
    test_reverse();
    test_remove();
    test_find();
    test_contains();
    test_copy_assignment();
    test_move_assignment();
    test_swap();
    test_exception_empty_front();
    test_exception_empty_back();
    test_exception_empty_pop_front();
    test_exception_empty_pop_back();
    test_mixed_operations();
    test_large_dataset();
    test_with_doubles();
    test_single_element();
    test_alternating_ends();
    test_clear_and_reuse();

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