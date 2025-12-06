/**
 * @file sorting_benchmark.cpp
 * @brief Benchmark comparing sorting algorithms
 * @author Jinhyeok
 * @date 2025-12-06
 * 
 * This benchmark compares sorting algorithm implementations:
 * - QuickSort: Divide-and-conquer with median-of-three pivot
 * - MergeSort: Stable divide-and-conquer
 * - HeapSort: In-place using binary heap
 * - InsertionSort: Simple comparison-based (for small arrays)
 * 
 * Test patterns:
 * - Random data
 * - Nearly sorted data (90% sorted)
 * - Reverse sorted data
 * - Many duplicates
 * - Small arrays
 * 
 * Environment: GitHub Codespaces
 */

#include "benchmark_utils.hpp"
#include "algorithm/sorting.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <cassert>

using namespace benchmark;
using namespace mylib::algorithm;

// ============================================
// Configuration
// ============================================

const std::vector<std::size_t> DATASET_SIZES = {
    100,       // Tiny (InsertionSort shines)
    1000,      // Small
    10000,     // Medium
    100000     // Large
};

const std::vector<std::size_t> SMALL_SIZES = {10, 50, 100, 500};

// ============================================
// Benchmark Functions
// ============================================

/**
 * @brief Benchmark QuickSort on given data
 */
template <typename T>
BenchmarkResult benchmark_quicksort(const std::string& name, 
                                   const std::vector<T>& data) {
    auto test_data = data;
    Timer timer;
    
    timer.start();
    quick_sort(test_data.begin(), test_data.end());
    timer.stop();
    
    // Verify sorted
    assert(std::is_sorted(test_data.begin(), test_data.end()));
    
    std::size_t memory = MemoryTracker::estimate_size(test_data);
    BenchmarkResult result("QuickSort - " + name, data.size(), 
                          timer.elapsed_ms(), memory);
    
    return result;
}

/**
 * @brief Benchmark MergeSort on given data
 */
template <typename T>
BenchmarkResult benchmark_mergesort(const std::string& name,
                                   const std::vector<T>& data) {
    auto test_data = data;
    Timer timer;
    
    timer.start();
    merge_sort(test_data.begin(), test_data.end());
    timer.stop();
    
    assert(std::is_sorted(test_data.begin(), test_data.end()));
    
    std::size_t memory = MemoryTracker::estimate_size(test_data);
    BenchmarkResult result("MergeSort - " + name, data.size(),
                          timer.elapsed_ms(), memory);
    
    return result;
}

/**
 * @brief Benchmark HeapSort on given data
 */
template <typename T>
BenchmarkResult benchmark_heapsort(const std::string& name,
                                  const std::vector<T>& data) {
    auto test_data = data;
    Timer timer;
    
    timer.start();
    heap_sort(test_data.begin(), test_data.end());
    timer.stop();
    
    assert(std::is_sorted(test_data.begin(), test_data.end()));
    
    std::size_t memory = MemoryTracker::estimate_size(test_data);
    BenchmarkResult result("HeapSort - " + name, data.size(),
                          timer.elapsed_ms(), memory);
    
    return result;
}

/**
 * @brief Benchmark InsertionSort on given data
 */
template <typename T>
BenchmarkResult benchmark_insertionsort(const std::string& name,
                                       const std::vector<T>& data) {
    auto test_data = data;
    Timer timer;
    
    timer.start();
    insertion_sort(test_data.begin(), test_data.end());
    timer.stop();
    
    assert(std::is_sorted(test_data.begin(), test_data.end()));
    
    std::size_t memory = MemoryTracker::estimate_size(test_data);
    BenchmarkResult result("InsertionSort - " + name, data.size(),
                          timer.elapsed_ms(), memory);
    
    return result;
}

/**
 * @brief Benchmark std::sort (usually IntroSort: QuickSort + HeapSort)
 */
template <typename T>
BenchmarkResult benchmark_stdsort(const std::string& name,
                                 const std::vector<T>& data) {
    auto test_data = data;
    Timer timer;
    
    timer.start();
    std::sort(test_data.begin(), test_data.end());
    timer.stop();
    
    assert(std::is_sorted(test_data.begin(), test_data.end()));
    
    std::size_t memory = MemoryTracker::estimate_size(test_data);
    BenchmarkResult result("std::sort - " + name, data.size(),
                          timer.elapsed_ms(), memory);
    
    return result;
}

// ============================================
// Run Benchmarks by Pattern
// ============================================

/**
 * @brief Run all sorting algorithms on a dataset
 */
std::vector<BenchmarkResult> run_sorting_benchmarks(
    const std::string& pattern_name,
    const std::vector<int>& data,
    bool include_insertion = true) {
    
    std::vector<BenchmarkResult> results;
    
    std::cout << "  Testing " << pattern_name 
              << " (size=" << data.size() << ")..." << std::endl;
    
    results.push_back(benchmark_quicksort(pattern_name, data));
    results.push_back(benchmark_mergesort(pattern_name, data));
    results.push_back(benchmark_heapsort(pattern_name, data));
    
    // InsertionSort too slow for large arrays
    if (include_insertion && data.size() <= 1000) {
        results.push_back(benchmark_insertionsort(pattern_name, data));
    }
    
    // Baseline: std::sort
    results.push_back(benchmark_stdsort(pattern_name, data));
    
    return results;
}

/**
 * @brief Print comparison for a specific pattern
 */
