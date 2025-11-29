/**
 * @file test_deque.cpp
 * @brief Test suite for Deque class
 * @author Jinhyeok
 * @date 2025-11-29
 * @version 1.0.0
 */

#include "linear/deque.hpp"
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
    Deque<int> deque;
    assert(deque.empty());
    assert(deque.size() == 0);
    END_TEST
}

void test_constructor_with_value() {
    TEST("Constructor with count and value")
    Deque<int> deque(5, 42);
    assert(deque.size() == 5);
    for (size_t i = 0; i < deque.size(); ++i) {
        assert(deque[i] == 42);
    }
    END_TEST
}

void test_initializer_list() {
    TEST("Initializer list constructor")
    Deque<int> deque = {1, 2, 3, 4, 5};
    assert(deque.size() == 5);
    assert(deque[0] == 1);
    assert(deque[4] == 5);
    END_TEST
}

void test_copy_constructor() {
    TEST("Copy constructor")
    Deque<int> deque1 = {1, 2, 3};
    Deque<int> deque2(deque1);
    assert(deque2.size() == 3);
    assert(deque2[1] == 2);
    
    deque2[0] = 99;
    assert(deque1[0] == 1);  // Verify deep copy
    END_TEST
}

void test_move_constructor() {
    TEST("Move constructor")
    Deque<int> deque1 = {1, 2, 3};
    Deque<int> deque2(std::move(deque1));
    assert(deque2.size() == 3);
    assert(deque2[0] == 1);
    assert(deque1.empty());
    END_TEST
}

void test_push_front() {
    TEST("push_front")
    Deque<int> deque;
    deque.push_front(3);
    deque.push_front(2);
    deque.push_front(1);
    assert(deque.size() == 3);
    assert(deque[0] == 1);
    assert(deque[1] == 2);
    assert(deque[2] == 3);
    END_TEST
}

void test_push_back() {
    TEST("push_back")
    Deque<int> deque;
    deque.push_back(1);
    deque.push_back(2);
    deque.push_back(3);
    assert(deque.size() == 3);
    assert(deque[0] == 1);
    assert(deque[2] == 3);
    END_TEST
}

void test_pop_front() {
    TEST("pop_front")
    Deque<int> deque = {1, 2, 3, 4, 5};
    deque.pop_front();
    assert(deque.size() == 4);
    assert(deque.front() == 2);
    END_TEST
}

void test_pop_back() {
    TEST("pop_back")
    Deque<int> deque = {1, 2, 3, 4, 5};
    deque.pop_back();
    assert(deque.size() == 4);
    assert(deque.back() == 4);
    END_TEST
}

void test_both_ends_operations() {
    TEST("Operations on both ends")
    Deque<int> deque;
    deque.push_back(3);
    deque.push_front(2);
    deque.push_back(4);
    deque.push_front(1);
    deque.push_back(5);
    
    assert(deque.size() == 5);
    assert(deque[0] == 1);
    assert(deque[1] == 2);
    assert(deque[2] == 3);
    assert(deque[3] == 4);
    assert(deque[4] == 5);
    END_TEST
}

void test_front_back_access() {
    TEST("front() and back() access")
    Deque<int> deque = {1, 2, 3, 4, 5};
    assert(deque.front() == 1);
    assert(deque.back() == 5);
    
    deque.front() = 100;
    deque.back() = 500;
    assert(deque.front() == 100);
    assert(deque.back() == 500);
    END_TEST
}

void test_at_method() {
    TEST("at() method with bounds checking")
    Deque<int> deque = {10, 20, 30};
    assert(deque.at(0) == 10);
    assert(deque.at(2) == 30);
    
    bool exception_thrown = false;
    try {
        deque.at(10);
    } catch (const std::out_of_range&) {
        exception_thrown = true;
    }
    assert(exception_thrown);
    END_TEST
}

void test_operator_brackets() {
    TEST("operator[] access")
    Deque<int> deque = {1, 2, 3, 4, 5};
    assert(deque[0] == 1);
    assert(deque[2] == 3);
    assert(deque[4] == 5);
    
    deque[2] = 99;
    assert(deque[2] == 99);
    END_TEST
}

