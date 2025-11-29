/**
 * @file test_queue.cpp
 * @brief Test suite for Queue class
 * @author Jinhyeok
 * @date 2025-11-29
 * @version 1.0.0
 */

#include "linear/queue.hpp"
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
    Queue<int> queue;
    assert(queue.empty());
    assert(queue.size() == 0);
    END_TEST
}

void test_copy_constructor() {
    TEST("Copy constructor")
    Queue<int> queue1;
    queue1.push(1);
    queue1.push(2);
    queue1.push(3);
    
    Queue<int> queue2(queue1);
    assert(queue2.size() == 3);
    assert(queue2.front() == 1);
    
    // Verify deep copy
    queue2.pop();
    assert(queue2.size() == 2);
    assert(queue1.size() == 3);
    END_TEST
}

void test_move_constructor() {
    TEST("Move constructor")
    Queue<int> queue1;
    queue1.push(1);
    queue1.push(2);
    queue1.push(3);
    
    Queue<int> queue2(std::move(queue1));
    assert(queue2.size() == 3);
    assert(queue2.front() == 1);
    assert(queue1.empty());
    END_TEST
}

void test_push_basic() {
    TEST("Basic push operation")
    Queue<int> queue;
    queue.push(10);
    assert(queue.size() == 1);
    assert(!queue.empty());
    assert(queue.front() == 10);
    assert(queue.back() == 10);
    END_TEST
}

void test_push_multiple() {
    TEST("Multiple push operations")
    Queue<int> queue;
    queue.push(1);
    queue.push(2);
    queue.push(3);
    assert(queue.size() == 3);
    assert(queue.front() == 1);
    assert(queue.back() == 3);
    END_TEST
}

void test_pop_basic() {
    TEST("Basic pop operation")
    Queue<int> queue;
    queue.push(10);
    queue.push(20);
    queue.pop();
    assert(queue.size() == 1);
    assert(queue.front() == 20);
    END_TEST
}

void test_fifo_order() {
    TEST("FIFO (First-In-First-Out) order")
    Queue<int> queue;
    queue.push(1);
    queue.push(2);
    queue.push(3);
    queue.push(4);
    queue.push(5);
    
    assert(queue.front() == 1); queue.pop();
    assert(queue.front() == 2); queue.pop();
    assert(queue.front() == 3); queue.pop();
    assert(queue.front() == 4); queue.pop();
    assert(queue.front() == 5); queue.pop();
    assert(queue.empty());
    END_TEST
}

void test_front_back_access() {
    TEST("Front and back element access")
    Queue<int> queue;
    queue.push(100);
    assert(queue.front() == 100);
    assert(queue.back() == 100);
    
    queue.push(200);
    assert(queue.front() == 100);
    assert(queue.back() == 200);
    
    queue.push(300);
    assert(queue.front() == 100);
    assert(queue.back() == 300);
    END_TEST
}

void test_front_back_modification() {
    TEST("Modify front and back elements")
    Queue<int> queue;
    queue.push(1);
    queue.push(2);
    queue.push(3);
    
    queue.front() = 100;
    queue.back() = 300;
    
    assert(queue.front() == 100);
    assert(queue.back() == 300);
    END_TEST
}

void test_empty_on_empty_queue() {
    TEST("Empty check on empty queue")
    Queue<int> queue;
    assert(queue.empty());
    queue.push(1);
    assert(!queue.empty());
    queue.pop();
    assert(queue.empty());
    END_TEST
}

void test_size_tracking() {
    TEST("Size tracking")
    Queue<int> queue;
    assert(queue.size() == 0);
    
    for (int i = 0; i < 10; ++i) {
        queue.push(i);
        assert(queue.size() == i + 1);
    }
    
    for (int i = 9; i >= 0; --i) {
        assert(queue.size() == i + 1);
        queue.pop();
    }
    
    assert(queue.size() == 0);
    END_TEST
}

void test_clear() {
    TEST("Clear operation")
    Queue<int> queue;
    queue.push(1);
    queue.push(2);
    queue.push(3);
    queue.clear();
    assert(queue.empty());
    assert(queue.size() == 0);
    END_TEST
}

void test_copy_assignment() {
    TEST("Copy assignment operator")
    Queue<int> queue1;
    queue1.push(1);
    queue1.push(2);
    
    Queue<int> queue2;
    queue2 = queue1;
    assert(queue2.size() == 2);
    assert(queue2.front() == 1);
    
    queue2.push(3);
    assert(queue1.size() == 2);
    assert(queue2.size() == 3);
    END_TEST
}

void test_move_assignment() {
    TEST("Move assignment operator")
    Queue<int> queue1;
    queue1.push(1);
    queue1.push(2);
    
    Queue<int> queue2;
    queue2 = std::move(queue1);
    assert(queue2.size() == 2);
    assert(queue1.empty());
    END_TEST
}

