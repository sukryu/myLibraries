/**
 * @file heap.hpp
 * @brief Binary Heap implementation (Max Heap and Min Heap)
 * @author Jinhyeok
 * @date 2025-11-29
 * @version 1.0.0
 * 
 * Copyright (c) 2025 Jinhyeok
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef MYLIB_TREE_HEAP_HPP
#define MYLIB_TREE_HEAP_HPP

#include <cstddef>
#include <stdexcept>
#include <utility>
#include <initializer_list>
#include <functional>
#include <vector>

namespace mylib {
namespace tree {

/**
 * @class Heap
 * @brief A binary heap implementation using array representation
 * 
 * This class implements a binary heap data structure stored in a contiguous
 * array. By default, it creates a max heap where the largest element is at
 * the root. A custom comparator can be provided to create a min heap or
 * use custom ordering.
 * 
 * The heap satisfies the heap property: for a max heap, every parent node
 * is greater than or equal to its children.
 * 
 * Array representation:
 * - Parent of node at index i: (i - 1) / 2
 * - Left child of node at index i: 2 * i + 1
 * - Right child of node at index i: 2 * i + 2
 * 
 * @tparam T The type of elements stored in the heap
 * @tparam Compare Comparison function object type (default: std::less<T> for max heap)
 */
template <typename T, typename Compare = std::less<T>>
class Heap {
public:
    // Type aliases
    using value_type = T;
    using size_type = std::size_t;
    using reference = T&;
    using const_reference = const T&;
    using comparator_type = Compare;

    /**
     * @brief Default constructor
     * Creates an empty heap
     */
    Heap();

    /**
     * @brief Constructor with custom comparator
     * @param comp Comparator to use for ordering
     */
    explicit Heap(const Compare& comp);

    /**
     * @brief Constructor with initial capacity
     * @param initial_capacity Capacity to reserve
     */
    explicit Heap(size_type initial_capacity);

    /**
     * @brief Constructor with initial capacity and comparator
     * @param initial_capacity Capacity to reserve
     * @param comp Comparator to use
     */
    Heap(size_type initial_capacity, const Compare& comp);

    /**
     * @brief Initializer list constructor
     * @param init Initializer list of elements
     * 
     * Uses heapify algorithm for O(n) construction.
     */
    Heap(std::initializer_list<T> init);

    /**
     * @brief Initializer list constructor with comparator
     * @param init Initializer list of elements
     * @param comp Comparator to use
     */
    Heap(std::initializer_list<T> init, const Compare& comp);

    /**
     * @brief Range constructor
     * @param first Iterator to first element
     * @param last Iterator past last element
     */
    template <typename InputIt>
    Heap(InputIt first, InputIt last);

    /**
     * @brief Range constructor with comparator
     * @param first Iterator to first element
     * @param last Iterator past last element
     * @param comp Comparator to use
     */
    template <typename InputIt>
    Heap(InputIt first, InputIt last, const Compare& comp);

    /**
     * @brief Copy constructor
     * @param other Heap to copy from
     */
    Heap(const Heap& other);

    /**
     * @brief Move constructor
     * @param other Heap to move from
     */
    Heap(Heap&& other) noexcept;

    /**
     * @brief Destructor
     */
    ~Heap() = default;

    /**
     * @brief Copy assignment operator
     * @param other Heap to copy from
     * @return Reference to this heap
     */
    Heap& operator=(const Heap& other);

    /**
     * @brief Move assignment operator
     * @param other Heap to move from
     * @return Reference to this heap
     */
    Heap& operator=(Heap&& other) noexcept;

    // Capacity
    /**
     * @brief Check if heap is empty
     * @return true if empty, false otherwise
     */
    bool empty() const noexcept;

    /**
     * @brief Get number of elements
     * @return Number of elements
     */
    size_type size() const noexcept;

    /**
     * @brief Get current capacity
     * @return Current capacity of underlying storage
     */
    size_type capacity() const noexcept;

    /**
     * @brief Reserve capacity
     * @param new_capacity Minimum capacity to reserve
     */
    void reserve(size_type new_capacity);

