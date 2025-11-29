/**
 * @file sorting.hpp
 * @brief Comprehensive sorting algorithms with flexible interfaces
 * @author Jinhyeok
 * @date 2025-11-29
 * @version 1.0.0
 * 
 * This file provides various sorting algorithms with:
 * - Iterator-based interface (works with any container)
 * - Custom comparator support
 * - Stability options
 * - Parallel execution hints
 * - Sorting statistics and analysis
 * - Partial sorting capabilities
 * - Key-based sorting (like Python's key parameter)
 * 
 * Copyright (c) 2025 Jinhyeok
 * MIT License
 */

#ifndef MYLIB_ALGORITHM_SORTING_HPP
#define MYLIB_ALGORITHM_SORTING_HPP

#include <cstddef>
#include <functional>
#include <iterator>
#include <vector>
#include <algorithm>
#include <utility>
#include <random>
#include <chrono>
#include <type_traits>

namespace mylib {
namespace algorithm {

/**
 * @struct SortStats
 * @brief Statistics collected during sorting operations
 */
struct SortStats {
    std::size_t comparisons = 0;    ///< Number of comparisons made
    std::size_t swaps = 0;          ///< Number of swaps performed
    std::size_t copies = 0;         ///< Number of copy operations
    double elapsed_ms = 0.0;        ///< Time elapsed in milliseconds
    
    void reset() {
        comparisons = swaps = copies = 0;
        elapsed_ms = 0.0;
    }
    
    SortStats& operator+=(const SortStats& other) {
        comparisons += other.comparisons;
        swaps += other.swaps;
        copies += other.copies;
        elapsed_ms += other.elapsed_ms;
        return *this;
    }
};

/**
 * @enum SortOrder
 * @brief Specifies the sorting order
 */
enum class SortOrder {
    Ascending,
    Descending
};

/**
 * @struct SortConfig
 * @brief Configuration options for sorting algorithms
 */
struct SortConfig {
    bool collect_stats = false;         ///< Whether to collect statistics
    bool stable = false;                ///< Whether to use stable sorting
    std::size_t insertion_threshold = 16; ///< Threshold for switching to insertion sort
    std::size_t parallel_threshold = 10000; ///< Threshold for parallel execution (hint)
};

// ============================================
// Forward declarations
// ============================================

template <typename RandomIt, typename Compare>
void quick_sort(RandomIt first, RandomIt last, Compare comp);

template <typename RandomIt, typename Compare>
void merge_sort(RandomIt first, RandomIt last, Compare comp);

template <typename RandomIt, typename Compare>
void heap_sort(RandomIt first, RandomIt last, Compare comp);

// ============================================
// Sorting class with full features
// ============================================

/**
 * @class Sorter
 * @brief A feature-rich sorting utility class
 * 
 * Provides various sorting algorithms with:
 * - Fluent interface for configuration
 * - Statistics collection
 * - Key-based sorting (like Python's sorted(key=...))
 * - Multiple algorithm choices
 * 
 * Example usage:
 * @code
 * std::vector<int> v = {3, 1, 4, 1, 5, 9};
 * 
 * // Simple sort
 * Sorter<int>::sort(v);
 * 
 * // With options
 * auto stats = Sorter<int>::with_stats()
 *     .descending()
 *     .quick_sort(v);
 * 
 * // Key-based sort (sort strings by length)
 * std::vector<std::string> words = {"apple", "pie", "banana"};
 * Sorter<std::string>::by_key([](const std::string& s) { 
 *     return s.length(); 
 * }).sort(words);
 * @endcode
 */
template <typename T>
class Sorter {
public:
    using value_type = T;
    using compare_type = std::function<bool(const T&, const T&)>;

private:
    /**
     * @brief Private constructor for factory methods
     */
    Sorter(compare_type comp) : m_compare(std::move(comp)), m_order(SortOrder::Ascending) {}

public:
    /**
     * @brief Default constructor with ascending order
     */
    Sorter() : m_compare([](const T& a, const T& b) { return a < b; }), m_order(SortOrder::Ascending) {}

