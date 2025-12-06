/**
 * @file balanced_tree_benchmark.cpp
 * @brief Benchmark comparing AVL Tree, Red-Black Tree, and Skip List
 * @author Jinhyeok
 * @date 2025-12-06
 * 
 * This benchmark compares three balanced search tree implementations:
 * - AVL Tree: Strictly balanced with rotations
 * - Red-Black Tree: Loosely balanced with color rules
 * - Skip List: Probabilistically balanced without rotations
 * 
 * Measurements:
 * - Insertion performance (sequential, reverse, random)
 * - Search performance (hit/miss)
 * - Deletion performance
 * - Memory usage
 * - Tree height/structure
 * 
 * Environment: GitHub Codespaces
 */

#include "benchmark_utils.hpp"
#include "tree/avl_tree.hpp"
#include "tree/red_black_tree.hpp"
#include "tree/skip_list.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>

using namespace benchmark;
using namespace mylib::tree;

// ============================================
// Configuration
// ============================================

const std::vector<std::size_t> DATASET_SIZES = {
    1000,      // Small
    10000,     // Medium
    100000     // Large
};

const int NUM_WARMUP_RUNS = 1;
const int NUM_BENCHMARK_RUNS = 3;

// ============================================
// Helper Functions
// ============================================

/**
 * @brief Get tree height for AVL tree
 */
template <typename T>
int get_height(const AVLTree<T>& tree) {
    return tree.height();
}

/**
 * @brief Get tree height for Red-Black tree
 */
template <typename T>
int get_height(const RedBlackTree<T>& tree) {
    return tree.height();
}

/**
 * @brief Get current level for Skip List
 */
template <typename T>
int get_height(const SkipList<T>& tree) {
    return tree.current_level();
}

/**
 * @brief Estimate memory usage
 */
template <typename Tree>
std::size_t estimate_memory(const Tree& tree, std::size_t element_count) {
    (void)tree;  // Suppress unused parameter warning
    // Base estimation: tree object + nodes
    std::size_t node_overhead = sizeof(void*) * 3;  // Typical node pointers
    return sizeof(Tree) + element_count * (sizeof(int) + node_overhead);
}

// Skip List has variable levels
template <>
std::size_t estimate_memory<SkipList<int>>(const SkipList<int>& tree, std::size_t element_count) {
    // Skip list nodes have variable forward pointers
    std::size_t avg_level = (tree.current_level() + 1) / 2.0;
    std::size_t node_size = sizeof(int) + sizeof(std::vector<void*>) + sizeof(void*) * avg_level;
    return sizeof(SkipList<int>) + element_count * node_size;
}

// ============================================
// Helper functions for tree operations
// ============================================

/**
 * @brief Delete element from AVL Tree (uses remove)
 */
inline void delete_element(AVLTree<int>& tree, int value) {
    tree.remove(value);
}

/**
 * @brief Delete element from Red-Black Tree (uses erase)
 */
inline void delete_element(RedBlackTree<int>& tree, int value) {
    tree.erase(value);
}

/**
 * @brief Delete element from Skip List (uses remove)
 */
inline void delete_element(SkipList<int>& tree, int value) {
    tree.remove(value);
}

// ============================================
// Benchmark Functions
// ============================================

/**
 * @brief Benchmark insertion with sequential data
 */
template <typename Tree>
BenchmarkResult benchmark_insert_sequential(const std::string& name, std::size_t size) {
    DataGenerator<int> gen;
    auto data = gen.sequential(size, 0);
    
    Tree tree;
    Timer timer;
    
    timer.start();
    for (const auto& val : data) {
        tree.insert(val);
    }
    timer.stop();
    
    std::size_t memory = estimate_memory(tree, size);
    int height = get_height(tree);
    
    BenchmarkResult result(name + " (Sequential)", size, timer.elapsed_ms(), memory);
    result.add_metric("height", height);
    
    return result;
}

/**
 * @brief Benchmark insertion with reverse data
 */
template <typename Tree>
BenchmarkResult benchmark_insert_reverse(const std::string& name, std::size_t size) {
    DataGenerator<int> gen;
    auto data = gen.reverse_sequential(size, 0);
    
    Tree tree;
    Timer timer;
    
    timer.start();
    for (const auto& val : data) {
        tree.insert(val);
    }
    timer.stop();
    
    std::size_t memory = estimate_memory(tree, size);
    int height = get_height(tree);
    
    BenchmarkResult result(name + " (Reverse)", size, timer.elapsed_ms(), memory);
    result.add_metric("height", height);
    
    return result;
}