void test_insert() {
    TEST("insert() at various positions")
    Deque<int> deque = {1, 2, 4, 5};
    deque.insert(2, 3);
    assert(deque.size() == 5);
    assert(deque[2] == 3);
    
    deque.insert(0, 0);
    assert(deque[0] == 0);
    
    deque.insert(deque.size(), 6);
    assert(deque.back() == 6);
    END_TEST
}

void test_erase() {
    TEST("erase() at various positions")
    Deque<int> deque = {1, 2, 3, 4, 5};
    deque.erase(2);
    assert(deque.size() == 4);
    assert(deque[2] == 4);
    
    deque.erase(0);
    assert(deque.front() == 2);
    END_TEST
}

void test_clear() {
    TEST("clear()")
    Deque<int> deque = {1, 2, 3, 4, 5};
    deque.clear();
    assert(deque.empty());
    assert(deque.size() == 0);
    END_TEST
}

void test_resize() {
    TEST("resize()")
    Deque<int> deque = {1, 2, 3};
    deque.resize(5);
    assert(deque.size() == 5);
    
    deque.resize(2);
    assert(deque.size() == 2);
    assert(deque[0] == 1);
    assert(deque[1] == 2);
    END_TEST
}

void test_resize_with_value() {
    TEST("resize() with default value")
    Deque<int> deque = {1, 2, 3};
    deque.resize(6, 99);
    assert(deque.size() == 6);
    assert(deque[3] == 99);
    assert(deque[5] == 99);
    END_TEST
}

void test_reverse() {
    TEST("reverse()")
    Deque<int> deque = {1, 2, 3, 4, 5};
    deque.reverse();
    assert(deque[0] == 5);
    assert(deque[4] == 1);
    
    deque.reverse();
    assert(deque[0] == 1);
    assert(deque[4] == 5);
    END_TEST
}

void test_remove() {
    TEST("remove() specific value")
    Deque<int> deque = {1, 2, 3, 2, 4, 2, 5};
    size_t removed = deque.remove(2);
    assert(removed == 3);
    assert(deque.size() == 4);
    assert(!deque.contains(2));
    END_TEST
}

void test_find() {
    TEST("find() element")
    Deque<int> deque = {10, 20, 30, 40, 50};
    assert(deque.find(30) == 2);
    assert(deque.find(10) == 0);
    assert(deque.find(99) == deque.size());
    END_TEST
}

void test_contains() {
    TEST("contains() element")
    Deque<int> deque = {1, 2, 3, 4, 5};
    assert(deque.contains(3));
    assert(deque.contains(1));
    assert(!deque.contains(10));
    END_TEST
}

void test_copy_assignment() {
    TEST("Copy assignment operator")
    Deque<int> deque1 = {1, 2, 3};
    Deque<int> deque2;
    deque2 = deque1;
    assert(deque2.size() == 3);
    
    deque2[0] = 99;
    assert(deque1[0] == 1);
    END_TEST
}

void test_move_assignment() {
    TEST("Move assignment operator")
    Deque<int> deque1 = {1, 2, 3};
    Deque<int> deque2;
    deque2 = std::move(deque1);
    assert(deque2.size() == 3);
    assert(deque1.empty());
    END_TEST
}

void test_swap() {
    TEST("swap()")
    Deque<int> deque1 = {1, 2, 3};
    Deque<int> deque2 = {10, 20};
    deque1.swap(deque2);
    assert(deque1.size() == 2);
    assert(deque1[0] == 10);
    assert(deque2.size() == 3);
    assert(deque2[0] == 1);
    END_TEST
}

void test_use_as_stack() {
    TEST("Use as Stack (LIFO)")
    Deque<int> deque;
    deque.push_back(1);
    deque.push_back(2);
    deque.push_back(3);
    
    assert(deque.back() == 3); deque.pop_back();
    assert(deque.back() == 2); deque.pop_back();
    assert(deque.back() == 1); deque.pop_back();
    assert(deque.empty());
    END_TEST
}

