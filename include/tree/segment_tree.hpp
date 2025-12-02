/**
 * @file segment_tree.hpp
 * @brief Segment Tree data structure implementation
 * @author Jinhyeok
 * @date 2025-12-02
 * @version 1.0.0
 * 
 * This file provides a comprehensive Segment Tree implementation with:
 * - Range queries (sum, min, max, gcd, etc.) in O(log n)
 * - Point updates in O(log n)
 * - Range updates with lazy propagation in O(log n)
 * - Customizable merge operations
 * 
 * Segment Tree Properties:
 * - Binary tree stored as array
 * - Leaf nodes represent array elements
 * - Internal nodes represent merged results of ranges
 * - Height: O(log n)
 * 
 * Time Complexity:
 * - build: O(n)
 * - query: O(log n)
 * - update: O(log n)
 * - range_update: O(log n) with lazy propagation
 * 
 * Space Complexity: O(4n) = O(n)
 * 
 * Copyright (c) 2025 Jinhyeok
 * MIT License
 */

#ifndef MYLIB_TREE_SEGMENT_TREE_HPP
#define MYLIB_TREE_SEGMENT_TREE_HPP

#include <cstddef>
#include <vector>
#include <functional>
#include <stdexcept>
#include <limits>
#include <algorithm>
#include <numeric>
#include <optional>

namespace mylib {
namespace tree {

/**
 * @class SegmentTree
 * @brief A segment tree for efficient range queries and updates
 * 
 * Supports various operations through customizable merge functions.
 * Common operations: sum, min, max, gcd, lcm, etc.
 * 
 * @tparam T Value type
 * @tparam MergeOp Merge operation (default: std::plus<T> for sum)
 * 
 * Usage:
 * @code
 * // Range sum queries
 * std::vector<int> arr = {1, 3, 5, 7, 9, 11};
 * SegmentTree<int> tree(arr);  // Default: sum
 * 
 * int sum = tree.query(1, 4);  // Sum of arr[1..4] = 3+5+7+9 = 24
 * tree.update(2, 10);          // arr[2] = 10
 * 
 * // Range minimum queries
 * auto min_tree = SegmentTree<int>::create_min(arr);
 * int min_val = min_tree.query(1, 4);
 * 
 * // Custom merge operation
 * SegmentTree<int> custom_tree(arr, [](int a, int b) { 
 *     return std::gcd(a, b); 
 * }, 0);
 * @endcode
 */
template <typename T, typename MergeOp = std::plus<T>>
class SegmentTree {
private:
    std::vector<T> m_tree;      // Segment tree array
    std::vector<T> m_lazy;      // Lazy propagation array
    std::size_t m_size;         // Size of original array
    MergeOp m_merge;            // Merge operation
    T m_identity;               // Identity element for merge operation
    bool m_has_lazy;            // Whether lazy propagation is enabled
    
public:
    // ============================================
    // Constructors
    // ============================================
    
    /**
     * @brief Constructor from vector with custom merge operation
     * @param arr Initial array
     * @param merge Merge operation
     * @param identity Identity element (neutral element for merge)
     * @param enable_lazy Enable lazy propagation for range updates
     */
    SegmentTree(const std::vector<T>& arr, 
                MergeOp merge = MergeOp(),
                T identity = T(),
                bool enable_lazy = false)
        : m_size(arr.size()),
          m_merge(merge),
          m_identity(identity),
          m_has_lazy(enable_lazy) {
        
        if (m_size == 0) {
            throw std::invalid_argument("Array cannot be empty");
        }
        
        // Allocate space for segment tree (4 * n is safe upper bound)
        m_tree.resize(4 * m_size, m_identity);
        
        if (m_has_lazy) {
            m_lazy.resize(4 * m_size, T());
        }
        
        build(arr, 0, 0, m_size - 1);
    }
    
    /**
     * @brief Constructor with size and default value
     */
    SegmentTree(std::size_t size, 
                const T& default_value = T(),
                MergeOp merge = MergeOp(),
                T identity = T(),
                bool enable_lazy = false)
        : SegmentTree(std::vector<T>(size, default_value), merge, identity, enable_lazy) {
    }
    
    // ============================================
    // Query operations
    // ============================================
    
    /**
     * @brief Range query [left, right]
     * @param left Left index (inclusive)
     * @param right Right index (inclusive)
     * @return Merged result of range
     */
    T query(std::size_t left, std::size_t right) {
        if (left > right || right >= m_size) {
            throw std::out_of_range("Invalid query range");
        }
        return query(0, 0, m_size - 1, left, right);
    }
    
    /**
     * @brief Query single element
     */
    T query(std::size_t index) {
        return query(index, index);
    }
    
    /**
     * @brief Query entire array
     */
    T query_all() {
        return m_tree[0];
    }
    
