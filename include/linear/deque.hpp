/**
 * @file deque.hpp
 * @brief Double-ended queue implementation using LinkedList as underlying container
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

#ifndef MYLIB_LINEAR_DEQUE_HPP
#define MYLIB_LINEAR_DEQUE_HPP

#include "linear/linked_list.hpp"
#include <cstddef>
#include <stdexcept>
#include <utility>
#include <initializer_list>

namespace mylib {
namespace linear {

/**
 * @class Deque
 * @brief A double-ended queue container
 * 
 * This class implements a deque (double-ended queue) data structure using
 * LinkedList as the underlying container. It provides O(1) insertion and
 * deletion at both ends.
 * 
 * @tparam T The type of elements stored in the deque
 */
template <typename T>
class Deque {
public:
    // Type aliases
    using value_type = T;
    using size_type = std::size_t;
    using reference = T&;
    using const_reference = const T&;

    /**
     * @brief Default constructor
     * Creates an empty deque
     */
    Deque();

    /**
     * @brief Constructor with count and default value
     * @param count Number of elements
     * @param value Value to initialize elements with
     */
    Deque(size_type count, const T& value);

    /**
     * @brief Initializer list constructor
     * @param init Initializer list
     */
    Deque(std::initializer_list<T> init);

    /**
     * @brief Copy constructor
     * @param other Deque to copy from
     */
    Deque(const Deque& other);

    /**
     * @brief Move constructor
     * @param other Deque to move from
     */
    Deque(Deque&& other) noexcept;

    /**
     * @brief Destructor
     */
    ~Deque() = default;

    /**
     * @brief Copy assignment operator
     * @param other Deque to copy from
     * @return Reference to this deque
     */
    Deque& operator=(const Deque& other);

    /**
     * @brief Move assignment operator
     * @param other Deque to move from
     * @return Reference to this deque
     */
    Deque& operator=(Deque&& other) noexcept;

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
     * @throws std::out_of_range if deque is empty
     */
    reference front();
    const_reference front() const;

    /**
     * @brief Access the last element
     * @return Reference to last element
     * @throws std::out_of_range if deque is empty
     */
    reference back();
    const_reference back() const;

    // Capacity
    /**
     * @brief Check if deque is empty
     * @return true if empty, false otherwise
     */
    bool empty() const noexcept;

    /**
     * @brief Get number of elements
     * @return Number of elements
     */
    size_type size() const noexcept;

    // Modifiers
    /**
     * @brief Clear all elements
     */
    void clear() noexcept;

    /**
     * @brief Add element to front
     * @param value Value to add
     */
    void push_front(const T& value);

    /**
     * @brief Add element to front (move version)
     * @param value Value to move
     */
    void push_front(T&& value);

    /**
     * @brief Add element to back
     * @param value Value to add
     */
    void push_back(const T& value);

    /**
     * @brief Add element to back (move version)
     * @param value Value to move
     */
    void push_back(T&& value);

    /**
     * @brief Remove first element
     * @throws std::out_of_range if deque is empty
     */
    void pop_front();

    /**
     * @brief Remove last element
     * @throws std::out_of_range if deque is empty
     */
    void pop_back();

    /**
     * @brief Insert element at position
     * @param index Position to insert at
     * @param value Value to insert
     * @throws std::out_of_range if index is out of bounds
     */
    void insert(size_type index, const T& value);

    /**
     * @brief Insert element at position (move version)
     * @param index Position to insert at
     * @param value Value to move
     * @throws std::out_of_range if index is out of bounds
     */
    void insert(size_type index, T&& value);

    /**
     * @brief Erase element at position
     * @param index Position to erase
     * @throws std::out_of_range if index is out of bounds
     */
    void erase(size_type index);

    /**
     * @brief Resize deque to contain count elements
     * @param count New size
     */
    void resize(size_type count);

    /**
     * @brief Resize deque with default value
     * @param count New size
     * @param value Value for new elements
     */
    void resize(size_type count, const T& value);

    /**
     * @brief Swap contents with another deque
     * @param other Deque to swap with
     */
    void swap(Deque& other) noexcept;

    /**
     * @brief Reverse the deque
     */
    void reverse() noexcept;

    /**
     * @brief Remove all elements with specific value
     * @param value Value to remove
     * @return Number of elements removed
     */
    size_type remove(const T& value);

    /**
     * @brief Find first occurrence of value
     * @param value Value to find
     * @return Index of first occurrence, or size() if not found
     */
    size_type find(const T& value) const;

    /**
     * @brief Check if value exists in deque
     * @param value Value to check
     * @return true if value exists, false otherwise
     */
    bool contains(const T& value) const;

private:
    LinkedList<T> m_container;  ///< Underlying container
};

} // namespace linear
} // namespace mylib

#endif // MYLIB_LINEAR_DEQUE_HPP