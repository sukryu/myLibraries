/**
 * @file stack.hpp
 * @brief Stack implementation using DynamicArray as underlying container
 * @author Jinhyeok
 * @date 2025-11-16
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

#ifndef MYLIB_LINEAR_STACK_HPP
#define MYLIB_LINEAR_STACK_HPP

#include "linear/dynamic_array.hpp"
#include <cstddef>
#include <stdexcept>
#include <utility>

namespace mylib {
namespace linear {

/**
 * @class Stack
 * @brief A LIFO (Last-In-First-Out) stack container
 * 
 * This class implements a stack data structure using DynamicArray as the
 * underlying container. It provides O(1) push and pop operations.
 * 
 * @tparam T The type of elements stored in the stack
 */
template <typename T>
class Stack {
public:
    // Type aliases
    using value_type = T;
    using size_type = std::size_t;
    using reference = T&;
    using const_reference = const T&;

    /**
     * @brief Default constructor
     * Creates an empty stack
     */
    Stack();

    /**
     * @brief Constructor with initial capacity
     * @param initial_capacity Initial capacity to reserve
     */
    explicit Stack(size_type initial_capacity);

    /**
     * @brief Copy constructor
     * @param other Stack to copy from
     */
    Stack(const Stack& other);

    /**
     * @brief Move constructor
     * @param other Stack to move from
     */
    Stack(Stack&& other) noexcept;

    /**
     * @brief Destructor
     */
    ~Stack() = default;

    /**
     * @brief Copy assignment operator
     * @param other Stack to copy from
     * @return Reference to this stack
     */
    Stack& operator=(const Stack& other);

    /**
     * @brief Move assignment operator
     * @param other Stack to move from
     * @return Reference to this stack
     */
    Stack& operator=(Stack&& other) noexcept;

    // Element access
    /**
     * @brief Access the top element
     * @return Reference to the top element
     * @throws std::out_of_range if stack is empty
     */
    reference top();
    const_reference top() const;

    // Capacity
    /**
     * @brief Check if the stack is empty
     * @return true if stack is empty, false otherwise
     */
    bool empty() const noexcept;

    /**
     * @brief Get the number of elements in the stack
     * @return Number of elements
     */
    size_type size() const noexcept;

    /**
     * @brief Get the current capacity of the underlying container
     * @return Current capacity
     */
    size_type capacity() const noexcept;

    // Modifiers
    /**
     * @brief Push an element onto the stack
     * @param value Value to push
     */
    void push(const T& value);

    /**
     * @brief Push an element onto the stack (move version)
     * @param value Value to move onto the stack
     */
    void push(T&& value);

    /**
     * @brief Remove the top element from the stack
     * @throws std::out_of_range if stack is empty
     */
    void pop();

    /**
     * @brief Remove all elements from the stack
     */
    void clear() noexcept;

    /**
     * @brief Swap contents with another stack
     * @param other Stack to swap with
     */
    void swap(Stack& other) noexcept;

    /**
     * @brief Reserve capacity for at least new_capacity elements
     * @param new_capacity Minimum capacity to reserve
     */
    void reserve(size_type new_capacity);

private:
    DynamicArray<T> m_container;  ///< Underlying container
};

} // namespace linear
} // namespace mylib

#endif // MYLIB_LINEAR_STACK_HPP
