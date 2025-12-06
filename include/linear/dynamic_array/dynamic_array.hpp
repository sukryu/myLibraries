/**
 * @file dynamic_array.hpp
 * @brief Dynamic array container with automatic memory management
 * @author Jinhyeok
 * @date 2025-12-06
 * @version 2.0.0
 * 
 * Phase 1 Optimizations Applied:
 * - ✅ Move semantics (already present)
 * - ✅ Iterator support (NEW)
 * - ✅ emplace_back (NEW)
 * - ✅ noexcept specifications (ENHANCED)
 * - ✅ Growth factor optimization (2.0 → 1.5)
 * 
 * Copyright (c) 2025 Jinhyeok
 * Licensed under MIT License
 */

#ifndef MYLIB_LINEAR_DYNAMIC_ARRAY_HPP
#define MYLIB_LINEAR_DYNAMIC_ARRAY_HPP

#include "dynamic_array_fwd.hpp"
#include <cstddef>
#include <stdexcept>
#include <utility>
#include <initializer_list>
#include <iterator>
#include <algorithm>
#include <memory>

namespace mylib {
namespace linear {

/**
 * @class DynamicArray
 * @brief STL-compatible dynamic array with automatic growth
 * 
 * A high-performance dynamic array container that provides:
 * - Automatic memory management
 * - Move semantics for efficiency
 * - STL-compatible iterators
 * - Exception safety guarantees
 * - Optimized growth strategy (φ ≈ 1.5)
 * 
 * @tparam T Element type (must be move-constructible)
 * 
 * Performance characteristics:
 * - Access: O(1)
 * - Insertion at end: O(1) amortized
 * - Insertion at arbitrary position: O(n)
 * - Space complexity: O(n)
 */
template <typename T>
class DynamicArray {
public:
    // ============================================
    // Type Aliases (STL Compatibility)
    // ============================================
    
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using const_pointer = const T*;
    
    // Iterator types
    using iterator = T*;
    using const_iterator = const T*;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    
    // ============================================
    // Constructors & Destructor
    // ============================================
    
    /**
     * @brief Default constructor - creates empty array
     * @complexity O(1)
     * @exception noexcept
     */
    DynamicArray() noexcept;
    
    /**
     * @brief Reserve constructor - pre-allocates capacity
     * @param initial_capacity Capacity to reserve
     * @complexity O(n) where n = initial_capacity
     * @exception Strong guarantee
     */
    explicit DynamicArray(size_type initial_capacity);
    
    /**
     * @brief Fill constructor - creates array with count copies
     * @param count Number of elements
     * @param value Value to copy
     * @complexity O(n) where n = count
     * @exception Strong guarantee
     */
    DynamicArray(size_type count, const T& value);
    
    /**
     * @brief Initializer list constructor
     * @param init Initializer list
     * @complexity O(n) where n = init.size()
     * @exception Strong guarantee
     * 
     * Usage: DynamicArray<int> arr = {1, 2, 3, 4, 5};
     */
    DynamicArray(std::initializer_list<T> init);
    
    /**
     * @brief Copy constructor
     * @param other Array to copy from
     * @complexity O(n)
     * @exception Strong guarantee
     */
    DynamicArray(const DynamicArray& other);
    
    /**
     * @brief Move constructor
     * @param other Array to move from (left in valid but unspecified state)
     * @complexity O(1)
     * @exception noexcept
     */
    DynamicArray(DynamicArray&& other) noexcept;
    
    /**
     * @brief Destructor - releases all resources
     * @complexity O(n)
     * @exception noexcept
     */
    ~DynamicArray() noexcept;
    
    // ============================================
    // Assignment Operators
    // ============================================
    
    /**
     * @brief Copy assignment
     * @param other Array to copy from
     * @return *this
     * @complexity O(n)
     * @exception Strong guarantee
     */
    DynamicArray& operator=(const DynamicArray& other);
    
    /**
     * @brief Move assignment
     * @param other Array to move from
     * @return *this
     * @complexity O(1)
     * @exception noexcept
     */
    DynamicArray& operator=(DynamicArray&& other) noexcept;
    
    /**
     * @brief Initializer list assignment
     * @param init Initializer list
     * @return *this
     * @complexity O(n)
     * @exception Strong guarantee
     */
    DynamicArray& operator=(std::initializer_list<T> init);
    
    // ============================================
    // Element Access
    // ============================================
    
    /**
     * @brief Bounds-checked element access
     * @param index Position of element
     * @return Reference to element
     * @throws std::out_of_range if index >= size()
     * @complexity O(1)
     */
    reference at(size_type index);
    const_reference at(size_type index) const;
    
    /**
     * @brief Unchecked element access
     * @param index Position of element
     * @return Reference to element
     * @complexity O(1)
     * @exception noexcept
     * @warning Undefined behavior if index >= size()
     */
    reference operator[](size_type index) noexcept;
    const_reference operator[](size_type index) const noexcept;
    
    /**
     * @brief Access first element
     * @return Reference to first element
     * @complexity O(1)
     * @exception noexcept
     * @pre !empty()
     */
    reference front() noexcept;
    const_reference front() const noexcept;
    
    /**
     * @brief Access last element
     * @return Reference to last element
     * @complexity O(1)
     * @exception noexcept
     * @pre !empty()
     */
    reference back() noexcept;
    const_reference back() const noexcept;
    
    /**
     * @brief Direct access to underlying array
     * @return Pointer to data
     * @complexity O(1)
     * @exception noexcept
     */
    pointer data() noexcept;
    const_pointer data() const noexcept;
    
