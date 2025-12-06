/**
 * @file test_benchmark_utils.cpp
 * @brief Test benchmark utilities
 */

#include "benchmark_utils.hpp"
#include <iostream>
#include <vector>
#include <algorithm>
#include <thread>

using namespace benchmark;

void test_timer() {
    std::cout << "=== Testing Timer ===" << std::endl;
    
    Timer timer;
    timer.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    timer.stop();
    
    std::cout << "Elapsed: " << timer.elapsed_ms() << " ms" << std::endl;
    std::cout << "Auto format: " << timer.elapsed_auto() << std::endl;
    std::cout << std::endl;
}

void test_data_generator() {
    std::cout << "=== Testing DataGenerator ===" << std::endl;
    
    DataGenerator<int> gen;
    
    auto seq = gen.sequential(10, 1);
    std::cout << "Sequential: ";
    for (int val : seq) std::cout << val << " ";
    std::cout << std::endl;
    
    auto rev = gen.reverse_sequential(10, 1);
    std::cout << "Reverse: ";
    for (int val : rev) std::cout << val << " ";
    std::cout << std::endl;
    
    auto shuffled = gen.shuffled(10, 1);
    std::cout << "Shuffled: ";
    for (int val : shuffled) std::cout << val << " ";
    std::cout << std::endl;
    
    auto nearly = gen.nearly_sorted(10, 20.0, 1);
    std::cout << "Nearly sorted (20%): ";
    for (int val : nearly) std::cout << val << " ";
    std::cout << std::endl;
    
    std::cout << std::endl;
}

void test_memory_tracker() {
    std::cout << "=== Testing MemoryTracker ===" << std::endl;
    
    std::vector<int> vec(1000);
    std::size_t size = MemoryTracker::estimate_size(vec);
    
    std::cout << "Vector of 1000 ints: " << MemoryTracker::format_bytes(size) << std::endl;
    std::cout << "1 MB: " << MemoryTracker::format_bytes(1024 * 1024) << std::endl;
    std::cout << "1 GB: " << MemoryTracker::format_bytes(1024 * 1024 * 1024) << std::endl;
    std::cout << std::endl;
}

void test_result_formatter() {
    std::cout << "=== Testing ResultFormatter ===" << std::endl;
    
    std::vector<BenchmarkResult> results;
    results.push_back(BenchmarkResult("QuickSort", 10000, 5.234, 40000));
    results.push_back(BenchmarkResult("MergeSort", 10000, 6.789, 80000));
    results.push_back(BenchmarkResult("HeapSort", 10000, 7.123, 40000));
    
    ResultFormatter::print_comparison(results);
    std::cout << std::endl;
    
    ResultFormatter::print_comparison_with_baseline(results, 0);
}

void test_benchmark_runner() {
    std::cout << "\n=== Testing BenchmarkRunner ===" << std::endl;
    
    BenchmarkRunner runner(true);
    
    // Simple sorting benchmark
    auto benchmark_sort = [](std::size_t size) {
        DataGenerator<int> gen;
        auto data = gen.shuffled(size, 0);
        std::sort(data.begin(), data.end());
    };
    
    runner.run_simple("Sort 1000", 1000, [&]() { benchmark_sort(1000); });
    runner.run_simple("Sort 5000", 5000, [&]() { benchmark_sort(5000); });
    runner.run_simple("Sort 10000", 10000, [&]() { benchmark_sort(10000); });
    
    std::cout << std::endl;
    runner.print_summary();
}

void test_progress_bar() {
    std::cout << "\n=== Testing ProgressBar ===" << std::endl;
    
    ProgressBar progress(100);
    
    for (int i = 0; i <= 100; ++i) {
        progress.update(i);
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
}

void test_measure_time() {
    std::cout << "\n=== Testing measure_time_ms ===" << std::endl;
    
    DataGenerator<int> gen;
    auto data = gen.shuffled(10000, 0);
    
    double time = measure_time_ms([&]() {
        std::sort(data.begin(), data.end());
    });
    
    std::cout << "Sorting 10000 elements took: " << time << " ms" << std::endl;
}

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "Benchmark Utilities Test Suite" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;
    
    test_timer();
    test_data_generator();
    test_memory_tracker();
    test_result_formatter();
    test_benchmark_runner();
    test_progress_bar();
    test_measure_time();
    
    std::cout << "\n✓ All utility tests completed!" << std::endl;
    
    return 0;
}