/**
 * @brief Benchmark insertion with random data
 */
template <typename Tree>
BenchmarkResult benchmark_insert_random(const std::string& name, std::size_t size) {
    DataGenerator<int> gen;
    auto data = gen.shuffled(size, 0);
    
    Tree tree;
    Timer timer;
    
    timer.start();
    for (const auto& val : data) {
        tree.insert(val);
    }
    timer.stop();
    
    std::size_t memory = estimate_memory(tree, size);
    int height = get_height(tree);
    
    BenchmarkResult result(name + " (Random)", size, timer.elapsed_ms(), memory);
    result.add_metric("height", height);
    
    return result;
}

/**
 * @brief Benchmark search with hits
 */
template <typename Tree>
BenchmarkResult benchmark_search_hit(const std::string& name, std::size_t size) {
    DataGenerator<int> gen;
    auto data = gen.shuffled(size, 0);
    
    // Build tree
    Tree tree;
    for (const auto& val : data) {
        tree.insert(val);
    }
    
    // Search for existing elements
    auto search_data = gen.shuffled(size, 0);
    Timer timer;
    
    timer.start();
    for (const auto& val : search_data) {
        tree.find(val);
    }
    timer.stop();
    
    std::size_t memory = estimate_memory(tree, size);
    
    BenchmarkResult result(name + " (Search Hit)", size, timer.elapsed_ms(), memory);
    
    return result;
}

/**
 * @brief Benchmark search with misses
 */
template <typename Tree>
BenchmarkResult benchmark_search_miss(const std::string& name, std::size_t size) {
    DataGenerator<int> gen;
    auto data = gen.shuffled(size, 0);
    
    // Build tree
    Tree tree;
    for (const auto& val : data) {
        tree.insert(val);
    }
    
    // Search for non-existing elements
    auto search_data = gen.sequential(size, size + 1000);  // Outside range
    Timer timer;
    
    timer.start();
    for (const auto& val : search_data) {
        tree.find(val);
    }
    timer.stop();
    
    std::size_t memory = estimate_memory(tree, size);
    
    BenchmarkResult result(name + " (Search Miss)", size, timer.elapsed_ms(), memory);
    
    return result;
}

/**
 * @brief Benchmark deletion
 */
template <typename Tree>
BenchmarkResult benchmark_delete(const std::string& name, std::size_t size) {
    DataGenerator<int> gen;
    auto data = gen.shuffled(size, 0);
    
    // Build tree
    Tree tree;
    for (const auto& val : data) {
        tree.insert(val);
    }
    
    // Delete half the elements
    auto delete_data = gen.shuffled(size / 2, 0);
    Timer timer;
    
    timer.start();
    for (const auto& val : delete_data) {
        delete_element(tree, val);
    }
    timer.stop();
    
    std::size_t memory = estimate_memory(tree, size / 2);
    
    BenchmarkResult result(name + " (Delete)", size / 2, timer.elapsed_ms(), memory);
    
    return result;
}

// ============================================
// Run All Benchmarks
// ============================================

/**
 * @brief Run all benchmarks for a specific tree type
 */
template <typename Tree>
std::vector<BenchmarkResult> run_tree_benchmarks(const std::string& tree_name, std::size_t size) {
    std::vector<BenchmarkResult> results;
    
    std::cout << "\n  Running " << tree_name << " benchmarks (size=" << size << ")..." << std::endl;
    
    // Insertion benchmarks
    results.push_back(benchmark_insert_sequential<Tree>(tree_name, size));
    results.push_back(benchmark_insert_reverse<Tree>(tree_name, size));
    results.push_back(benchmark_insert_random<Tree>(tree_name, size));
    
    // Search benchmarks
    results.push_back(benchmark_search_hit<Tree>(tree_name, size));
    results.push_back(benchmark_search_miss<Tree>(tree_name, size));
    
    // Delete benchmark
    results.push_back(benchmark_delete<Tree>(tree_name, size));
    
    return results;
}

/**
 * @brief Print comparison for a specific operation
 */