    // ============================================
    // Update operations
    // ============================================
    
    /**
     * @brief Point update: set arr[index] = value
     * @param index Index to update
     * @param value New value
     */
    void update(std::size_t index, const T& value) {
        if (index >= m_size) {
            throw std::out_of_range("Index out of range");
        }
        update(0, 0, m_size - 1, index, value);
    }
    
    /**
     * @brief Range update: add value to range [left, right]
     * Requires lazy propagation to be enabled
     * @param left Left index (inclusive)
     * @param right Right index (inclusive)
     * @param value Value to add
     */
    void range_update(std::size_t left, std::size_t right, const T& value) {
        if (!m_has_lazy) {
            throw std::runtime_error("Lazy propagation not enabled");
        }
        if (left > right || right >= m_size) {
            throw std::out_of_range("Invalid update range");
        }
        range_update(0, 0, m_size - 1, left, right, value);
    }
    
    // ============================================
    // Utility
    // ============================================
    
    /**
     * @brief Get size of array
     */
    std::size_t size() const noexcept {
        return m_size;
    }
    
    /**
     * @brief Check if empty
     */
    bool empty() const noexcept {
        return m_size == 0;
    }
    
    /**
     * @brief Get identity element
     */
    T identity() const noexcept {
        return m_identity;
    }
    
    /**
     * @brief Check if lazy propagation is enabled
     */
    bool has_lazy_propagation() const noexcept {
        return m_has_lazy;
    }
    
    /**
     * @brief Convert segment tree to vector (reconstruct original array)
     */
    std::vector<T> to_vector() const {
        std::vector<T> result(m_size);
        for (std::size_t i = 0; i < m_size; ++i) {
            result[i] = const_cast<SegmentTree*>(this)->query(i, i);
        }
        return result;
    }
    
private:
    // ============================================
    // Build implementation
    // ============================================
    
    /**
     * @brief Build segment tree from array
     */
    void build(const std::vector<T>& arr, std::size_t node, 
               std::size_t start, std::size_t end) {
        if (start == end) {
            // Leaf node
            m_tree[node] = arr[start];
        } else {
            std::size_t mid = start + (end - start) / 2;
            std::size_t left_child = 2 * node + 1;
            std::size_t right_child = 2 * node + 2;
            
            build(arr, left_child, start, mid);
            build(arr, right_child, mid + 1, end);
            
            m_tree[node] = m_merge(m_tree[left_child], m_tree[right_child]);
        }
    }
    
    // ============================================
    // Query implementation
    // ============================================
    
    /**
     * @brief Recursive range query
     */
    T query(std::size_t node, std::size_t start, std::size_t end,
            std::size_t left, std::size_t right) {
        
        // Push lazy updates if needed
        if (m_has_lazy && m_lazy[node] != T()) {
            push_lazy(node, start, end);
        }
        
        // No overlap
        if (start > right || end < left) {
            return m_identity;
        }
        
        // Complete overlap
        if (start >= left && end <= right) {
            return m_tree[node];
        }
        
        // Partial overlap
        std::size_t mid = start + (end - start) / 2;
        std::size_t left_child = 2 * node + 1;
        std::size_t right_child = 2 * node + 2;
        
        T left_result = query(left_child, start, mid, left, right);
        T right_result = query(right_child, mid + 1, end, left, right);
        
        return m_merge(left_result, right_result);
    }
    
    // ============================================
    // Update implementation
    // ============================================
    
    /**
     * @brief Recursive point update
     */
    void update(std::size_t node, std::size_t start, std::size_t end,
                std::size_t index, const T& value) {
        
        if (start == end) {
            // Leaf node
            m_tree[node] = value;
        } else {
            std::size_t mid = start + (end - start) / 2;
            std::size_t left_child = 2 * node + 1;
            std::size_t right_child = 2 * node + 2;
            
            if (index <= mid) {
                update(left_child, start, mid, index, value);
            } else {
                update(right_child, mid + 1, end, index, value);
            }
            
            m_tree[node] = m_merge(m_tree[left_child], m_tree[right_child]);
        }
    }
    
    /**
     * @brief Recursive range update with lazy propagation
     */
    void range_update(std::size_t node, std::size_t start, std::size_t end,
                      std::size_t left, std::size_t right, const T& value) {
        
        // Push existing lazy updates
        if (m_lazy[node] != T()) {
            push_lazy(node, start, end);
        }
        
        // No overlap
        if (start > right || end < left) {
            return;
        }
        
        // Complete overlap
        if (start >= left && end <= right) {
            m_lazy[node] += value;
            push_lazy(node, start, end);
            return;
        }
        
        // Partial overlap
        std::size_t mid = start + (end - start) / 2;
        std::size_t left_child = 2 * node + 1;
        std::size_t right_child = 2 * node + 2;
        
        range_update(left_child, start, mid, left, right, value);
        range_update(right_child, mid + 1, end, left, right, value);
        
        m_tree[node] = m_merge(m_tree[left_child], m_tree[right_child]);
    }
    
