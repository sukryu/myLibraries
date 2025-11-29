/**
 * @file test_stack.cpp
 * @brief Test suite for Stack class
 * @author Jinhyeok
 * @date 2025-11-16
 * @version 1.0.0
 */

#include "linear/stack.hpp"
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
    Stack<int> stack;
    assert(stack.empty());
    assert(stack.size() == 0);
    END_TEST
}

void test_constructor_with_capacity() {
    TEST("Constructor with capacity")
    Stack<int> stack(10);
    assert(stack.empty());
    assert(stack.capacity() >= 10);
    END_TEST
}

void test_push_basic() {
    TEST("Basic push operation")
    Stack<int> stack;
    stack.push(10);
    assert(stack.size() == 1);
    assert(!stack.empty());
    assert(stack.top() == 10);
    END_TEST
}

void test_push_multiple() {
    TEST("Multiple push operations")
    Stack<int> stack;
    stack.push(1);
    stack.push(2);
    stack.push(3);
    assert(stack.size() == 3);
    assert(stack.top() == 3);
    END_TEST
}

void test_pop_basic() {
    TEST("Basic pop operation")
    Stack<int> stack;
    stack.push(10);
    stack.push(20);
    stack.pop();
    assert(stack.size() == 1);
    assert(stack.top() == 10);
    END_TEST
}

void test_lifo_order() {
    TEST("LIFO (Last-In-First-Out) order")
    Stack<int> stack;
    stack.push(1);
    stack.push(2);
    stack.push(3);
    stack.push(4);
    stack.push(5);
    
    assert(stack.top() == 5); stack.pop();
    assert(stack.top() == 4); stack.pop();
    assert(stack.top() == 3); stack.pop();
    assert(stack.top() == 2); stack.pop();
    assert(stack.top() == 1); stack.pop();
    assert(stack.empty());
    END_TEST
}

void test_top_access() {
    TEST("Top element access")
    Stack<int> stack;
    stack.push(100);
    assert(stack.top() == 100);
    stack.top() = 200;  // Modify top element
    assert(stack.top() == 200);
    END_TEST
}

void test_empty_on_empty_stack() {
    TEST("Empty check on empty stack")
    Stack<int> stack;
    assert(stack.empty());
    stack.push(1);
    assert(!stack.empty());
    stack.pop();
    assert(stack.empty());
    END_TEST
}

void test_size_tracking() {
    TEST("Size tracking")
    Stack<int> stack;
    assert(stack.size() == 0);
    
    for (int i = 0; i < 10; ++i) {
        stack.push(i);
        assert(stack.size() == i + 1);
    }
    
    for (int i = 9; i >= 0; --i) {
        assert(stack.size() == i + 1);
        stack.pop();
    }
    
    assert(stack.size() == 0);
    END_TEST
}

void test_clear() {
    TEST("Clear operation")
    Stack<int> stack;
    stack.push(1);
    stack.push(2);
    stack.push(3);
    stack.clear();
    assert(stack.empty());
    assert(stack.size() == 0);
    END_TEST
}

void test_copy_constructor() {
    TEST("Copy constructor")
    Stack<int> stack1;
    stack1.push(1);
    stack1.push(2);
    stack1.push(3);
    
    Stack<int> stack2(stack1);
    assert(stack2.size() == 3);
    assert(stack2.top() == 3);
    
    // Verify deep copy
    stack2.pop();
    assert(stack2.size() == 2);
    assert(stack1.size() == 3);
    END_TEST
}

void test_move_constructor() {
    TEST("Move constructor")
    Stack<int> stack1;
    stack1.push(1);
    stack1.push(2);
    stack1.push(3);
    
    Stack<int> stack2(std::move(stack1));
    assert(stack2.size() == 3);
    assert(stack2.top() == 3);
    assert(stack1.empty());
    END_TEST
}

void test_copy_assignment() {
    TEST("Copy assignment operator")
    Stack<int> stack1;
    stack1.push(1);
    stack1.push(2);
    
    Stack<int> stack2;
    stack2 = stack1;
    assert(stack2.size() == 2);
    assert(stack2.top() == 2);
    
    stack2.push(3);
    assert(stack1.size() == 2);
    assert(stack2.size() == 3);
    END_TEST
}