    // Element access
    /**
     * @brief Access the top element (max for max-heap, min for min-heap)
     * @return Const reference to top element
     * @throws std::out_of_range if heap is empty
     */
    const_reference top() const;

    // Modifiers
    /**
     * @brief Insert an element into the heap
     * @param value Value to insert
     * 
     * Time complexity: O(log n)
     */
    void push(const T& value);

    /**
     * @brief Insert an element into the heap (move version)
     * @param value Value to move into heap
     */
    void push(T&& value);

    /**
     * @brief Construct element in-place
     * @param args Arguments to forward to element constructor
     */
    template <typename... Args>
    void emplace(Args&&... args);

    /**
     * @brief Remove the top element
     * @throws std::out_of_range if heap is empty
     * 
     * Time complexity: O(log n)
     */
    void pop();

    /**
     * @brief Remove and return the top element
     * @return The top element
     * @throws std::out_of_range if heap is empty
     */
    T extract();

    /**
     * @brief Clear all elements
     */
    void clear() noexcept;

    /**
     * @brief Swap contents with another heap
     * @param other Heap to swap with
     */
    void swap(Heap& other) noexcept;

    // Heap operations
    /**
     * @brief Check if the heap property is satisfied
     * @return true if valid heap, false otherwise
     */
    bool is_valid() const;

    /**
     * @brief Get the underlying data (for debugging/testing)
     * @return Const reference to underlying vector
     */
    const std::vector<T>& data() const noexcept;

private:
    std::vector<T> m_data;    ///< Underlying storage
    Compare m_comp;           ///< Comparator for ordering

    // Index calculations
    /**
     * @brief Get parent index
     * @param index Child index
     * @return Parent index
     */
    static size_type parent(size_type index) noexcept;

    /**
     * @brief Get left child index
     * @param index Parent index
     * @return Left child index
     */
    static size_type left_child(size_type index) noexcept;

    /**
     * @brief Get right child index
     * @param index Parent index
     * @return Right child index
     */
    static size_type right_child(size_type index) noexcept;

    /**
     * @brief Check if index has parent
     * @param index Node index
     * @return true if has parent
     */
    static bool has_parent(size_type index) noexcept;

    /**
     * @brief Check if index has left child
     * @param index Node index
     * @return true if has left child
     */
    bool has_left_child(size_type index) const noexcept;

    /**
     * @brief Check if index has right child
     * @param index Node index
     * @return true if has right child
     */
    bool has_right_child(size_type index) const noexcept;

    // Heap maintenance
    /**
     * @brief Restore heap property by moving element up
     * @param index Starting index
     * 
     * Used after insertion.
     */
    void sift_up(size_type index);

    /**
     * @brief Restore heap property by moving element down
     * @param index Starting index
     * 
     * Used after removal.
     */
    void sift_down(size_type index);

    /**
     * @brief Build heap from unordered data
     * 
     * Time complexity: O(n)
     */
    void heapify();

    /**
     * @brief Compare two elements using comparator
     * @param a First element
     * @param b Second element
     * @return true if a should be below b in heap
     * 
     * For max heap with std::less: returns true if a < b
     */
    bool compare(const T& a, const T& b) const;
};

/**
 * @brief Type alias for max heap
 */
template <typename T>
using MaxHeap = Heap<T, std::less<T>>;

/**
 * @brief Type alias for min heap
 */
template <typename T>
using MinHeap = Heap<T, std::greater<T>>;

// ============================================
// Template member function implementations
// (Must be in header for template instantiation)
// ============================================

template <typename T, typename Compare>
template <typename InputIt>
Heap<T, Compare>::Heap(InputIt first, InputIt last)
    : m_data(first, last), m_comp() {
    heapify();
}

template <typename T, typename Compare>
template <typename InputIt>
Heap<T, Compare>::Heap(InputIt first, InputIt last, const Compare& comp)
    : m_data(first, last), m_comp(comp) {
    heapify();
}

template <typename T, typename Compare>
template <typename... Args>
void Heap<T, Compare>::emplace(Args&&... args) {
    m_data.emplace_back(std::forward<Args>(args)...);
    sift_up(m_data.size() - 1);
}

} // namespace tree
} // namespace mylib

#endif // MYLIB_TREE_HEAP_HPP