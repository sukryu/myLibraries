/**
 * @file test_heap.cpp
 * @brief Test suite for Heap class
 * @author Jinhyeok
 * @date 2025-11-29
 * @version 1.0.0
 */

#include "tree/heap.hpp"
#include <iostream>
#include <cassert>
#include <vector>
#include <string>
#include <algorithm>
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
// MaxHeap Tests
// ============================================

void test_default_constructor() {
    TEST("Default constructor (MaxHeap)")
    Heap<int> heap;
    assert(heap.empty());
    assert(heap.size() == 0);
    assert(heap.is_valid());
    END_TEST
}

void test_initializer_list() {
    TEST("Initializer list constructor")
    Heap<int> heap = {3, 1, 4, 1, 5, 9, 2, 6};
    assert(heap.size() == 8);
    assert(!heap.empty());
    assert(heap.top() == 9);  // Max element
    assert(heap.is_valid());
    END_TEST
}

void test_copy_constructor() {
    TEST("Copy constructor")
    Heap<int> heap1 = {5, 3, 7, 1, 9};
    Heap<int> heap2(heap1);
    
    assert(heap2.size() == 5);
    assert(heap2.top() == 9);
    assert(heap2.is_valid());
    
    // Verify deep copy
    heap2.pop();
    assert(heap2.size() == 4);
    assert(heap1.size() == 5);
    END_TEST
}

void test_move_constructor() {
    TEST("Move constructor")
    Heap<int> heap1 = {5, 3, 7};
    Heap<int> heap2(std::move(heap1));
    
    assert(heap2.size() == 3);
    assert(heap2.top() == 7);
    assert(heap2.is_valid());
    assert(heap1.empty());
    END_TEST
}

void test_copy_assignment() {
    TEST("Copy assignment operator")
    Heap<int> heap1 = {5, 3, 7};
    Heap<int> heap2;
    heap2 = heap1;
    
    assert(heap2.size() == 3);
    assert(heap2.top() == 7);
    assert(heap2.is_valid());
    
    heap2.pop();
    assert(heap1.size() == 3);
    END_TEST
}

void test_move_assignment() {
    TEST("Move assignment operator")
    Heap<int> heap1 = {5, 3, 7};
    Heap<int> heap2;
    heap2 = std::move(heap1);
    
    assert(heap2.size() == 3);
    assert(heap2.top() == 7);
    assert(heap1.empty());
    END_TEST
}

void test_push_basic() {
    TEST("Basic push")
    Heap<int> heap;
    
    heap.push(10);
    assert(heap.size() == 1);
    assert(heap.top() == 10);
    assert(heap.is_valid());
    
    heap.push(20);
    assert(heap.top() == 20);
    assert(heap.is_valid());
    
    heap.push(5);
    assert(heap.top() == 20);
    assert(heap.size() == 3);
    assert(heap.is_valid());
    END_TEST
}

void test_push_maintains_heap_property() {
    TEST("Push maintains heap property")
    Heap<int> heap;
    
    heap.push(5);
    heap.push(3);
    heap.push(7);
    heap.push(1);
    heap.push(9);
    heap.push(2);
    heap.push(8);
    
    assert(heap.top() == 9);
    assert(heap.is_valid());
    END_TEST
}

void test_pop_basic() {
    TEST("Basic pop")
    Heap<int> heap = {5, 3, 7, 1, 9};
    
    assert(heap.top() == 9);
    heap.pop();
    assert(heap.top() == 7);
    assert(heap.size() == 4);
    assert(heap.is_valid());
    
    heap.pop();
    assert(heap.top() == 5);
    assert(heap.is_valid());
    END_TEST
}

void test_pop_all_elements() {
    TEST("Pop all elements in order")
    Heap<int> heap = {3, 1, 4, 1, 5, 9, 2, 6};
    
    std::vector<int> result;
    while (!heap.empty()) {
        result.push_back(heap.top());
        heap.pop();
    }
    
    // Should be in descending order (max heap)
    assert(result.size() == 8);
    for (size_t i = 1; i < result.size(); ++i) {
        assert(result[i] <= result[i - 1]);
    }
    END_TEST
}

void test_extract() {
    TEST("Extract operation")
    Heap<int> heap = {5, 3, 7, 1, 9};
    
    int max = heap.extract();
    assert(max == 9);
    assert(heap.size() == 4);
    assert(heap.top() == 7);
    assert(heap.is_valid());
    
    max = heap.extract();
    assert(max == 7);
    assert(heap.size() == 3);
    END_TEST
}