    /**
     * @brief Create sorter with statistics collection enabled
     */
    static Sorter with_stats() {
        Sorter s;
        s.m_config.collect_stats = true;
        return s;
    }

    /**
     * @brief Create sorter with key extractor (like Python's key parameter)
     * @param key_func Function that extracts a comparable key from each element
     */
    template <typename KeyFunc>
    static Sorter by_key(KeyFunc key_func) {
        Sorter s([key_func](const T& a, const T& b) {
            return key_func(a) < key_func(b);
        });
        return s;
    }

    /**
     * @brief Create sorter with custom comparator
     */
    static Sorter with_compare(compare_type comp) {
        Sorter s(std::move(comp));
        return s;
    }

    // Fluent configuration methods
    Sorter& ascending() { 
        m_order = SortOrder::Ascending; 
        return *this; 
    }
    
    Sorter& descending() { 
        m_order = SortOrder::Descending; 
        return *this; 
    }
    
    Sorter& stable_sort() { 
        m_config.stable = true; 
        return *this; 
    }
    
    Sorter& collect_stats() { 
        m_config.collect_stats = true; 
        return *this; 
    }

    Sorter& set_threshold(std::size_t threshold) {
        m_config.insertion_threshold = threshold;
        return *this;
    }

    // ============================================
    // Container-based sorting (convenience)
    // ============================================

    /**
     * @brief Sort a container using the default algorithm (IntroSort-like)
     */
    SortStats sort(std::vector<T>& container) {
        return quick_sort(container);
    }

    /**
     * @brief Sort using QuickSort
     */
    SortStats quick_sort(std::vector<T>& container) {
        return quick_sort_range(container.begin(), container.end());
    }

    /**
     * @brief Sort using MergeSort (stable)
     */
    SortStats merge_sort(std::vector<T>& container) {
        return merge_sort_range(container.begin(), container.end());
    }

    /**
     * @brief Sort using HeapSort
     */
    SortStats heap_sort(std::vector<T>& container) {
        return heap_sort_range(container.begin(), container.end());
    }

    /**
     * @brief Sort using InsertionSort (good for small/nearly sorted data)
     */
    SortStats insertion_sort(std::vector<T>& container) {
        return insertion_sort_range(container.begin(), container.end());
    }

    // ============================================
    // Iterator-based sorting
    // ============================================

    /**
     * @brief Sort range using QuickSort
     */
    template <typename RandomIt>
    SortStats quick_sort_range(RandomIt first, RandomIt last) {
        SortStats stats;
        auto start_time = std::chrono::high_resolution_clock::now();
        
        auto comp = get_effective_compare();
        if (m_config.collect_stats) {
            quick_sort_impl(first, last, comp, stats);
        } else {
            quick_sort_impl(first, last, comp);
        }
        
        auto end_time = std::chrono::high_resolution_clock::now();
        stats.elapsed_ms = std::chrono::duration<double, std::milli>(end_time - start_time).count();
        return stats;
    }

    /**
     * @brief Sort range using MergeSort
     */
    template <typename RandomIt>
    SortStats merge_sort_range(RandomIt first, RandomIt last) {
        SortStats stats;
        auto start_time = std::chrono::high_resolution_clock::now();
        
        auto comp = get_effective_compare();
        std::vector<T> buffer(std::distance(first, last));
        if (m_config.collect_stats) {
            merge_sort_impl(first, last, buffer.begin(), comp, stats);
        } else {
            merge_sort_impl(first, last, buffer.begin(), comp);
        }
        
        auto end_time = std::chrono::high_resolution_clock::now();
        stats.elapsed_ms = std::chrono::duration<double, std::milli>(end_time - start_time).count();
        return stats;
    }

