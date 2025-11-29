/**
 * @file test_sorting.cpp
 * @brief Test suite for sorting algorithms
 * @author Jinhyeok
 * @date 2025-11-29
 * @version 1.0.0
 */

#include "algorithm/sorting.hpp"
#include <iostream>
#include <cassert>
#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include <numeric>
#include <cmath>

using namespace mylib::algorithm;

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

// Helper function to check if vector is sorted
template <typename T>
bool is_sorted_asc(const std::vector<T>& v) {
    for (size_t i = 1; i < v.size(); ++i) {
        if (v[i] < v[i-1]) return false;
    }
    return true;
}

template <typename T>
bool is_sorted_desc(const std::vector<T>& v) {
    for (size_t i = 1; i < v.size(); ++i) {
        if (v[i] > v[i-1]) return false;
    }
    return true;
}

// ============================================
// QuickSort Tests
// ============================================

void test_quick_sort_basic() {
    TEST("QuickSort basic")
    std::vector<int> v = {5, 2, 8, 1, 9, 3, 7, 4, 6};
    quick_sort(v.begin(), v.end());
    assert(is_sorted_asc(v));
    END_TEST
}

void test_quick_sort_empty() {
    TEST("QuickSort empty vector")
    std::vector<int> v;
    quick_sort(v.begin(), v.end());
    assert(v.empty());
    END_TEST
}

void test_quick_sort_single() {
    TEST("QuickSort single element")
    std::vector<int> v = {42};
    quick_sort(v.begin(), v.end());
    assert(v[0] == 42);
    END_TEST
}

void test_quick_sort_sorted() {
    TEST("QuickSort already sorted")
    std::vector<int> v = {1, 2, 3, 4, 5};
    quick_sort(v.begin(), v.end());
    assert(is_sorted_asc(v));
    END_TEST
}

void test_quick_sort_reverse() {
    TEST("QuickSort reverse sorted")
    std::vector<int> v = {5, 4, 3, 2, 1};
    quick_sort(v.begin(), v.end());
    assert(is_sorted_asc(v));
    END_TEST
}

void test_quick_sort_duplicates() {
    TEST("QuickSort with duplicates")
    std::vector<int> v = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
    quick_sort(v.begin(), v.end());
    assert(is_sorted_asc(v));
    END_TEST
}

void test_quick_sort_custom_compare() {
    TEST("QuickSort with custom comparator (descending)")
    std::vector<int> v = {5, 2, 8, 1, 9};
    quick_sort(v.begin(), v.end(), std::greater<int>{});
    assert(is_sorted_desc(v));
    END_TEST
}

// ============================================
// MergeSort Tests
// ============================================

void test_merge_sort_basic() {
    TEST("MergeSort basic")
    std::vector<int> v = {5, 2, 8, 1, 9, 3, 7, 4, 6};
    merge_sort(v.begin(), v.end());
    assert(is_sorted_asc(v));
    END_TEST
}

void test_merge_sort_empty() {
    TEST("MergeSort empty vector")
    std::vector<int> v;
    merge_sort(v.begin(), v.end());
    assert(v.empty());
    END_TEST
}

void test_merge_sort_single() {
    TEST("MergeSort single element")
    std::vector<int> v = {42};
    merge_sort(v.begin(), v.end());
    assert(v[0] == 42);
    END_TEST
}

void test_merge_sort_duplicates() {
    TEST("MergeSort with duplicates")
    std::vector<int> v = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
    merge_sort(v.begin(), v.end());
    assert(is_sorted_asc(v));
    END_TEST
}