void test_top_exception() {
    TEST("top() on empty heap throws exception")
    Heap<int> heap;
    
    bool exception_thrown = false;
    try {
        heap.top();
    } catch (const std::out_of_range&) {
        exception_thrown = true;
    }
    assert(exception_thrown);
    END_TEST
}

void test_pop_exception() {
    TEST("pop() on empty heap throws exception")
    Heap<int> heap;
    
    bool exception_thrown = false;
    try {
        heap.pop();
    } catch (const std::out_of_range&) {
        exception_thrown = true;
    }
    assert(exception_thrown);
    END_TEST
}

void test_extract_exception() {
    TEST("extract() on empty heap throws exception")
    Heap<int> heap;
    
    bool exception_thrown = false;
    try {
        heap.extract();
    } catch (const std::out_of_range&) {
        exception_thrown = true;
    }
    assert(exception_thrown);
    END_TEST
}

void test_clear() {
    TEST("clear()")
    Heap<int> heap = {5, 3, 7, 1, 9};
    
    heap.clear();
    assert(heap.empty());
    assert(heap.size() == 0);
    
    // Reuse after clear
    heap.push(100);
    assert(heap.size() == 1);
    assert(heap.top() == 100);
    END_TEST
}

void test_swap() {
    TEST("swap()")
    Heap<int> heap1 = {1, 2, 3};
    Heap<int> heap2 = {10, 20};
    
    heap1.swap(heap2);
    
    assert(heap1.size() == 2);
    assert(heap1.top() == 20);
    assert(heap2.size() == 3);
    assert(heap2.top() == 3);
    assert(heap1.is_valid());
    assert(heap2.is_valid());
    END_TEST
}

void test_reserve() {
    TEST("reserve()")
    Heap<int> heap;
    heap.reserve(100);
    assert(heap.capacity() >= 100);
    assert(heap.empty());
    
    for (int i = 0; i < 50; ++i) {
        heap.push(i);
    }
    assert(heap.capacity() >= 100);
    END_TEST
}

void test_heapify_efficiency() {
    TEST("Heapify from initializer list")
    // Large initializer list should use O(n) heapify
    std::vector<int> data;
    for (int i = 0; i < 1000; ++i) {
        data.push_back(i);
    }
    
    Heap<int> heap(data.begin(), data.end());
    assert(heap.size() == 1000);
    assert(heap.top() == 999);
    assert(heap.is_valid());
    END_TEST
}

void test_heap_sort() {
    TEST("Heap sort (using MaxHeap)")
    std::vector<int> data = {64, 34, 25, 12, 22, 11, 90, 5, 77, 30};
    Heap<int> heap(data.begin(), data.end());
    
    std::vector<int> sorted;
    while (!heap.empty()) {
        sorted.push_back(heap.extract());
    }
    
    // Should be sorted in descending order
    for (size_t i = 1; i < sorted.size(); ++i) {
        assert(sorted[i] <= sorted[i - 1]);
    }
    END_TEST
}

void test_duplicate_elements() {
    TEST("Duplicate elements")
    Heap<int> heap = {5, 5, 5, 3, 3, 7, 7, 7, 7};
    
    assert(heap.size() == 9);
    assert(heap.top() == 7);
    assert(heap.is_valid());
    
    heap.pop();
    assert(heap.top() == 7);  // Another 7
    END_TEST
}

void test_single_element() {
    TEST("Single element operations")
    Heap<int> heap;
    
    heap.push(42);
    assert(heap.size() == 1);
    assert(heap.top() == 42);
    assert(heap.is_valid());
    
    heap.pop();
    assert(heap.empty());
    END_TEST
}

void test_two_elements() {
    TEST("Two elements")
    Heap<int> heap;
    
    heap.push(10);
    heap.push(20);
    assert(heap.top() == 20);
    assert(heap.is_valid());
    
    heap.pop();
    assert(heap.top() == 10);
    assert(heap.is_valid());
    END_TEST
}

// ============================================
// MinHeap Tests
// ============================================

void test_min_heap_basic() {
    TEST("MinHeap basic operations")
    MinHeap<int> heap;
    
    heap.push(5);
    heap.push(3);
    heap.push(7);
    heap.push(1);
    heap.push(9);
    
    assert(heap.top() == 1);  // Minimum element
    assert(heap.is_valid());
    END_TEST
}

void test_min_heap_initializer_list() {
    TEST("MinHeap initializer list")
    MinHeap<int> heap = {3, 1, 4, 1, 5, 9, 2, 6};
    
    assert(heap.size() == 8);
    assert(heap.top() == 1);  // Minimum
    assert(heap.is_valid());
    END_TEST
}