    /**
     * @brief Sort range using HeapSort
     */
    template <typename RandomIt>
    SortStats heap_sort_range(RandomIt first, RandomIt last) {
        SortStats stats;
        auto start_time = std::chrono::high_resolution_clock::now();
        
        auto comp = get_effective_compare();
        if (m_config.collect_stats) {
            heap_sort_impl(first, last, comp, stats);
        } else {
            heap_sort_impl(first, last, comp);
        }
        
        auto end_time = std::chrono::high_resolution_clock::now();
        stats.elapsed_ms = std::chrono::duration<double, std::milli>(end_time - start_time).count();
        return stats;
    }

    /**
     * @brief Sort range using InsertionSort
     */
    template <typename RandomIt>
    SortStats insertion_sort_range(RandomIt first, RandomIt last) {
        SortStats stats;
        auto start_time = std::chrono::high_resolution_clock::now();
        
        auto comp = get_effective_compare();
        if (m_config.collect_stats) {
            insertion_sort_impl(first, last, comp, stats);
        } else {
            insertion_sort_impl(first, last, comp);
        }
        
        auto end_time = std::chrono::high_resolution_clock::now();
        stats.elapsed_ms = std::chrono::duration<double, std::milli>(end_time - start_time).count();
        return stats;
    }

    // ============================================
    // Partial sorting
    // ============================================

    /**
     * @brief Partially sort so that first k elements are the smallest k (sorted)
     */
    template <typename RandomIt>
    SortStats partial_sort(RandomIt first, RandomIt middle, RandomIt last) {
        SortStats stats;
        auto start_time = std::chrono::high_resolution_clock::now();
        
        auto comp = get_effective_compare();
        partial_sort_impl(first, middle, last, comp, stats);
        
        auto end_time = std::chrono::high_resolution_clock::now();
        stats.elapsed_ms = std::chrono::duration<double, std::milli>(end_time - start_time).count();
        return stats;
    }

    /**
     * @brief Find the k-th smallest element (0-indexed)
     */
    template <typename RandomIt>
    typename std::iterator_traits<RandomIt>::value_type
    nth_element(RandomIt first, std::size_t k, RandomIt last) {
        auto comp = get_effective_compare();
        nth_element_impl(first, first + k, last, comp);
        return *(first + k);
    }

    // ============================================
    // Static convenience functions
    // ============================================

    /**
     * @brief Simple ascending sort (static convenience)
     */
    static void sort_default(std::vector<T>& container) {
        Sorter().quick_sort(container);
    }

    /**
     * @brief Check if container is sorted
     */
    static bool is_sorted(const std::vector<T>& container) {
        return std::is_sorted(container.begin(), container.end());
    }

    /**
     * @brief Check if container is sorted with custom comparator
     */
    template <typename Compare>
    static bool is_sorted(const std::vector<T>& container, Compare comp) {
        return std::is_sorted(container.begin(), container.end(), comp);
    }

    /**
     * @brief Get a sorted copy without modifying original
     */
    static std::vector<T> sorted(const std::vector<T>& container) {
        std::vector<T> result = container;
        Sorter().quick_sort(result);
        return result;
    }

    /**
     * @brief Get a sorted copy with key function (like Python)
     */
    template <typename KeyFunc>
    static std::vector<T> sorted_by(const std::vector<T>& container, KeyFunc key_func) {
        std::vector<T> result = container;
        Sorter::by_key(key_func).sort(result);
        return result;
    }

    /**
     * @brief Get indices that would sort the array (like numpy.argsort)
     */
    static std::vector<std::size_t> argsort(const std::vector<T>& container) {
        std::vector<std::size_t> indices(container.size());
        for (std::size_t i = 0; i < indices.size(); ++i) {
            indices[i] = i;
        }
        std::sort(indices.begin(), indices.end(), [&container](std::size_t a, std::size_t b) {
            return container[a] < container[b];
        });
        return indices;
    }