void test_merge_sort_stability() {
    TEST("MergeSort stability")
    // Pairs where we sort by first element only
    std::vector<std::pair<int, int>> v = {{2, 1}, {1, 1}, {2, 2}, {1, 2}, {2, 3}};
    merge_sort(v.begin(), v.end(), [](const auto& a, const auto& b) {
        return a.first < b.first;
    });
    
    // Check sorted by first
    for (size_t i = 1; i < v.size(); ++i) {
        assert(v[i].first >= v[i-1].first);
    }
    // In stable sort, elements with same first should maintain relative order
    // Original order of 2s: (2,1), (2,2), (2,3)
    std::vector<int> twos_order;
    for (const auto& p : v) {
        if (p.first == 2) twos_order.push_back(p.second);
    }
    assert(twos_order[0] == 1);
    assert(twos_order[1] == 2);
    assert(twos_order[2] == 3);
    END_TEST
}

// ============================================
// HeapSort Tests
// ============================================

void test_heap_sort_basic() {
    TEST("HeapSort basic")
    std::vector<int> v = {5, 2, 8, 1, 9, 3, 7, 4, 6};
    heap_sort(v.begin(), v.end());
    assert(is_sorted_asc(v));
    END_TEST
}

void test_heap_sort_empty() {
    TEST("HeapSort empty vector")
    std::vector<int> v;
    heap_sort(v.begin(), v.end());
    assert(v.empty());
    END_TEST
}

void test_heap_sort_single() {
    TEST("HeapSort single element")
    std::vector<int> v = {42};
    heap_sort(v.begin(), v.end());
    assert(v[0] == 42);
    END_TEST
}

void test_heap_sort_duplicates() {
    TEST("HeapSort with duplicates")
    std::vector<int> v = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
    heap_sort(v.begin(), v.end());
    assert(is_sorted_asc(v));
    END_TEST
}

void test_heap_sort_custom_compare() {
    TEST("HeapSort with custom comparator")
    std::vector<int> v = {5, 2, 8, 1, 9};
    heap_sort(v.begin(), v.end(), std::greater<int>{});
    assert(is_sorted_desc(v));
    END_TEST
}

// ============================================
// InsertionSort Tests
// ============================================

void test_insertion_sort_basic() {
    TEST("InsertionSort basic")
    std::vector<int> v = {5, 2, 8, 1, 9, 3, 7, 4, 6};
    insertion_sort(v.begin(), v.end());
    assert(is_sorted_asc(v));
    END_TEST
}

void test_insertion_sort_nearly_sorted() {
    TEST("InsertionSort nearly sorted (efficient case)")
    std::vector<int> v = {1, 2, 4, 3, 5, 6, 8, 7, 9, 10};
    insertion_sort(v.begin(), v.end());
    assert(is_sorted_asc(v));
    END_TEST
}

void test_insertion_sort_small() {
    TEST("InsertionSort small array")
    std::vector<int> v = {3, 1, 2};
    insertion_sort(v.begin(), v.end());
    assert(is_sorted_asc(v));
    END_TEST
}

// ============================================
// Sorter Class Tests
// ============================================

void test_sorter_simple() {
    TEST("Sorter simple sort")
    std::vector<int> v = {5, 2, 8, 1, 9};
    Sorter<int>::sort_default(v);
    assert(is_sorted_asc(v));
    END_TEST
}

void test_sorter_descending() {
    TEST("Sorter descending")
    std::vector<int> v = {5, 2, 8, 1, 9};
    Sorter<int>().descending().quick_sort(v);
    assert(is_sorted_desc(v));
    END_TEST
}

void test_sorter_with_stats() {
    TEST("Sorter with statistics")
    std::vector<int> v = {5, 2, 8, 1, 9, 3, 7, 4, 6};
    auto stats = Sorter<int>::with_stats().quick_sort(v);
    
    assert(is_sorted_asc(v));
    assert(stats.comparisons > 0);
    assert(stats.swaps > 0);
    assert(stats.elapsed_ms >= 0);
    END_TEST
}

void test_sorter_by_key_length() {
    TEST("Sorter by_key (string length)")
    std::vector<std::string> v = {"apple", "pie", "banana", "kiwi", "strawberry"};
    Sorter<std::string>::by_key([](const std::string& s) {
        return s.length();
    }).sort(v);
    
    // Should be sorted by length
    for (size_t i = 1; i < v.size(); ++i) {
        assert(v[i].length() >= v[i-1].length());
    }
    END_TEST
}