void print_pattern_results(const std::string& pattern,
                          const std::vector<BenchmarkResult>& results) {
    ResultFormatter::print_section(pattern);
    ResultFormatter::print_comparison_with_baseline(results, 0);
}

// ============================================
// Main
// ============================================

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "Sorting Algorithms Benchmark" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Comparing: QuickSort, MergeSort, HeapSort, InsertionSort, std::sort" << std::endl;
    std::cout << "Environment: GitHub Codespaces" << std::endl;
    std::cout << "========================================" << std::endl;
    
    DataGenerator<int> gen;
    
    // ========================================
    // Test 1: Small Arrays (InsertionSort advantage)
    // ========================================
    
    std::cout << "\n" << std::string(90, '=') << std::endl;
    std::cout << "Small Arrays Test (10, 50, 100, 500 elements)" << std::endl;
    std::cout << std::string(90, '=') << std::endl;
    
    for (std::size_t size : SMALL_SIZES) {
        auto data = gen.shuffled(size, 0);
        auto results = run_sorting_benchmarks("Small Random", data, true);
        
        if (size == 100) {
            print_pattern_results("Small Arrays (100 elements)", results);
        }
    }
    
    // ========================================
    // Test 2: Random Data (typical case)
    // ========================================
    
    for (std::size_t size : DATASET_SIZES) {
        std::cout << "\n" << std::string(90, '=') << std::endl;
        std::cout << "Dataset Size: " << size << " elements" << std::endl;
        std::cout << std::string(90, '=') << std::endl;
        
        // Random data
        auto random_data = gen.shuffled(size, 0);
        auto random_results = run_sorting_benchmarks("Random", random_data, size <= 1000);
        print_pattern_results("Random Data", random_results);
        
        // Nearly sorted (90% sorted)
        auto nearly_sorted = gen.nearly_sorted(size, 10.0, 0);
        auto nearly_results = run_sorting_benchmarks("Nearly Sorted", nearly_sorted, size <= 1000);
        print_pattern_results("Nearly Sorted (90%)", nearly_results);
        
        // Reverse sorted
        auto reverse_sorted = gen.reverse_sequential(size, 0);
        auto reverse_results = run_sorting_benchmarks("Reverse", reverse_sorted, size <= 1000);
        print_pattern_results("Reverse Sorted", reverse_results);
        
        // Many duplicates (only 10% unique values)
        auto duplicates = gen.with_duplicates(size, size / 10);
        auto dup_results = run_sorting_benchmarks("Duplicates", duplicates, size <= 1000);
        print_pattern_results("Many Duplicates (10% unique)", dup_results);
    }
    
    // ========================================
    // Summary
    // ========================================
    
    std::cout << "\n" << std::string(90, '=') << std::endl;
    std::cout << "Summary and Recommendations" << std::endl;
    std::cout << std::string(90, '=') << std::endl;
    std::cout << std::endl;
    
    std::cout << "Algorithm Characteristics:" << std::endl;
    std::cout << "  QuickSort:       Fast average case, in-place, unstable" << std::endl;
    std::cout << "  MergeSort:       Stable, O(n log n) guaranteed, extra space O(n)" << std::endl;
    std::cout << "  HeapSort:        In-place, O(n log n) guaranteed, unstable" << std::endl;
    std::cout << "  InsertionSort:   Best for small/nearly sorted, O(n²) worst case" << std::endl;
    std::cout << "  std::sort:       IntroSort (Quick+Heap), industry standard" << std::endl;
    std::cout << std::endl;
    
    std::cout << "Time Complexity:" << std::endl;
    std::cout << "  QuickSort:       O(n log n) average, O(n²) worst" << std::endl;
    std::cout << "  MergeSort:       O(n log n) always" << std::endl;
    std::cout << "  HeapSort:        O(n log n) always" << std::endl;
    std::cout << "  InsertionSort:   O(n) best, O(n²) average/worst" << std::endl;
    std::cout << std::endl;
    
    std::cout << "Space Complexity:" << std::endl;
    std::cout << "  QuickSort:       O(log n) stack space" << std::endl;
    std::cout << "  MergeSort:       O(n) auxiliary space" << std::endl;
    std::cout << "  HeapSort:        O(1) in-place" << std::endl;
    std::cout << "  InsertionSort:   O(1) in-place" << std::endl;
    std::cout << std::endl;
    
    std::cout << "Stability:" << std::endl;
    std::cout << "  QuickSort:       ✗ Unstable" << std::endl;
    std::cout << "  MergeSort:       ✓ Stable" << std::endl;
    std::cout << "  HeapSort:        ✗ Unstable" << std::endl;
    std::cout << "  InsertionSort:   ✓ Stable" << std::endl;
    std::cout << std::endl;
    
    std::cout << "Best Use Cases:" << std::endl;
    std::cout << "  QuickSort:       General-purpose, random data" << std::endl;
    std::cout << "  MergeSort:       Need stability, linked lists, external sorting" << std::endl;
    std::cout << "  HeapSort:        Memory constrained, need O(n log n) guarantee" << std::endl;
    std::cout << "  InsertionSort:   Small arrays (n < 50), nearly sorted data" << std::endl;
    std::cout << "  std::sort:       Default choice for production" << std::endl;
    std::cout << std::endl;
    
    std::cout << "========================================" << std::endl;
    std::cout << "Benchmark Complete!" << std::endl;
    std::cout << "========================================" << std::endl;
    
    return 0;
}