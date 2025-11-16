/**
 * @file dynamic_array.hpp
 * @brief Dynamic array implementation with automatic memory management
 * @author Jinhyeok
 * @date 2025-11-15
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

#ifndef MYLIB_LINEAR_DYNAMIC_ARRAY_HPP
#define MYLIB_LINEAR_DYNAMIC_ARRAY_HPP

#include <cstddef>      // for size_t
#include <stdexcept>    // for std::out_of_range
#include <utility>      // for std::move
#include <initializer_list>

namespace mylib {
namespace linear {

/**
 * @class DynamicArray
 * @brief A dynamic array container that automatically grows as needed
 * 
 * This class provides a dynamic array implementation similar to std::vector,
 * with automatic memory management and capacity growth.
 * 
 * @tparam T The type of elements stored in the array
 */
template <typename T>
class DynamicArray {
public:
    // Type aliases
    using value_type = T;
    using size_type = std::size_t;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using const_pointer = const T*;

    /**
     * @brief Default constructor
     * Creates an empty dynamic array with initial capacity
     */
    DynamicArray();

    /**
     * @brief Constructor with initial capacity
     * @param initial_capacity Initial capacity to reserve
     */
    explicit DynamicArray(size_type initial_capacity);

    /**
     * @brief Constructor with size and default value
     * @param count Number of elements
     * @param value Value to initialize elements with
     */
    DynamicArray(size_type count, const T& value);

    /**
     * @brief Initializer list constructor
     * @param init Initializer list
     */
    DynamicArray(std::initializer_list<T> init);

    /**
     * @brief Copy constructor
     * @param other Array to copy from
     */
    DynamicArray(const DynamicArray& other);

    /**
     * @brief Move constructor
     * @param other Array to move from
     */
    DynamicArray(DynamicArray&& other) noexcept;

    /**
     * @brief Destructor
     */
    ~DynamicArray();

    /**
     * @brief Copy assignment operator
     * @param other Array to copy from
     * @return Reference to this array
     */
    DynamicArray& operator=(const DynamicArray& other);

    /**
     * @brief Move assignment operator
     * @param other Array to move from
     * @return Reference to this array
     */
    DynamicArray& operator=(DynamicArray&& other) noexcept;

    // Element access
    /**
     * @brief Access element at index with bounds checking
     * @param index Position of element
     * @return Reference to element
     * @throws std::out_of_range if index is out of bounds
     */
    reference at(size_type index);
    const_reference at(size_type index) const;

    /**
     * @brief Access element at index without bounds checking
     * @param index Position of element
     * @return Reference to element
     */
    reference operator[](size_type index);
    const_reference operator[](size_type index) const;

    /**
     * @brief Access the first element
     * @return Reference to first element
     */
    reference front();
    const_reference front() const;

    /**
     * @brief Access the last element
     * @return Reference to last element
     */
    reference back();
    const_reference back() const;

    /**
     * @brief Direct access to underlying array
     * @return Pointer to underlying array
     */
    pointer data() noexcept;
    const_pointer data() const noexcept;

    // Capacity
    /**
     * @brief Check if array is empty
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
     * @return Current capacity
     */
    size_type capacity() const noexcept;

    /**
     * @brief Reserve capacity for at least new_capacity elements
     * @param new_capacity Minimum capacity to reserve
     */
    void reserve(size_type new_capacity);

    /**
     * @brief Reduce capacity to fit size
     */
    void shrink_to_fit();

    // Modifiers
    /**
     * @brief Clear all elements
     */
    void clear() noexcept;

    /**
     * @brief Add element to end
     * @param value Value to add
     */
    void push_back(const T& value);

    /**
     * @brief Add element to end (move version)
     * @param value Value to move
     */
    void push_back(T&& value);

    /**
     * @brief Remove last element
     */
    void pop_back();

    /**
     * @brief Resize array to contain count elements
     * @param count New size
     */
    void resize(size_type count);

    /**
     * @brief Resize array with default value
     * @param count New size
     * @param value Value for new elements
     */
    void resize(size_type count, const T& value);

    /**
     * @brief Swap contents with another array
     * @param other Array to swap with
     */
    void swap(DynamicArray& other) noexcept;

private:
    pointer m_data;           ///< Pointer to array data
    size_type m_size;         ///< Current number of elements
    size_type m_capacity;     ///< Current capacity

    static constexpr size_type DEFAULT_CAPACITY = 16;
    static constexpr double GROWTH_FACTOR = 2.0;

    /**
     * @brief Grow capacity when needed
     */
    void grow();

    /**
     * @brief Reallocate with new capacity
     * @param new_capacity New capacity
     */
    void reallocate(size_type new_capacity);
};

} // namespace linear
} // namespace mylib

#endif // MYLIB_LINEAR_DYNAMIC_ARRAY_HPP