void test_sorter_by_key_absolute() {
    TEST("Sorter by_key (absolute value)")
    std::vector<int> v = {-5, 2, -8, 1, -9, 3};
    Sorter<int>::by_key([](int x) {
        return std::abs(x);
    }).sort(v);
    
    // Should be sorted by absolute value
    for (size_t i = 1; i < v.size(); ++i) {
        assert(std::abs(v[i]) >= std::abs(v[i-1]));
    }
    END_TEST
}

void test_sorter_by_key_struct() {
    TEST("Sorter by_key (struct field)")
    struct Person {
        std::string name;
        int age;
    };
    
    std::vector<Person> people = {
        {"Alice", 30},
        {"Bob", 25},
        {"Charlie", 35},
        {"Diana", 28}
    };
    
    Sorter<Person>::by_key([](const Person& p) {
        return p.age;
    }).sort(people);
    
    // Should be sorted by age
    for (size_t i = 1; i < people.size(); ++i) {
        assert(people[i].age >= people[i-1].age);
    }
    assert(people[0].name == "Bob");
    assert(people[3].name == "Charlie");
    END_TEST
}

void test_sorter_custom_compare() {
    TEST("Sorter with_compare")
    std::vector<int> v = {5, 2, 8, 1, 9};
    Sorter<int>::with_compare([](int a, int b) {
        return a > b;  // Descending
    }).sort(v);
    assert(is_sorted_desc(v));
    END_TEST
}

// ============================================
// Static Utility Functions Tests
// ============================================

void test_sorted_copy() {
    TEST("Sorter::sorted (non-modifying)")
    std::vector<int> original = {5, 2, 8, 1, 9};
    auto sorted_vec = Sorter<int>::sorted(original);
    
    assert(is_sorted_asc(sorted_vec));
    assert(original[0] == 5);  // Original unchanged
    END_TEST
}

void test_sorted_by() {
    TEST("Sorter::sorted_by")
    std::vector<std::string> original = {"apple", "pie", "banana"};
    auto sorted_vec = Sorter<std::string>::sorted_by(original, [](const std::string& s) {
        return s.length();
    });
    
    assert(sorted_vec[0] == "pie");
    assert(sorted_vec[1] == "apple");
    assert(sorted_vec[2] == "banana");
    END_TEST
}

void test_argsort() {
    TEST("Sorter::argsort")
    std::vector<int> v = {30, 10, 20, 50, 40};
    auto indices = Sorter<int>::argsort(v);
    
    assert(indices.size() == 5);
    assert(indices[0] == 1);  // 10 is at index 1
    assert(indices[1] == 2);  // 20 is at index 2
    assert(indices[2] == 0);  // 30 is at index 0
    assert(indices[3] == 4);  // 40 is at index 4
    assert(indices[4] == 3);  // 50 is at index 3
    END_TEST
}

void test_top_k() {
    TEST("Sorter::top_k")
    std::vector<int> v = {3, 1, 4, 1, 5, 9, 2, 6};
    auto top3 = Sorter<int>::top_k(v, 3);
    
    assert(top3.size() == 3);
    assert(top3[0] == 9);
    assert(top3[1] == 6);
    assert(top3[2] == 5);
    END_TEST
}

void test_bottom_k() {
    TEST("Sorter::bottom_k")
    std::vector<int> v = {3, 1, 4, 1, 5, 9, 2, 6};
    auto bottom3 = Sorter<int>::bottom_k(v, 3);
    
    assert(bottom3.size() == 3);
    assert(bottom3[0] == 1);
    assert(bottom3[1] == 1);
    assert(bottom3[2] == 2);
    END_TEST
}