void test_min_heap_pop_order() {
    TEST("MinHeap pop order (ascending)")
    MinHeap<int> heap = {5, 2, 8, 1, 9, 3, 7, 4, 6};
    
    std::vector<int> result;
    while (!heap.empty()) {
        result.push_back(heap.extract());
    }
    
    // Should be in ascending order
    for (size_t i = 1; i < result.size(); ++i) {
        assert(result[i] >= result[i - 1]);
    }
    END_TEST
}

void test_min_heap_sort() {
    TEST("Heap sort (using MinHeap for ascending order)")
    std::vector<int> data = {64, 34, 25, 12, 22, 11, 90, 5, 77, 30};
    MinHeap<int> heap(data.begin(), data.end());
    
    std::vector<int> sorted;
    while (!heap.empty()) {
        sorted.push_back(heap.extract());
    }
    
    // Should be sorted in ascending order
    for (size_t i = 1; i < sorted.size(); ++i) {
        assert(sorted[i] >= sorted[i - 1]);
    }
    END_TEST
}

void test_min_heap_with_duplicates() {
    TEST("MinHeap with duplicates")
    MinHeap<int> heap = {5, 1, 1, 3, 1, 7, 2};
    
    assert(heap.top() == 1);
    heap.pop();
    assert(heap.top() == 1);  // Another 1
    heap.pop();
    assert(heap.top() == 1);  // Another 1
    heap.pop();
    assert(heap.top() == 2);
    assert(heap.is_valid());
    END_TEST
}

// ============================================
// MaxHeap Type Alias Tests
// ============================================

void test_max_heap_alias() {
    TEST("MaxHeap type alias")
    MaxHeap<int> heap = {3, 1, 4, 1, 5, 9};
    
    assert(heap.top() == 9);
    assert(heap.is_valid());
    
    heap.pop();
    assert(heap.top() == 5);
    END_TEST
}

// ============================================
// Edge Cases and Stress Tests
// ============================================

void test_large_dataset() {
    TEST("Large dataset (stress test)")
    Heap<int> heap;
    const int count = 10000;
    
    // Insert elements
    for (int i = 0; i < count; ++i) {
        heap.push(i);
        assert(heap.is_valid());
    }
    
    assert(heap.size() == count);
    assert(heap.top() == count - 1);
    
    // Extract all
    int prev = heap.extract();
    while (!heap.empty()) {
        int curr = heap.extract();
        assert(curr <= prev);
        prev = curr;
    }
    END_TEST
}

void test_random_operations() {
    TEST("Random operations")
    Heap<int> heap;
    std::mt19937 rng(42);  // Fixed seed for reproducibility
    std::uniform_int_distribution<int> dist(0, 1000);
    
    // Random pushes
    for (int i = 0; i < 500; ++i) {
        heap.push(dist(rng));
        assert(heap.is_valid());
    }
    
    // Random pops
    for (int i = 0; i < 250; ++i) {
        heap.pop();
        assert(heap.is_valid());
    }
    
    // More random pushes
    for (int i = 0; i < 250; ++i) {
        heap.push(dist(rng));
        assert(heap.is_valid());
    }
    
    assert(heap.size() == 500);
    END_TEST
}

void test_ascending_insertion() {
    TEST("Ascending order insertion")
    Heap<int> heap;
    
    for (int i = 1; i <= 100; ++i) {
        heap.push(i);
    }
    
    assert(heap.top() == 100);
    assert(heap.is_valid());
    END_TEST
}

void test_descending_insertion() {
    TEST("Descending order insertion")
    Heap<int> heap;
    
    for (int i = 100; i >= 1; --i) {
        heap.push(i);
    }
    
    assert(heap.top() == 100);
    assert(heap.is_valid());
    END_TEST
}

void test_with_doubles() {
    TEST("Heap<double>")
    Heap<double> heap = {3.14, 1.41, 2.71, 1.73, 0.57};
    
    assert(heap.size() == 5);
    assert(heap.top() > 3.13 && heap.top() < 3.15);
    assert(heap.is_valid());
    END_TEST
}

void test_min_heap_doubles() {
    TEST("MinHeap<double>")
    MinHeap<double> heap = {3.14, 1.41, 2.71, 1.73, 0.57};
    
    assert(heap.top() > 0.56 && heap.top() < 0.58);
    assert(heap.is_valid());
    END_TEST
}