    /**
     * @brief Get top k elements (sorted)
     */
    static std::vector<T> top_k(const std::vector<T>& container, std::size_t k) {
        if (k >= container.size()) {
            return sorted(container);
        }
        std::vector<T> result = container;
        std::partial_sort(result.begin(), result.begin() + k, result.end(), std::greater<T>{});
        result.resize(k);
        return result;
    }

    /**
     * @brief Get bottom k elements (sorted)
     */
    static std::vector<T> bottom_k(const std::vector<T>& container, std::size_t k) {
        if (k >= container.size()) {
            return sorted(container);
        }
        std::vector<T> result = container;
        std::partial_sort(result.begin(), result.begin() + k, result.end());
        result.resize(k);
        return result;
    }

private:
    compare_type m_compare;
    SortOrder m_order;
    SortConfig m_config;

    /**
     * @brief Get effective comparator considering sort order
     */
    compare_type get_effective_compare() const {
        if (m_order == SortOrder::Descending) {
            return [this](const T& a, const T& b) {
                return m_compare(b, a);
            };
        }
        return m_compare;
    }

    // ============================================
    // QuickSort implementation
    // ============================================

    template <typename RandomIt, typename Compare>
    void quick_sort_impl(RandomIt first, RandomIt last, Compare comp) {
        while (last - first > static_cast<std::ptrdiff_t>(m_config.insertion_threshold)) {
            // Median-of-three pivot selection
            RandomIt pivot = median_of_three(first, first + (last - first) / 2, last - 1, comp);
            pivot = partition_impl(first, last, pivot, comp);
            
            // Recurse on smaller partition, iterate on larger (tail call optimization)
            if (pivot - first < last - pivot) {
                quick_sort_impl(first, pivot, comp);
                first = pivot + 1;
            } else {
                quick_sort_impl(pivot + 1, last, comp);
                last = pivot;
            }
        }
        insertion_sort_impl(first, last, comp);
    }

    template <typename RandomIt, typename Compare>
    void quick_sort_impl(RandomIt first, RandomIt last, Compare comp, SortStats& stats) {
        while (last - first > static_cast<std::ptrdiff_t>(m_config.insertion_threshold)) {
            RandomIt pivot = median_of_three(first, first + (last - first) / 2, last - 1, comp, stats);
            pivot = partition_impl(first, last, pivot, comp, stats);
            
            if (pivot - first < last - pivot) {
                quick_sort_impl(first, pivot, comp, stats);
                first = pivot + 1;
            } else {
                quick_sort_impl(pivot + 1, last, comp, stats);
                last = pivot;
            }
        }
        insertion_sort_impl(first, last, comp, stats);
    }

    template <typename RandomIt, typename Compare>
    RandomIt median_of_three(RandomIt a, RandomIt b, RandomIt c, Compare comp) {
        if (comp(*a, *b)) {
            if (comp(*b, *c)) return b;
            else if (comp(*a, *c)) return c;
            else return a;
        } else {
            if (comp(*a, *c)) return a;
            else if (comp(*b, *c)) return c;
            else return b;
        }
    }

    template <typename RandomIt, typename Compare>
    RandomIt median_of_three(RandomIt a, RandomIt b, RandomIt c, Compare comp, SortStats& stats) {
        stats.comparisons += 3;
        return median_of_three(a, b, c, comp);
    }

    template <typename RandomIt, typename Compare>
    RandomIt partition_impl(RandomIt first, RandomIt last, RandomIt pivot, Compare comp) {
        auto pivot_value = std::move(*pivot);
        std::iter_swap(pivot, last - 1);
        
        RandomIt store = first;
        for (RandomIt it = first; it != last - 1; ++it) {
            if (comp(*it, pivot_value)) {
                std::iter_swap(it, store);
                ++store;
            }
        }
        std::iter_swap(store, last - 1);
        return store;
    }