void test_swap() {
    TEST("Swap operation")
    Queue<int> queue1;
    queue1.push(1);
    queue1.push(2);
    
    Queue<int> queue2;
    queue2.push(10);
    queue2.push(20);
    queue2.push(30);
    
    queue1.swap(queue2);
    assert(queue1.size() == 3);
    assert(queue1.front() == 10);
    assert(queue2.size() == 2);
    assert(queue2.front() == 1);
    END_TEST
}

void test_exception_empty_front() {
    TEST("Exception on empty queue front()")
    Queue<int> queue;
    bool exception_thrown = false;
    try {
        queue.front();
    } catch (const std::out_of_range&) {
        exception_thrown = true;
    }
    assert(exception_thrown);
    END_TEST
}

void test_exception_empty_back() {
    TEST("Exception on empty queue back()")
    Queue<int> queue;
    bool exception_thrown = false;
    try {
        queue.back();
    } catch (const std::out_of_range&) {
        exception_thrown = true;
    }
    assert(exception_thrown);
    END_TEST
}

void test_exception_empty_pop() {
    TEST("Exception on empty queue pop()")
    Queue<int> queue;
    bool exception_thrown = false;
    try {
        queue.pop();
    } catch (const std::out_of_range&) {
        exception_thrown = true;
    }
    assert(exception_thrown);
    END_TEST
}

void test_push_pop_sequence() {
    TEST("Push-pop sequence")
    Queue<int> queue;
    
    // Interleaved push and pop
    queue.push(1);
    queue.push(2);
    queue.pop();
    queue.push(3);
    queue.push(4);
    queue.pop();
    
    assert(queue.size() == 2);
    assert(queue.front() == 3);
    assert(queue.back() == 4);
    END_TEST
}

void test_large_dataset() {
    TEST("Large dataset (stress test)")
    Queue<int> queue;
    const int count = 10000;
    
    // Push many elements
    for (int i = 0; i < count; ++i) {
        queue.push(i);
    }
    assert(queue.size() == count);
    assert(queue.front() == 0);
    assert(queue.back() == count - 1);
    
    // Pop all elements
    for (int i = 0; i < count; ++i) {
        assert(queue.front() == i);
        queue.pop();
    }
    assert(queue.empty());
    END_TEST
}

void test_with_doubles() {
    TEST("Queue<double>")
    Queue<double> queue;
    queue.push(1.1);
    queue.push(2.2);
    queue.push(3.3);
    
    assert(queue.size() == 3);
    assert(queue.front() > 1.0 && queue.front() < 1.2);
    queue.pop();
    assert(queue.front() > 2.1 && queue.front() < 2.3);
    END_TEST
}

void test_practical_use_case() {
    TEST("Practical use case: Task queue simulation")
    Queue<int> task_queue;
    
    // Add tasks
    for (int i = 1; i <= 5; ++i) {
        task_queue.push(i * 100);
    }
    
    // Process tasks in order
    int expected_task = 100;
    while (!task_queue.empty()) {
        assert(task_queue.front() == expected_task);
        task_queue.pop();
        expected_task += 100;
    }
    END_TEST
}

void test_clear_and_reuse() {
    TEST("Clear and reuse queue")
    Queue<int> queue;
    
    // First use
    queue.push(1);
    queue.push(2);
    queue.push(3);
    queue.clear();
    
    // Reuse after clear
    queue.push(10);
    queue.push(20);
    assert(queue.size() == 2);
    assert(queue.front() == 10);
    assert(queue.back() == 20);
    END_TEST
}

void test_single_element() {
    TEST("Single element operations")
    Queue<int> queue;
    queue.push(42);
    assert(queue.size() == 1);
    assert(queue.front() == 42);
    assert(queue.back() == 42);
    queue.pop();
    assert(queue.empty());
    END_TEST
}

void test_continuous_operations() {
    TEST("Continuous push/pop operations")
    Queue<int> queue;
    
    // Keep queue size around 3-5
    for (int i = 0; i < 100; ++i) {
        queue.push(i);
        if (queue.size() > 3) {
            queue.pop();
        }
    }
    
    assert(queue.size() > 0);
    assert(!queue.empty());
    END_TEST
}

void test_alternating_operations() {
    TEST("Alternating push and pop")
    Queue<int> queue;
    
    for (int i = 0; i < 50; ++i) {
        queue.push(i);
        queue.push(i + 1);
        queue.pop();
    }
    
    assert(queue.size() == 50);
    END_TEST
}

// Main test runner
int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "Queue Test Suite" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;

    // Run all tests
    test_default_constructor();
    test_copy_constructor();
    test_move_constructor();
    test_push_basic();
    test_push_multiple();
    test_pop_basic();
    test_fifo_order();
    test_front_back_access();
    test_front_back_modification();
    test_empty_on_empty_queue();
    test_size_tracking();
    test_clear();
    test_copy_assignment();
    test_move_assignment();
    test_swap();
    test_exception_empty_front();
    test_exception_empty_back();
    test_exception_empty_pop();
    test_push_pop_sequence();
    test_large_dataset();
    test_with_doubles();
    test_practical_use_case();
    test_clear_and_reuse();
    test_single_element();
    test_continuous_operations();
    test_alternating_operations();

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