void test_is_sorted_check() {
    TEST("Sorter::is_sorted")
    std::vector<int> sorted_vec = {1, 2, 3, 4, 5};
    std::vector<int> unsorted_vec = {3, 1, 4, 1, 5};
    
    assert(Sorter<int>::is_sorted(sorted_vec));
    assert(!Sorter<int>::is_sorted(unsorted_vec));
    END_TEST
}

// ============================================
// Utility Functions Tests
// ============================================

void test_shuffle() {
    TEST("shuffle")
    std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::vector<int> original = v;
    
    mylib::algorithm::shuffle(v.begin(), v.end(), 42);  // With seed
    
    // Should contain same elements
    std::sort(v.begin(), v.end());
    assert(v == original);
    END_TEST
}

void test_shuffle_reproducible() {
    TEST("shuffle reproducible with seed")
    std::vector<int> v1 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::vector<int> v2 = v1;
    
    mylib::algorithm::shuffle(v1.begin(), v1.end(), 42);
    mylib::algorithm::shuffle(v2.begin(), v2.end(), 42);
    
    assert(v1 == v2);
    END_TEST
}

void test_reverse() {
    TEST("reverse")
    std::vector<int> v = {1, 2, 3, 4, 5};
    mylib::algorithm::reverse(v.begin(), v.end());
    
    assert(v[0] == 5);
    assert(v[4] == 1);
    END_TEST
}

void test_is_sorted_function() {
    TEST("is_sorted function")
    std::vector<int> v1 = {1, 2, 3, 4, 5};
    std::vector<int> v2 = {5, 4, 3, 2, 1};
    std::vector<int> v3 = {3, 1, 4};
    
    assert(mylib::algorithm::is_sorted(v1.begin(), v1.end()));
    assert(!mylib::algorithm::is_sorted(v2.begin(), v2.end()));
    assert(mylib::algorithm::is_sorted(v2.begin(), v2.end(), std::greater<int>{}));
    assert(!mylib::algorithm::is_sorted(v3.begin(), v3.end()));
    END_TEST
}

// ============================================
// Partial Sort Tests
// ============================================

void test_partial_sort() {
    TEST("Sorter partial_sort")
    std::vector<int> v = {9, 3, 7, 1, 5, 8, 2, 6, 4};
    
    Sorter<int>().partial_sort(v.begin(), v.begin() + 3, v.end());
    
    // First 3 elements should be smallest, sorted
    assert(v[0] == 1);
    assert(v[1] == 2);
    assert(v[2] == 3);
    END_TEST
}

void test_nth_element() {
    TEST("Sorter nth_element")
    std::vector<int> v = {9, 3, 7, 1, 5, 8, 2, 6, 4};
    
    // Find median (4th smallest, 0-indexed)
    int median = Sorter<int>().nth_element(v.begin(), 4, v.end());
    assert(median == 5);
    END_TEST
}

// ============================================
// String Sort Tests
// ============================================

void test_string_sort() {
    TEST("String sort alphabetically")
    std::vector<std::string> v = {"banana", "apple", "cherry", "date"};
    quick_sort(v.begin(), v.end());
    
    assert(v[0] == "apple");
    assert(v[1] == "banana");
    assert(v[2] == "cherry");
    assert(v[3] == "date");
    END_TEST
}

void test_string_sort_case_insensitive() {
    TEST("String sort case-insensitive")
    std::vector<std::string> v = {"Banana", "apple", "Cherry", "DATE"};
    
    Sorter<std::string>::by_key([](const std::string& s) {
        std::string lower = s;
        std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
        return lower;
    }).sort(v);
    
    // Should be: apple, Banana, Cherry, DATE
    assert(v[0] == "apple");
    assert(v[1] == "Banana");
    END_TEST
}

// ============================================
// Double/Float Sort Tests
// ============================================

void test_double_sort() {
    TEST("Double sort")
    std::vector<double> v = {3.14, 1.41, 2.71, 1.73, 0.57};
    quick_sort(v.begin(), v.end());
    assert(is_sorted_asc(v));
    END_TEST
}