    template <typename RandomIt, typename Compare>
    RandomIt partition_impl(RandomIt first, RandomIt last, RandomIt pivot, Compare comp, SortStats& stats) {
        auto pivot_value = std::move(*pivot);
        std::iter_swap(pivot, last - 1);
        ++stats.swaps;
        
        RandomIt store = first;
        for (RandomIt it = first; it != last - 1; ++it) {
            ++stats.comparisons;
            if (comp(*it, pivot_value)) {
                std::iter_swap(it, store);
                ++stats.swaps;
                ++store;
            }
        }
        std::iter_swap(store, last - 1);
        ++stats.swaps;
        return store;
    }

    // ============================================
    // MergeSort implementation
    // ============================================

    template <typename RandomIt, typename BufferIt, typename Compare>
    void merge_sort_impl(RandomIt first, RandomIt last, BufferIt buffer, Compare comp) {
        auto size = last - first;
        if (size <= static_cast<std::ptrdiff_t>(m_config.insertion_threshold)) {
            insertion_sort_impl(first, last, comp);
            return;
        }
        
        RandomIt mid = first + size / 2;
        merge_sort_impl(first, mid, buffer, comp);
        merge_sort_impl(mid, last, buffer + size / 2, comp);
        merge_impl(first, mid, last, buffer, comp);
    }

    template <typename RandomIt, typename BufferIt, typename Compare>
    void merge_sort_impl(RandomIt first, RandomIt last, BufferIt buffer, Compare comp, SortStats& stats) {
        auto size = last - first;
        if (size <= static_cast<std::ptrdiff_t>(m_config.insertion_threshold)) {
            insertion_sort_impl(first, last, comp, stats);
            return;
        }
        
        RandomIt mid = first + size / 2;
        merge_sort_impl(first, mid, buffer, comp, stats);
        merge_sort_impl(mid, last, buffer + size / 2, comp, stats);
        merge_impl(first, mid, last, buffer, comp, stats);
    }

    template <typename RandomIt, typename BufferIt, typename Compare>
    void merge_impl(RandomIt first, RandomIt mid, RandomIt last, BufferIt buffer, Compare comp) {
        RandomIt left = first, right = mid;
        BufferIt out = buffer;
        
        while (left != mid && right != last) {
            if (comp(*right, *left)) {
                *out++ = std::move(*right++);
            } else {
                *out++ = std::move(*left++);
            }
        }
        
        while (left != mid) *out++ = std::move(*left++);
        while (right != last) *out++ = std::move(*right++);
        
        std::move(buffer, buffer + (last - first), first);
    }

    template <typename RandomIt, typename BufferIt, typename Compare>
    void merge_impl(RandomIt first, RandomIt mid, RandomIt last, BufferIt buffer, Compare comp, SortStats& stats) {
        RandomIt left = first, right = mid;
        BufferIt out = buffer;
        
        while (left != mid && right != last) {
            ++stats.comparisons;
            if (comp(*right, *left)) {
                *out++ = std::move(*right++);
                ++stats.copies;
            } else {
                *out++ = std::move(*left++);
                ++stats.copies;
            }
        }
        
        while (left != mid) { *out++ = std::move(*left++); ++stats.copies; }
        while (right != last) { *out++ = std::move(*right++); ++stats.copies; }
        
        auto size = last - first;
        std::move(buffer, buffer + size, first);
        stats.copies += size;
    }

    // ============================================
    // HeapSort implementation
    // ============================================

    template <typename RandomIt, typename Compare>
    void heap_sort_impl(RandomIt first, RandomIt last, Compare comp) {
        auto size = last - first;
        
        // Build max heap
        for (auto i = size / 2; i > 0; --i) {
            sift_down(first, i - 1, size, comp);
        }
        
        // Extract elements
        for (auto i = size - 1; i > 0; --i) {
            std::iter_swap(first, first + i);
            sift_down(first, 0, i, comp);
        }
    }