void test_priority_queue_simulation() {
    TEST("Priority queue simulation")
    // Simulate task scheduling with priorities
    MaxHeap<int> pq;
    
    // Add tasks with priorities
    pq.push(3);   // Low priority
    pq.push(10);  // High priority
    pq.push(5);   // Medium priority
    pq.push(1);   // Lowest priority
    pq.push(8);   // Medium-high priority
    
    // Process in priority order
    assert(pq.extract() == 10);
    assert(pq.extract() == 8);
    assert(pq.extract() == 5);
    assert(pq.extract() == 3);
    assert(pq.extract() == 1);
    assert(pq.empty());
    END_TEST
}

void test_kth_largest_element() {
    TEST("Find kth largest element")
    std::vector<int> data = {3, 2, 1, 5, 6, 4};
    int k = 2;
    
    // Use min heap of size k to find kth largest
    MinHeap<int> heap;
    for (int val : data) {
        heap.push(val);
        if (heap.size() > static_cast<size_t>(k)) {
            heap.pop();
        }
    }
    
    // Top of min heap is kth largest
    assert(heap.top() == 5);  // 2nd largest in {3,2,1,5,6,4} is 5
    END_TEST
}

void test_merge_sorted_streams() {
    TEST("Merge sorted streams simulation")
    // Simulate merging k sorted lists
    MinHeap<int> heap;
    
    // Add first elements from 3 sorted streams
    heap.push(1);   // Stream 1: [1, 4, 7]
    heap.push(2);   // Stream 2: [2, 5, 8]
    heap.push(3);   // Stream 3: [3, 6, 9]
    
    std::vector<int> merged;
    merged.push_back(heap.extract());  // 1
    
    heap.push(4);  // Next from stream 1
    merged.push_back(heap.extract());  // 2
    
    heap.push(5);  // Next from stream 2
    merged.push_back(heap.extract());  // 3
    
    // Verify order
    assert(merged[0] == 1);
    assert(merged[1] == 2);
    assert(merged[2] == 3);
    END_TEST
}

void test_clear_and_reuse() {
    TEST("Clear and reuse heap")
    Heap<int> heap = {5, 3, 7, 1, 9};
    
    heap.clear();
    assert(heap.empty());
    
    heap.push(100);
    heap.push(200);
    heap.push(50);
    
    assert(heap.size() == 3);
    assert(heap.top() == 200);
    assert(heap.is_valid());
    END_TEST
}

void test_push_pop_interleaved() {
    TEST("Interleaved push and pop")
    Heap<int> heap;
    
    heap.push(5);
    heap.push(10);
    heap.pop();
    heap.push(3);
    heap.push(15);
    heap.pop();
    heap.push(7);
    
    assert(heap.size() == 3);
    assert(heap.top() == 7);
    assert(heap.is_valid());
    END_TEST
}

// Main test runner
int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "Heap Test Suite" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;

    // Constructor tests
    test_default_constructor();
    test_initializer_list();
    test_copy_constructor();
    test_move_constructor();
    test_copy_assignment();
    test_move_assignment();

    // Basic operations
    test_push_basic();
    test_push_maintains_heap_property();
    test_pop_basic();
    test_pop_all_elements();
    test_extract();

    // Exception tests
    test_top_exception();
    test_pop_exception();
    test_extract_exception();

    // Utility operations
    test_clear();
    test_swap();
    test_reserve();

    // Heap algorithms
    test_heapify_efficiency();
    test_heap_sort();
    test_duplicate_elements();

    // Edge cases
    test_single_element();
    test_two_elements();

    // MinHeap tests
    std::cout << std::endl;
    std::cout << "--- MinHeap Tests ---" << std::endl;
    test_min_heap_basic();
    test_min_heap_initializer_list();
    test_min_heap_pop_order();
    test_min_heap_sort();
    test_min_heap_with_duplicates();

    // Type alias tests
    std::cout << std::endl;
    std::cout << "--- Type Alias Tests ---" << std::endl;
    test_max_heap_alias();

    // Stress tests
    std::cout << std::endl;
    std::cout << "--- Stress Tests ---" << std::endl;
    test_large_dataset();
    test_random_operations();
    test_ascending_insertion();
    test_descending_insertion();

    // Type tests
    std::cout << std::endl;
    std::cout << "--- Type Tests ---" << std::endl;
    test_with_doubles();
    test_min_heap_doubles();

    // Practical use cases
    std::cout << std::endl;
    std::cout << "--- Practical Use Cases ---" << std::endl;
    test_priority_queue_simulation();
    test_kth_largest_element();
    test_merge_sorted_streams();

    // Additional tests
    std::cout << std::endl;
    std::cout << "--- Additional Tests ---" << std::endl;
    test_clear_and_reuse();
    test_push_pop_interleaved();

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