// ============================================
// Large Dataset Tests
// ============================================

void test_quick_sort_large() {
    TEST("QuickSort large dataset (10000)")
    std::vector<int> v(10000);
    std::iota(v.begin(), v.end(), 0);
    mylib::algorithm::shuffle(v.begin(), v.end(), 42);
    
    quick_sort(v.begin(), v.end());
    assert(is_sorted_asc(v));
    END_TEST
}

void test_merge_sort_large() {
    TEST("MergeSort large dataset (10000)")
    std::vector<int> v(10000);
    std::iota(v.begin(), v.end(), 0);
    mylib::algorithm::shuffle(v.begin(), v.end(), 42);
    
    merge_sort(v.begin(), v.end());
    assert(is_sorted_asc(v));
    END_TEST
}

void test_heap_sort_large() {
    TEST("HeapSort large dataset (10000)")
    std::vector<int> v(10000);
    std::iota(v.begin(), v.end(), 0);
    mylib::algorithm::shuffle(v.begin(), v.end(), 42);
    
    heap_sort(v.begin(), v.end());
    assert(is_sorted_asc(v));
    END_TEST
}

// ============================================
// Edge Cases
// ============================================

void test_all_same_elements() {
    TEST("All same elements")
    std::vector<int> v(100, 42);
    quick_sort(v.begin(), v.end());
    
    for (int x : v) {
        assert(x == 42);
    }
    END_TEST
}

void test_two_elements() {
    TEST("Two elements")
    std::vector<int> v1 = {2, 1};
    std::vector<int> v2 = {1, 2};
    
    quick_sort(v1.begin(), v1.end());
    quick_sort(v2.begin(), v2.end());
    
    assert(v1[0] == 1 && v1[1] == 2);
    assert(v2[0] == 1 && v2[1] == 2);
    END_TEST
}

void test_negative_numbers() {
    TEST("Negative numbers")
    std::vector<int> v = {-5, 3, -2, 8, -1, 0, -9, 4};
    quick_sort(v.begin(), v.end());
    assert(is_sorted_asc(v));
    assert(v[0] == -9);
    assert(v[v.size()-1] == 8);
    END_TEST
}

// ============================================
// Statistics Tests
// ============================================

void test_stats_comparison() {
    TEST("Compare algorithm statistics")
    std::vector<int> v1 = {5, 2, 8, 1, 9, 3, 7, 4, 6, 0};
    std::vector<int> v2 = v1;
    std::vector<int> v3 = v1;
    
    auto quick_stats = Sorter<int>::with_stats().quick_sort(v1);
    auto merge_stats = Sorter<int>::with_stats().merge_sort(v2);
    auto heap_stats = Sorter<int>::with_stats().heap_sort(v3);
    
    assert(is_sorted_asc(v1));
    assert(is_sorted_asc(v2));
    assert(is_sorted_asc(v3));
    
    // All should have recorded some operations
    assert(quick_stats.comparisons > 0);
    assert(merge_stats.comparisons > 0);
    assert(heap_stats.comparisons > 0);
    END_TEST
}

// ============================================
// Practical Use Cases
// ============================================

void test_sort_by_multiple_criteria() {
    TEST("Sort by multiple criteria")
    struct Student {
        std::string name;
        int grade;
        int age;
    };
    
    std::vector<Student> students = {
        {"Alice", 90, 20},
        {"Bob", 85, 21},
        {"Charlie", 90, 19},
        {"Diana", 85, 20}
    };
    
    // Sort by grade (desc), then by age (asc)
    Sorter<Student>::with_compare([](const Student& a, const Student& b) {
        if (a.grade != b.grade) return a.grade > b.grade;
        return a.age < b.age;
    }).sort(students);
    
    assert(students[0].name == "Charlie");  // 90, 19
    assert(students[1].name == "Alice");    // 90, 20
    assert(students[2].name == "Diana");    // 85, 20
    assert(students[3].name == "Bob");      // 85, 21
    END_TEST
}