    template <typename RandomIt, typename Compare>
    void heap_sort_impl(RandomIt first, RandomIt last, Compare comp, SortStats& stats) {
        auto size = last - first;
        
        for (auto i = size / 2; i > 0; --i) {
            sift_down(first, i - 1, size, comp, stats);
        }
        
        for (auto i = size - 1; i > 0; --i) {
            std::iter_swap(first, first + i);
            ++stats.swaps;
            sift_down(first, 0, i, comp, stats);
        }
    }

    template <typename RandomIt, typename Compare>
    void sift_down(RandomIt first, std::ptrdiff_t index, std::ptrdiff_t size, Compare comp) {
        while (2 * index + 1 < size) {
            std::ptrdiff_t child = 2 * index + 1;
            if (child + 1 < size && comp(*(first + child), *(first + child + 1))) {
                ++child;
            }
            if (!comp(*(first + index), *(first + child))) {
                break;
            }
            std::iter_swap(first + index, first + child);
            index = child;
        }
    }

    template <typename RandomIt, typename Compare>
    void sift_down(RandomIt first, std::ptrdiff_t index, std::ptrdiff_t size, Compare comp, SortStats& stats) {
        while (2 * index + 1 < size) {
            std::ptrdiff_t child = 2 * index + 1;
            ++stats.comparisons;
            if (child + 1 < size && comp(*(first + child), *(first + child + 1))) {
                ++child;
            }
            ++stats.comparisons;
            if (!comp(*(first + index), *(first + child))) {
                break;
            }
            std::iter_swap(first + index, first + child);
            ++stats.swaps;
            index = child;
        }
    }

    // ============================================
    // InsertionSort implementation
    // ============================================

    template <typename RandomIt, typename Compare>
    void insertion_sort_impl(RandomIt first, RandomIt last, Compare comp) {
        for (RandomIt i = first + 1; i != last; ++i) {
            auto key = std::move(*i);
            RandomIt j = i;
            while (j != first && comp(key, *(j - 1))) {
                *j = std::move(*(j - 1));
                --j;
            }
            *j = std::move(key);
        }
    }

    template <typename RandomIt, typename Compare>
    void insertion_sort_impl(RandomIt first, RandomIt last, Compare comp, SortStats& stats) {
        for (RandomIt i = first + 1; i != last; ++i) {
            auto key = std::move(*i);
            ++stats.copies;
            RandomIt j = i;
            while (j != first) {
                ++stats.comparisons;
                if (!comp(key, *(j - 1))) break;
                *j = std::move(*(j - 1));
                ++stats.copies;
                --j;
            }
            *j = std::move(key);
            ++stats.copies;
        }
    }

    // ============================================
    // Partial sort implementation
    // ============================================

    template <typename RandomIt, typename Compare>
    void partial_sort_impl(RandomIt first, RandomIt middle, RandomIt last, Compare comp, SortStats& stats) {
        // Build heap of first (middle - first) elements
        auto heap_size = middle - first;
        for (auto i = heap_size / 2; i > 0; --i) {
            sift_down(first, i - 1, heap_size, comp, stats);
        }
        
        // Compare rest with heap top, replace if smaller
        for (RandomIt it = middle; it != last; ++it) {
            ++stats.comparisons;
            if (comp(*it, *first)) {
                std::iter_swap(it, first);
                ++stats.swaps;
                sift_down(first, 0, heap_size, comp, stats);
            }
        }
        
        // Sort the heap
        heap_sort_impl(first, middle, comp, stats);
    }

