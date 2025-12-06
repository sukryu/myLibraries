/**
 * @file benchmark_utils.hpp
 * @brief Utilities for benchmarking data structures and algorithms
 * @author Jinhyeok
 * @date 2025-12-06
 * @version 1.0.0
 * 
 * This file provides utilities for:
 * - High-precision timing
 * - Test data generation
 * - Memory usage tracking
 * - Result formatting and visualization
 * 
 * Copyright (c) 2025 Jinhyeok
 * MIT License
 */

#ifndef BENCHMARK_UTILS_HPP
#define BENCHMARK_UTILS_HPP

#include <chrono>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <numeric>
#include <iomanip>
#include <iostream>
#include <functional>
#include <map>
#include <sstream>

namespace benchmark {

// ============================================
// Timer - High precision timing
// ============================================

/**
 * @class Timer
 * @brief High-precision timer for benchmarking
 * 
 * Usage:
 * @code
 * Timer timer;
 * timer.start();
 * // ... code to benchmark ...
 * timer.stop();
 * std::cout << "Time: " << timer.elapsed_ms() << " ms" << std::endl;
 * @endcode
 */
class Timer {
private:
    using Clock = std::chrono::high_resolution_clock;
    using TimePoint = std::chrono::time_point<Clock>;
    using Duration = std::chrono::duration<double>;
    
    TimePoint m_start;
    TimePoint m_end;
    bool m_running;
    
public:
    Timer() : m_running(false) {}
    
    /**
     * @brief Start the timer
     */
    void start() {
        m_start = Clock::now();
        m_running = true;
    }
    
    /**
     * @brief Stop the timer
     */
    void stop() {
        m_end = Clock::now();
        m_running = false;
    }
    
    /**
     * @brief Get elapsed time in nanoseconds
     */
    long long elapsed_ns() const {
        auto end = m_running ? Clock::now() : m_end;
        return std::chrono::duration_cast<std::chrono::nanoseconds>(end - m_start).count();
    }
    
    /**
     * @brief Get elapsed time in microseconds
     */
    double elapsed_us() const {
        return elapsed_ns() / 1000.0;
    }
    
    /**
     * @brief Get elapsed time in milliseconds
     */
    double elapsed_ms() const {
        return elapsed_ns() / 1000000.0;
    }
    
    /**
     * @brief Get elapsed time in seconds
     */
    double elapsed_s() const {
        return elapsed_ns() / 1000000000.0;
    }
    
    /**
     * @brief Auto-format elapsed time with appropriate unit
     */
    std::string elapsed_auto() const {
        long long ns = elapsed_ns();
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2);
        
        if (ns < 1000) {
            oss << ns << " ns";
        } else if (ns < 1000000) {
            oss << (ns / 1000.0) << " μs";
        } else if (ns < 1000000000) {
            oss << (ns / 1000000.0) << " ms";
        } else {
            oss << (ns / 1000000000.0) << " s";
        }
        
        return oss.str();
    }
};

/**
 * @brief RAII timer that automatically starts and stops
 */
class ScopedTimer {
private:
    Timer& m_timer;
    
public:
    explicit ScopedTimer(Timer& timer) : m_timer(timer) {
        m_timer.start();
    }
    
    ~ScopedTimer() {
        m_timer.stop();
    }
};

/**
 * @brief Measure execution time of a function
 */
template <typename Func>
double measure_time_ms(Func&& func) {
    Timer timer;
    timer.start();
    func();
    timer.stop();
    return timer.elapsed_ms();
}

// ============================================
// DataGenerator - Test data generation
// ============================================

/**
 * @class DataGenerator
 * @brief Generate test data for benchmarking
 */
template <typename T>
class DataGenerator {
private:
    std::mt19937 m_rng;
    
public:
    DataGenerator() : m_rng(std::random_device{}()) {}
    
    explicit DataGenerator(unsigned seed) : m_rng(seed) {}
    
    /**
     * @brief Generate sequential data [start, start+1, ..., start+count-1]
     */
    std::vector<T> sequential(std::size_t count, T start = T()) {
        std::vector<T> data(count);
        std::iota(data.begin(), data.end(), start);
        return data;
    }
    
    /**
     * @brief Generate reverse sequential data [start+count-1, ..., start+1, start]
     */
    std::vector<T> reverse_sequential(std::size_t count, T start = T()) {
        auto data = sequential(count, start);
        std::reverse(data.begin(), data.end());
        return data;
    }
    
    /**
     * @brief Generate random data in range [min, max]
     */
    std::vector<T> random(std::size_t count, T min_val, T max_val) {
        std::uniform_int_distribution<T> dist(min_val, max_val);
        std::vector<T> data(count);
        for (auto& val : data) {
            val = dist(m_rng);
        }
        return data;
    }
    
    /**
     * @brief Generate shuffled sequential data
     */
    std::vector<T> shuffled(std::size_t count, T start = T()) {
        auto data = sequential(count, start);
        std::shuffle(data.begin(), data.end(), m_rng);
        return data;
    }
    