void test_find_kth_largest() {
    TEST("Find k-th largest element")
    std::vector<int> v = {3, 2, 1, 5, 6, 4};
    int k = 2;  // Find 2nd largest
    
    auto top = Sorter<int>::top_k(v, k);
    assert(top.back() == 5);  // 2nd largest
    END_TEST
}

void test_percentile() {
    TEST("Find percentile")
    std::vector<int> v(100);
    std::iota(v.begin(), v.end(), 1);  // 1 to 100
    mylib::algorithm::shuffle(v.begin(), v.end(), 42);
    
    // Find 90th percentile (index 89 in 0-indexed sorted array)
    int p90 = Sorter<int>().nth_element(v.begin(), 89, v.end());
    assert(p90 == 90);
    END_TEST
}

// Main test runner
int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "Sorting Algorithms Test Suite" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;

    // QuickSort tests
    std::cout << "--- QuickSort Tests ---" << std::endl;
    test_quick_sort_basic();
    test_quick_sort_empty();
    test_quick_sort_single();
    test_quick_sort_sorted();
    test_quick_sort_reverse();
    test_quick_sort_duplicates();
    test_quick_sort_custom_compare();

    // MergeSort tests
    std::cout << std::endl << "--- MergeSort Tests ---" << std::endl;
    test_merge_sort_basic();
    test_merge_sort_empty();
    test_merge_sort_single();
    test_merge_sort_duplicates();
    test_merge_sort_stability();

    // HeapSort tests
    std::cout << std::endl << "--- HeapSort Tests ---" << std::endl;
    test_heap_sort_basic();
    test_heap_sort_empty();
    test_heap_sort_single();
    test_heap_sort_duplicates();
    test_heap_sort_custom_compare();

    // InsertionSort tests
    std::cout << std::endl << "--- InsertionSort Tests ---" << std::endl;
    test_insertion_sort_basic();
    test_insertion_sort_nearly_sorted();
    test_insertion_sort_small();

    // Sorter class tests
    std::cout << std::endl << "--- Sorter Class Tests ---" << std::endl;
    test_sorter_simple();
    test_sorter_descending();
    test_sorter_with_stats();
    test_sorter_by_key_length();
    test_sorter_by_key_absolute();
    test_sorter_by_key_struct();
    test_sorter_custom_compare();

    // Static utility tests
    std::cout << std::endl << "--- Static Utility Tests ---" << std::endl;
    test_sorted_copy();
    test_sorted_by();
    test_argsort();
    test_top_k();
    test_bottom_k();
    test_is_sorted_check();

    // Utility function tests
    std::cout << std::endl << "--- Utility Function Tests ---" << std::endl;
    test_shuffle();
    test_shuffle_reproducible();
    test_reverse();
    test_is_sorted_function();

    // Partial sort tests
    std::cout << std::endl << "--- Partial Sort Tests ---" << std::endl;
    test_partial_sort();
    test_nth_element();

    // String tests
    std::cout << std::endl << "--- String Sort Tests ---" << std::endl;
    test_string_sort();
    test_string_sort_case_insensitive();

    // Double tests
    std::cout << std::endl << "--- Double Sort Tests ---" << std::endl;
    test_double_sort();

    // Large dataset tests
    std::cout << std::endl << "--- Large Dataset Tests ---" << std::endl;
    test_quick_sort_large();
    test_merge_sort_large();
    test_heap_sort_large();

    // Edge cases
    std::cout << std::endl << "--- Edge Cases ---" << std::endl;
    test_all_same_elements();
    test_two_elements();
    test_negative_numbers();

    // Statistics tests
    std::cout << std::endl << "--- Statistics Tests ---" << std::endl;
    test_stats_comparison();

    // Practical use cases
    std::cout << std::endl << "--- Practical Use Cases ---" << std::endl;
    test_sort_by_multiple_criteria();
    test_find_kth_largest();
    test_percentile();

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