    template <typename RandomIt, typename Compare>
    void nth_element_impl(RandomIt first, RandomIt nth, RandomIt last, Compare comp) {
        while (last - first > 3) {
            RandomIt pivot = median_of_three(first, first + (last - first) / 2, last - 1, comp);
            pivot = partition_impl(first, last, pivot, comp);
            
            if (pivot == nth) return;
            if (nth < pivot) {
                last = pivot;
            } else {
                first = pivot + 1;
            }
        }
        insertion_sort_impl(first, last, comp);
    }
};

// ============================================
// Free function interfaces
// ============================================

/**
 * @brief QuickSort with custom comparator
 */
template <typename RandomIt, typename Compare>
void quick_sort(RandomIt first, RandomIt last, Compare comp) {
    using T = typename std::iterator_traits<RandomIt>::value_type;
    Sorter<T>::with_compare(comp).quick_sort_range(first, last);
}

/**
 * @brief QuickSort with default comparator
 */
template <typename RandomIt>
void quick_sort(RandomIt first, RandomIt last) {
    using T = typename std::iterator_traits<RandomIt>::value_type;
    quick_sort(first, last, std::less<T>{});
}

/**
 * @brief MergeSort with custom comparator
 */
template <typename RandomIt, typename Compare>
void merge_sort(RandomIt first, RandomIt last, Compare comp) {
    using T = typename std::iterator_traits<RandomIt>::value_type;
    Sorter<T>::with_compare(comp).merge_sort_range(first, last);
}

/**
 * @brief MergeSort with default comparator
 */
template <typename RandomIt>
void merge_sort(RandomIt first, RandomIt last) {
    using T = typename std::iterator_traits<RandomIt>::value_type;
    merge_sort(first, last, std::less<T>{});
}

/**
 * @brief HeapSort with custom comparator
 */
template <typename RandomIt, typename Compare>
void heap_sort(RandomIt first, RandomIt last, Compare comp) {
    using T = typename std::iterator_traits<RandomIt>::value_type;
    Sorter<T>::with_compare(comp).heap_sort_range(first, last);
}

/**
 * @brief HeapSort with default comparator
 */
template <typename RandomIt>
void heap_sort(RandomIt first, RandomIt last) {
    using T = typename std::iterator_traits<RandomIt>::value_type;
    heap_sort(first, last, std::less<T>{});
}

/**
 * @brief InsertionSort with custom comparator
 */
template <typename RandomIt, typename Compare>
void insertion_sort(RandomIt first, RandomIt last, Compare comp) {
    using T = typename std::iterator_traits<RandomIt>::value_type;
    Sorter<T>::with_compare(comp).insertion_sort_range(first, last);
}

/**
 * @brief InsertionSort with default comparator
 */
template <typename RandomIt>
void insertion_sort(RandomIt first, RandomIt last) {
    using T = typename std::iterator_traits<RandomIt>::value_type;
    insertion_sort(first, last, std::less<T>{});
}

// ============================================
// Utility functions
// ============================================

/**
 * @brief Check if range is sorted
 */
template <typename RandomIt, typename Compare>
bool is_sorted(RandomIt first, RandomIt last, Compare comp) {
    return std::is_sorted(first, last, comp);
}

template <typename RandomIt>
bool is_sorted(RandomIt first, RandomIt last) {
    return std::is_sorted(first, last);
}

/**
 * @brief Shuffle range randomly
 */
template <typename RandomIt>
void shuffle(RandomIt first, RandomIt last) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::shuffle(first, last, gen);
}

/**
 * @brief Shuffle range with seed for reproducibility
 */
template <typename RandomIt>
void shuffle(RandomIt first, RandomIt last, unsigned int seed) {
    std::mt19937 gen(seed);
    std::shuffle(first, last, gen);
}

/**
 * @brief Reverse range in-place
 */
template <typename BidirIt>
void reverse(BidirIt first, BidirIt last) {
    std::reverse(first, last);
}

/**
 * @brief Rotate range so that middle becomes first
 */
template <typename ForwardIt>
void rotate(ForwardIt first, ForwardIt middle, ForwardIt last) {
    std::rotate(first, middle, last);
}

} // namespace algorithm
} // namespace mylib

#endif // MYLIB_ALGORITHM_SORTING_HPP