    /**
     * @brief Generate nearly sorted data (k% shuffled)
     */
    std::vector<T> nearly_sorted(std::size_t count, double shuffle_percent = 10.0, T start = T()) {
        auto data = sequential(count, start);
        std::size_t shuffle_count = static_cast<std::size_t>(count * shuffle_percent / 100.0);
        
        for (std::size_t i = 0; i < shuffle_count; ++i) {
            std::uniform_int_distribution<std::size_t> dist(0, count - 1);
            std::size_t idx1 = dist(m_rng);
            std::size_t idx2 = dist(m_rng);
            std::swap(data[idx1], data[idx2]);
        }
        
        return data;
    }
    
    /**
     * @brief Generate data with duplicates
     */
    std::vector<T> with_duplicates(std::size_t count, std::size_t unique_count) {
        if (unique_count > count) unique_count = count;
        
        auto unique_data = random(unique_count, T(), static_cast<T>(unique_count * 10));
        std::vector<T> data(count);
        
        std::uniform_int_distribution<std::size_t> dist(0, unique_count - 1);
        for (auto& val : data) {
            val = unique_data[dist(m_rng)];
        }
        
        return data;
    }
};

// ============================================
// MemoryTracker - Memory usage estimation
// ============================================

/**
 * @class MemoryTracker
 * @brief Track memory usage of data structures
 */
class MemoryTracker {
public:
    /**
     * @brief Estimate memory usage of a container
     */
    template <typename Container>
    static std::size_t estimate_size(const Container& container) {
        // Basic estimation: sizeof(Container) + element storage
        return sizeof(Container) + container.size() * sizeof(typename Container::value_type);
    }
    
    /**
     * @brief Format bytes to human-readable string
     */
    static std::string format_bytes(std::size_t bytes) {
        const char* units[] = {"B", "KB", "MB", "GB"};
        int unit_idx = 0;
        double size = static_cast<double>(bytes);
        
        while (size >= 1024.0 && unit_idx < 3) {
            size /= 1024.0;
            ++unit_idx;
        }
        
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2) << size << " " << units[unit_idx];
        return oss.str();
    }
};

// ============================================
// BenchmarkResult - Store benchmark results
// ============================================

/**
 * @struct BenchmarkResult
 * @brief Store results of a benchmark run
 */
struct BenchmarkResult {
    std::string name;                    // Name of the benchmark
    std::size_t data_size;               // Size of test data
    double time_ms;                      // Execution time in milliseconds
    std::size_t memory_bytes;            // Memory usage in bytes
    std::map<std::string, double> extras; // Additional metrics
    
    BenchmarkResult(const std::string& name = "", 
                   std::size_t data_size = 0,
                   double time_ms = 0.0,
                   std::size_t memory_bytes = 0)
        : name(name), data_size(data_size), time_ms(time_ms), memory_bytes(memory_bytes) {}
    
    /**
     * @brief Add an extra metric
     */
    void add_metric(const std::string& key, double value) {
        extras[key] = value;
    }
    
    /**
     * @brief Get throughput (operations per second)
     */
    double throughput() const {
        if (time_ms == 0.0) return 0.0;
        return (data_size / time_ms) * 1000.0;  // ops/sec
    }
};

// ============================================
// ResultFormatter - Format and display results
// ============================================

/**
 * @class ResultFormatter
 * @brief Format benchmark results for display
 */
class ResultFormatter {
public:
    /**
     * @brief Print a single result
     */
    static void print_result(const BenchmarkResult& result) {
        std::cout << std::left << std::setw(30) << result.name
                  << std::right << std::setw(12) << result.data_size
                  << std::setw(15) << std::fixed << std::setprecision(3) << result.time_ms << " ms"
                  << std::setw(15) << MemoryTracker::format_bytes(result.memory_bytes)
                  << std::setw(18) << std::fixed << std::setprecision(0) << result.throughput() << " ops/s"
                  << std::endl;
    }
    
    /**
     * @brief Print table header
     */
    static void print_header() {
        std::cout << std::string(90, '=') << std::endl;
        std::cout << std::left << std::setw(30) << "Benchmark"
                  << std::right << std::setw(12) << "Size"
                  << std::setw(15) << "Time"
                  << std::setw(15) << "Memory"
                  << std::setw(18) << "Throughput"
                  << std::endl;
        std::cout << std::string(90, '-') << std::endl;
    }
    
    /**
     * @brief Print table footer
     */
    static void print_footer() {
        std::cout << std::string(90, '=') << std::endl;
    }
    
    /**
     * @brief Print comparison table of multiple results
     */
    static void print_comparison(const std::vector<BenchmarkResult>& results) {
        if (results.empty()) return;
        
        print_header();
        for (const auto& result : results) {
            print_result(result);
        }
        print_footer();
    }
    