    /**
     * @brief Push lazy value to children
     */
    void push_lazy(std::size_t node, std::size_t start, std::size_t end) {
        if (m_lazy[node] == T()) return;
        
        // Update current node
        std::size_t range_size = end - start + 1;
        m_tree[node] += m_lazy[node] * static_cast<T>(range_size);
        
        // Propagate to children if not leaf
        if (start != end) {
            std::size_t left_child = 2 * node + 1;
            std::size_t right_child = 2 * node + 2;
            
            m_lazy[left_child] += m_lazy[node];
            m_lazy[right_child] += m_lazy[node];
        }
        
        // Clear lazy value
        m_lazy[node] = T();
    }
};

// ============================================
// Specialized Segment Trees
// ============================================

/**
 * @class RangeSumSegmentTree
 * @brief Segment tree specialized for range sum queries
 */
template <typename T>
class RangeSumSegmentTree : public SegmentTree<T, std::plus<T>> {
public:
    explicit RangeSumSegmentTree(const std::vector<T>& arr, bool enable_lazy = true)
        : SegmentTree<T, std::plus<T>>(arr, std::plus<T>(), T(), enable_lazy) {
    }
    
    RangeSumSegmentTree(std::size_t size, const T& default_value = T())
        : SegmentTree<T, std::plus<T>>(size, default_value, std::plus<T>(), T(), true) {
    }
};

/**
 * @class RangeMinSegmentTree
 * @brief Segment tree specialized for range minimum queries
 */
template <typename T>
class RangeMinSegmentTree : public SegmentTree<T, std::function<T(const T&, const T&)>> {
public:
    explicit RangeMinSegmentTree(const std::vector<T>& arr)
        : SegmentTree<T, std::function<T(const T&, const T&)>>(
            arr,
            [](const T& a, const T& b) { return std::min(a, b); },
            std::numeric_limits<T>::max(),
            false) {
    }
};

/**
 * @class RangeMaxSegmentTree
 * @brief Segment tree specialized for range maximum queries
 */
template <typename T>
class RangeMaxSegmentTree : public SegmentTree<T, std::function<T(const T&, const T&)>> {
public:
    explicit RangeMaxSegmentTree(const std::vector<T>& arr)
        : SegmentTree<T, std::function<T(const T&, const T&)>>(
            arr,
            [](const T& a, const T& b) { return std::max(a, b); },
            std::numeric_limits<T>::lowest(),
            false) {
    }
};

/**
 * @class RangeGCDSegmentTree
 * @brief Segment tree specialized for range GCD queries
 */
template <typename T>
class RangeGCDSegmentTree : public SegmentTree<T, std::function<T(const T&, const T&)>> {
public:
    explicit RangeGCDSegmentTree(const std::vector<T>& arr)
        : SegmentTree<T, std::function<T(const T&, const T&)>>(
            arr,
            [](const T& a, const T& b) { return std::gcd(a, b); },
            T(),
            false) {
    }
};

// ============================================
// Factory functions
// ============================================

/**
 * @brief Create segment tree for range sum queries
 */
template <typename T>
SegmentTree<T, std::plus<T>> create_sum_segment_tree(const std::vector<T>& arr, bool enable_lazy = false) {
    return SegmentTree<T, std::plus<T>>(arr, std::plus<T>(), T(), enable_lazy);
}

/**
 * @brief Create segment tree for range minimum queries
 */
template <typename T>
auto create_min_segment_tree(const std::vector<T>& arr) {
    using FuncType = std::function<T(const T&, const T&)>;
    return SegmentTree<T, FuncType>(
        arr,
        [](const T& a, const T& b) { return std::min(a, b); },
        std::numeric_limits<T>::max(),
        false);
}

/**
 * @brief Create segment tree for range maximum queries
 */
template <typename T>
auto create_max_segment_tree(const std::vector<T>& arr) {
    using FuncType = std::function<T(const T&, const T&)>;
    return SegmentTree<T, FuncType>(
        arr,
        [](const T& a, const T& b) { return std::max(a, b); },
        std::numeric_limits<T>::lowest(),
        false);
}

/**
 * @brief Create segment tree for GCD queries
 */
template <typename T>
auto create_gcd_segment_tree(const std::vector<T>& arr) {
    using FuncType = std::function<T(const T&, const T&)>;
    return SegmentTree<T, FuncType>(
        arr,
        [](const T& a, const T& b) { return std::gcd(a, b); },
        T(),
        false);
}

} // namespace tree
} // namespace mylib

#endif // MYLIB_TREE_SEGMENT_TREE_HPP