    // ============================================
    // Iterators
    // ============================================
    
    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    const_iterator cbegin() const noexcept;
    
    iterator end() noexcept;
    const_iterator end() const noexcept;
    const_iterator cend() const noexcept;
    
    reverse_iterator rbegin() noexcept;
    const_reverse_iterator rbegin() const noexcept;
    const_reverse_iterator crbegin() const noexcept;
    
    reverse_iterator rend() noexcept;
    const_reverse_iterator rend() const noexcept;
    const_reverse_iterator crend() const noexcept;
    
    // ============================================
    // Capacity
    // ============================================
    
    /**
     * @brief Check if array is empty
     * @return true if size() == 0
     * @complexity O(1)
     * @exception noexcept
     */
    bool empty() const noexcept;
    
    /**
     * @brief Get number of elements
     * @return Number of elements
     * @complexity O(1)
     * @exception noexcept
     */
    size_type size() const noexcept;
    
    /**
     * @brief Get current capacity
     * @return Current capacity
     * @complexity O(1)
     * @exception noexcept
     */
    size_type capacity() const noexcept;
    
    /**
     * @brief Reserve capacity for at least new_capacity elements
     * @param new_capacity Minimum capacity
     * @complexity O(n) if reallocation occurs
     * @exception Strong guarantee
     * @note Does nothing if new_capacity <= capacity()
     */
    void reserve(size_type new_capacity);
    
    /**
     * @brief Reduce capacity to fit size
     * @complexity O(n)
     * @exception Strong guarantee
     * @note Non-binding request (may do nothing)
     */
    void shrink_to_fit();
    
    // ============================================
    // Modifiers
    // ============================================
    
    /**
     * @brief Clear all elements (capacity unchanged)
     * @complexity O(n)
     * @exception noexcept
     */
    void clear() noexcept;
    
    /**
     * @brief Add element to end (copy)
     * @param value Element to copy
     * @complexity O(1) amortized
     * @exception Strong guarantee
     */
    void push_back(const T& value);
    
    /**
     * @brief Add element to end (move)
     * @param value Element to move
     * @complexity O(1) amortized
     * @exception Strong guarantee if T's move constructor doesn't throw
     * 
     * Example:
     *   arr.push_back(std::string("hello"));  // Moves temporary
     */
    void push_back(T&& value);
    
    /**
     * @brief Construct element in-place at end
     * @tparam Args Constructor argument types
     * @param args Arguments to forward to constructor
     * @return Reference to constructed element
     * @complexity O(1) amortized
     * @exception Strong guarantee if T's constructor doesn't throw
     * 
     * Example:
     *   arr.emplace_back(10, 'x');  // Constructs directly, no copy/move
     */
    template <typename... Args>
    reference emplace_back(Args&&... args);
    
    /**
     * @brief Remove last element
     * @complexity O(1)
     * @exception noexcept
     * @pre !empty()
     */
    void pop_back() noexcept;
    
    /**
     * @brief Resize to contain count elements
     * @param count New size
     * @complexity O(n)
     * @exception Strong guarantee
     * @note New elements are default-initialized
     */
    void resize(size_type count);
    
    /**
     * @brief Resize with default value for new elements
     * @param count New size
     * @param value Value for new elements
     * @complexity O(n)
     * @exception Strong guarantee
     */
    void resize(size_type count, const T& value);
    
    /**
     * @brief Swap contents with another array
     * @param other Array to swap with
     * @complexity O(1)
     * @exception noexcept
     */
    void swap(DynamicArray& other) noexcept;
    
    // ============================================
    // Comparison Operators
    // ============================================
    
    bool operator==(const DynamicArray& other) const;
    bool operator!=(const DynamicArray& other) const;
    bool operator<(const DynamicArray& other) const;
    bool operator<=(const DynamicArray& other) const;
    bool operator>(const DynamicArray& other) const;
    bool operator>=(const DynamicArray& other) const;
    
private:
    // ============================================
    // Member Variables
    // ============================================
    
    pointer m_data;           ///< Pointer to dynamic array
    size_type m_size;         ///< Number of elements
    size_type m_capacity;     ///< Allocated capacity
    
    // ============================================
    // Constants
    // ============================================
    
    static constexpr size_type DEFAULT_CAPACITY = 16;
    static constexpr double GROWTH_FACTOR = 1.5;  // Optimized from 2.0
    
    // ============================================
    // Helper Methods
    // ============================================
    
    /**
     * @brief Calculate new capacity with growth factor
     * @param current Current capacity
     * @param required Minimum required capacity
     * @return New capacity
     */
    size_type calculate_growth(size_type current, size_type required) const noexcept;
    
    /**
     * @brief Grow capacity when needed (doubles or uses GROWTH_FACTOR)
     */
    void grow();
    
    /**
     * @brief Reallocate with new capacity
     * @param new_capacity New capacity
     */
    void reallocate(size_type new_capacity);
    
    /**
     * @brief Destroy range of elements
     * @param first First element to destroy
     * @param last One past last element to destroy
     */
    void destroy_range(pointer first, pointer last) noexcept;
};

// ============================================
// Non-member Functions
// ============================================

/**
 * @brief Swap two arrays
 * @param lhs First array
 * @param rhs Second array
 * @complexity O(1)
 * @exception noexcept
 */
template <typename T>
void swap(DynamicArray<T>& lhs, DynamicArray<T>& rhs) noexcept;

} // namespace linear
} // namespace mylib

// Include implementation
#include "dynamic_array_impl.hpp"

#endif // MYLIB_LINEAR_DYNAMIC_ARRAY_HPP