void print_operation_comparison(const std::string& operation,
                                const std::vector<BenchmarkResult>& avl_results,
                                const std::vector<BenchmarkResult>& rb_results,
                                const std::vector<BenchmarkResult>& skip_results) {
    ResultFormatter::print_section(operation);
    
    std::vector<BenchmarkResult> comparison;
    
    // Find matching results
    for (const auto& r : avl_results) {
        if (r.name.find(operation) != std::string::npos) {
            comparison.push_back(r);
            break;
        }
    }
    
    for (const auto& r : rb_results) {
        if (r.name.find(operation) != std::string::npos) {
            comparison.push_back(r);
            break;
        }
    }
    
    for (const auto& r : skip_results) {
        if (r.name.find(operation) != std::string::npos) {
            comparison.push_back(r);
            break;
        }
    }
    
    if (!comparison.empty()) {
        ResultFormatter::print_comparison_with_baseline(comparison, 0);
        
        // Print tree heights if available
        bool has_height = false;
        for (const auto& r : comparison) {
            if (r.extras.find("height") != r.extras.end()) {
                has_height = true;
                break;
            }
        }
        
        if (has_height) {
            std::cout << "\nTree Heights:" << std::endl;
            for (const auto& r : comparison) {
                auto it = r.extras.find("height");
                if (it != r.extras.end()) {
                    std::cout << "  " << std::left << std::setw(30) << r.name
                              << ": " << static_cast<int>(it->second) << std::endl;
                }
            }
        }
    }
}

// ============================================
// Main
// ============================================

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "Balanced Tree Benchmark" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Comparing: AVL Tree, Red-Black Tree, Skip List" << std::endl;
    std::cout << "Environment: GitHub Codespaces" << std::endl;
    std::cout << "========================================" << std::endl;
    
    for (std::size_t size : DATASET_SIZES) {
        std::cout << "\n" << std::string(90, '=') << std::endl;
        std::cout << "Dataset Size: " << size << " elements" << std::endl;
        std::cout << std::string(90, '=') << std::endl;
        
        // Run benchmarks for each tree type
        auto avl_results = run_tree_benchmarks<AVLTree<int>>("AVL Tree", size);
        auto rb_results = run_tree_benchmarks<RedBlackTree<int>>("Red-Black Tree", size);
        auto skip_results = run_tree_benchmarks<SkipList<int>>("Skip List", size);
        
        // Print comparisons by operation
        print_operation_comparison("Sequential", avl_results, rb_results, skip_results);
        print_operation_comparison("Reverse", avl_results, rb_results, skip_results);
        print_operation_comparison("Random", avl_results, rb_results, skip_results);
        print_operation_comparison("Search Hit", avl_results, rb_results, skip_results);
        print_operation_comparison("Search Miss", avl_results, rb_results, skip_results);
        print_operation_comparison("Delete", avl_results, rb_results, skip_results);
    }
    
    // Summary and recommendations
    std::cout << "\n" << std::string(90, '=') << std::endl;
    std::cout << "Summary and Recommendations" << std::endl;
    std::cout << std::string(90, '=') << std::endl;
    std::cout << std::endl;
    
    std::cout << "Implementation Complexity:" << std::endl;
    std::cout << "  AVL Tree:        ⭐⭐⭐⭐   (Complex rotations, strict balancing)" << std::endl;
    std::cout << "  Red-Black Tree:  ⭐⭐⭐⭐⭐ (Very complex, color rules)" << std::endl;
    std::cout << "  Skip List:       ⭐⭐     (Simple, no rotations)" << std::endl;
    std::cout << std::endl;
    
    std::cout << "Use Cases:" << std::endl;
    std::cout << "  AVL Tree:        Best for read-heavy workloads (frequent searches)" << std::endl;
    std::cout << "  Red-Black Tree:  Balanced performance, standard choice (std::map)" << std::endl;
    std::cout << "  Skip List:       Easy to implement, good for concurrent access" << std::endl;
    std::cout << std::endl;
    
    std::cout << "Performance Characteristics:" << std::endl;
    std::cout << "  AVL Tree:        Faster searches, slower insertions/deletions" << std::endl;
    std::cout << "  Red-Black Tree:  Balanced across all operations" << std::endl;
    std::cout << "  Skip List:       Faster insertions/deletions, probabilistic guarantees" << std::endl;
    std::cout << std::endl;
    
    std::cout << "Tree Height:" << std::endl;
    std::cout << "  AVL Tree:        Minimal (strictly balanced)" << std::endl;
    std::cout << "  Red-Black Tree:  ~1.5x AVL height (looser balancing)" << std::endl;
    std::cout << "  Skip List:       Probabilistic (~log n on average)" << std::endl;
    std::cout << std::endl;
    
    std::cout << "========================================" << std::endl;
    std::cout << "Benchmark Complete!" << std::endl;
    std::cout << "========================================" << std::endl;
    
    return 0;
}