    /**
     * @brief Print comparison with speedup/slowdown
     */
    static void print_comparison_with_baseline(const std::vector<BenchmarkResult>& results, 
                                                std::size_t baseline_idx = 0) {
        if (results.empty() || baseline_idx >= results.size()) return;
        
        const auto& baseline = results[baseline_idx];
        
        std::cout << std::string(110, '=') << std::endl;
        std::cout << std::left << std::setw(30) << "Benchmark"
                  << std::right << std::setw(12) << "Size"
                  << std::setw(15) << "Time"
                  << std::setw(15) << "Memory"
                  << std::setw(18) << "Throughput"
                  << std::setw(20) << "Relative Speed"
                  << std::endl;
        std::cout << std::string(110, '-') << std::endl;
        
        for (const auto& result : results) {
            std::cout << std::left << std::setw(30) << result.name
                      << std::right << std::setw(12) << result.data_size
                      << std::setw(15) << std::fixed << std::setprecision(3) << result.time_ms << " ms"
                      << std::setw(15) << MemoryTracker::format_bytes(result.memory_bytes)
                      << std::setw(18) << std::fixed << std::setprecision(0) << result.throughput() << " ops/s";
            
            if (result.name == baseline.name) {
                std::cout << std::setw(20) << "(baseline)";
            } else {
                double speedup = baseline.time_ms / result.time_ms;
                std::cout << std::setw(20) << std::fixed << std::setprecision(2) << speedup << "x";
            }
            std::cout << std::endl;
        }
        
        std::cout << std::string(110, '=') << std::endl;
    }
    
    /**
     * @brief Print section header
     */
    static void print_section(const std::string& title) {
        std::cout << "\n" << std::string(90, '=') << std::endl;
        std::cout << "  " << title << std::endl;
        std::cout << std::string(90, '=') << std::endl << std::endl;
    }
};

// ============================================
// BenchmarkRunner - Run and manage benchmarks
// ============================================

/**
 * @class BenchmarkRunner
 * @brief Run and manage multiple benchmarks
 */
class BenchmarkRunner {
private:
    std::vector<BenchmarkResult> m_results;
    bool m_verbose;
    
public:
    explicit BenchmarkRunner(bool verbose = true) : m_verbose(verbose) {}
    
    /**
     * @brief Run a benchmark function
     */
    template <typename Func>
    BenchmarkResult run(const std::string& name, 
                       std::size_t data_size,
                       Func&& benchmark_func) {
        if (m_verbose) {
            std::cout << "Running: " << name << " (size=" << data_size << ")... " << std::flush;
        }
        
        Timer timer;
        std::size_t memory = 0;
        
        timer.start();
        memory = benchmark_func();  // Function should return memory usage
        timer.stop();
        
        BenchmarkResult result(name, data_size, timer.elapsed_ms(), memory);
        m_results.push_back(result);
        
        if (m_verbose) {
            std::cout << "Done (" << timer.elapsed_auto() << ")" << std::endl;
        }
        
        return result;
    }
    
    /**
     * @brief Run a benchmark without memory tracking
     */
    template <typename Func>
    BenchmarkResult run_simple(const std::string& name, 
                              std::size_t data_size,
                              Func&& benchmark_func) {
        return run(name, data_size, [&]() {
            benchmark_func();
            return 0;  // No memory tracking
        });
    }
    
    /**
     * @brief Get all results
     */
    const std::vector<BenchmarkResult>& results() const {
        return m_results;
    }
    
    /**
     * @brief Clear all results
     */
    void clear() {
        m_results.clear();
    }
    
    /**
     * @brief Print summary of all results
     */
    void print_summary() const {
        ResultFormatter::print_comparison(m_results);
    }
    
    /**
     * @brief Print summary with baseline comparison
     */
    void print_summary_with_baseline(std::size_t baseline_idx = 0) const {
        ResultFormatter::print_comparison_with_baseline(m_results, baseline_idx);
    }
};

// ============================================
// Progress indicator
// ============================================

/**
 * @class ProgressBar
 * @brief Simple progress bar for long-running benchmarks
 */
class ProgressBar {
private:
    std::size_t m_total;
    std::size_t m_current;
    int m_bar_width;
    
public:
    explicit ProgressBar(std::size_t total, int bar_width = 50)
        : m_total(total), m_current(0), m_bar_width(bar_width) {}
    
    void update(std::size_t current) {
        m_current = current;
        display();
    }
    
    void increment() {
        update(m_current + 1);
    }
    
    void display() const {
        double progress = static_cast<double>(m_current) / m_total;
        int pos = static_cast<int>(m_bar_width * progress);
        
        std::cout << "[";
        for (int i = 0; i < m_bar_width; ++i) {
            if (i < pos) std::cout << "=";
            else if (i == pos) std::cout << ">";
            else std::cout << " ";
        }
        std::cout << "] " << int(progress * 100.0) << "% (" 
                  << m_current << "/" << m_total << ")\r";
        std::cout.flush();
        
        if (m_current >= m_total) {
            std::cout << std::endl;
        }
    }
};

} // namespace benchmark

#endif // BENCHMARK_UTILS_HPP