void test_use_as_queue() {
    TEST("Use as Queue (FIFO)")
    Deque<int> deque;
    deque.push_back(1);
    deque.push_back(2);
    deque.push_back(3);
    
    assert(deque.front() == 1); deque.pop_front();
    assert(deque.front() == 2); deque.pop_front();
    assert(deque.front() == 3); deque.pop_front();
    assert(deque.empty());
    END_TEST
}

void test_exception_empty_front() {
    TEST("Exception on empty deque front()")
    Deque<int> deque;
    bool exception_thrown = false;
    try {
        deque.front();
    } catch (const std::out_of_range&) {
        exception_thrown = true;
    }
    assert(exception_thrown);
    END_TEST
}

void test_exception_empty_back() {
    TEST("Exception on empty deque back()")
    Deque<int> deque;
    bool exception_thrown = false;
    try {
        deque.back();
    } catch (const std::out_of_range&) {
        exception_thrown = true;
    }
    assert(exception_thrown);
    END_TEST
}

void test_exception_empty_pop_front() {
    TEST("Exception on empty deque pop_front()")
    Deque<int> deque;
    bool exception_thrown = false;
    try {
        deque.pop_front();
    } catch (const std::out_of_range&) {
        exception_thrown = true;
    }
    assert(exception_thrown);
    END_TEST
}

void test_exception_empty_pop_back() {
    TEST("Exception on empty deque pop_back()")
    Deque<int> deque;
    bool exception_thrown = false;
    try {
        deque.pop_back();
    } catch (const std::out_of_range&) {
        exception_thrown = true;
    }
    assert(exception_thrown);
    END_TEST
}

void test_mixed_operations() {
    TEST("Mixed push/pop operations")
    Deque<int> deque;
    deque.push_back(2);
    deque.push_front(1);
    deque.push_back(3);
    deque.pop_front();
    deque.push_front(0);
    deque.pop_back();
    
    assert(deque.size() == 2);
    assert(deque[0] == 0);
    assert(deque[1] == 2);
    END_TEST
}

void test_large_dataset() {
    TEST("Large dataset (stress test)")
    Deque<int> deque;
    const int count = 1000;
    
    for (int i = 0; i < count; ++i) {
        if (i % 2 == 0) {
            deque.push_back(i);
        } else {
            deque.push_front(i);
        }
    }
    assert(deque.size() == count);
    
    for (int i = 0; i < count / 2; ++i) {
        deque.pop_front();
        deque.pop_back();
    }
    assert(deque.empty());
    END_TEST
}

void test_with_doubles() {
    TEST("Deque<double>")
    Deque<double> deque = {1.1, 2.2, 3.3};
    assert(deque.size() == 3);
    deque.push_front(0.0);
    deque.push_back(4.4);
    assert(deque.size() == 5);
    END_TEST
}

void test_sliding_window_simulation() {
    TEST("Sliding window simulation")
    Deque<int> window;
    const int window_size = 3;
    
    // Simulate sliding window
    for (int i = 1; i <= 10; ++i) {
        window.push_back(i);
        if (window.size() > window_size) {
            window.pop_front();
        }
        assert(window.size() <= window_size);
    }
    
    assert(window.size() == window_size);
    assert(window.front() == 8);
    assert(window.back() == 10);
    END_TEST
}

void test_clear_and_reuse() {
    TEST("Clear and reuse deque")
    Deque<int> deque = {1, 2, 3, 4, 5};
    deque.clear();
    assert(deque.empty());
    
    deque.push_back(10);
    deque.push_front(5);
    assert(deque.size() == 2);
    assert(deque[0] == 5);
    assert(deque[1] == 10);
    END_TEST
}

// Main test runner
int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "Deque Test Suite" << std::endl;
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
    test_both_ends_operations();
    test_front_back_access();
    test_at_method();
    test_operator_brackets();
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
    test_use_as_stack();
    test_use_as_queue();
    test_exception_empty_front();
    test_exception_empty_back();
    test_exception_empty_pop_front();
    test_exception_empty_pop_back();
    test_mixed_operations();
    test_large_dataset();
    test_with_doubles();
    test_sliding_window_simulation();
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