void test_move_assignment() {
    TEST("Move assignment operator")
    Stack<int> stack1;
    stack1.push(1);
    stack1.push(2);
    
    Stack<int> stack2;
    stack2 = std::move(stack1);
    assert(stack2.size() == 2);
    assert(stack1.empty());
    END_TEST
}

void test_swap() {
    TEST("Swap operation")
    Stack<int> stack1;
    stack1.push(1);
    stack1.push(2);
    
    Stack<int> stack2;
    stack2.push(10);
    stack2.push(20);
    stack2.push(30);
    
    stack1.swap(stack2);
    assert(stack1.size() == 3);
    assert(stack1.top() == 30);
    assert(stack2.size() == 2);
    assert(stack2.top() == 2);
    END_TEST
}

void test_reserve() {
    TEST("Reserve capacity")
    Stack<int> stack;
    stack.reserve(100);
    assert(stack.capacity() >= 100);
    assert(stack.empty());
    
    for (int i = 0; i < 50; ++i) {
        stack.push(i);
    }
    assert(stack.capacity() >= 100);
    END_TEST
}

void test_exception_empty_top() {
    TEST("Exception on empty stack top()")
    Stack<int> stack;
    bool exception_thrown = false;
    try {
        stack.top();
    } catch (const std::out_of_range&) {
        exception_thrown = true;
    }
    assert(exception_thrown);
    END_TEST
}

void test_exception_empty_pop() {
    TEST("Exception on empty stack pop()")
    Stack<int> stack;
    bool exception_thrown = false;
    try {
        stack.pop();
    } catch (const std::out_of_range&) {
        exception_thrown = true;
    }
    assert(exception_thrown);
    END_TEST
}

void test_push_pop_sequence() {
    TEST("Push-pop sequence")
    Stack<int> stack;
    
    // Interleaved push and pop
    stack.push(1);
    stack.push(2);
    stack.pop();
    stack.push(3);
    stack.push(4);
    stack.pop();
    stack.pop();
    
    assert(stack.size() == 1);
    assert(stack.top() == 1);
    END_TEST
}

void test_large_dataset() {
    TEST("Large dataset (stress test)")
    Stack<int> stack;
    const int count = 10000;
    
    // Push many elements
    for (int i = 0; i < count; ++i) {
        stack.push(i);
    }
    assert(stack.size() == count);
    assert(stack.top() == count - 1);
    
    // Pop all elements
    for (int i = count - 1; i >= 0; --i) {
        assert(stack.top() == i);
        stack.pop();
    }
    assert(stack.empty());
    END_TEST
}

void test_with_doubles() {
    TEST("Stack<double>")
    Stack<double> stack;
    stack.push(1.1);
    stack.push(2.2);
    stack.push(3.3);
    
    assert(stack.size() == 3);
    assert(stack.top() > 3.2 && stack.top() < 3.4);
    stack.pop();
    assert(stack.top() > 2.1 && stack.top() < 2.3);
    END_TEST
}

void test_practical_use_case() {
    TEST("Practical use case: expression evaluation")
    Stack<char> stack;
    
    // Simulate parentheses matching
    std::string expr = "(({}))";
    for (char c : expr) {
        if (c == '(' || c == '{' || c == '[') {
            stack.push(c);
        } else {
            if (!stack.empty()) {
                stack.pop();
            }
        }
    }
    
    assert(stack.empty());  // Balanced expression
    END_TEST
}

void test_clear_and_reuse() {
    TEST("Clear and reuse stack")
    Stack<int> stack;
    
    // First use
    stack.push(1);
    stack.push(2);
    stack.push(3);
    stack.clear();
    
    // Reuse after clear
    stack.push(10);
    stack.push(20);
    assert(stack.size() == 2);
    assert(stack.top() == 20);
    END_TEST
}

// Main test runner
int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "Stack Test Suite" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;

    // Run all tests
    test_default_constructor();
    test_constructor_with_capacity();
    test_push_basic();
    test_push_multiple();
    test_pop_basic();
    test_lifo_order();
    test_top_access();
    test_empty_on_empty_stack();
    test_size_tracking();
    test_clear();
    test_copy_constructor();
    test_move_constructor();
    test_copy_assignment();
    test_move_assignment();
    test_swap();
    test_reserve();
    test_exception_empty_top();
    test_exception_empty_pop();
    test_push_pop_sequence();
    test_large_dataset();
    test_with_doubles();
    test_practical_use_case();
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
