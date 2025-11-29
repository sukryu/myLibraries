/**
 * @file queue.hpp
 * @brief Queue implementation using LinkedList as underlying container
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

#ifndef MYLIB_LINEAR_QUEUE_HPP
#define MYLIB_LINEAR_QUEUE_HPP

#include "linear/linked_list.hpp"
#include <cstddef>
#include <stdexcept>
#include <utility>

namespace mylib {
namespace linear {

/**
 * @class Queue
 * @brief A FIFO (First-In-First-Out) queue container
 * 
 * This class implements a queue data structure using LinkedList as the
 * underlying container. It provides O(1) enqueue and dequeue operations.
 * 
 * @tparam T The type of elements stored in the queue
 */
template <typename T>
class Queue {
public:
    // Type aliases
    using value_type = T;
    using size_type = std::size_t;
    using reference = T&;
    using const_reference = const T&;

    /**
     * @brief Default constructor
     * Creates an empty queue
     */
    Queue();

    /**
     * @brief Copy constructor
     * @param other Queue to copy from
     */
    Queue(const Queue& other);

    /**
     * @brief Move constructor
     * @param other Queue to move from
     */
    Queue(Queue&& other) noexcept;

    /**
     * @brief Destructor
     */
    ~Queue() = default;

    /**
     * @brief Copy assignment operator
     * @param other Queue to copy from
     * @return Reference to this queue
     */
    Queue& operator=(const Queue& other);

    /**
     * @brief Move assignment operator
     * @param other Queue to move from
     * @return Reference to this queue
     */
    Queue& operator=(Queue&& other) noexcept;

    // Element access
    /**
     * @brief Access the first element (front of queue)
     * @return Reference to the first element
     * @throws std::out_of_range if queue is empty
     */
    reference front();
    const_reference front() const;

    /**
     * @brief Access the last element (back of queue)
     * @return Reference to the last element
     * @throws std::out_of_range if queue is empty
     */
    reference back();
    const_reference back() const;

    // Capacity
    /**
     * @brief Check if the queue is empty
     * @return true if queue is empty, false otherwise
     */
    bool empty() const noexcept;

    /**
     * @brief Get the number of elements in the queue
     * @return Number of elements
     */
    size_type size() const noexcept;

    // Modifiers
    /**
     * @brief Add an element to the back of the queue
     * @param value Value to add
     */
    void push(const T& value);

    /**
     * @brief Add an element to the back of the queue (move version)
     * @param value Value to move into the queue
     */
    void push(T&& value);

    /**
     * @brief Remove the front element from the queue
     * @throws std::out_of_range if queue is empty
     */
    void pop();

    /**
     * @brief Remove all elements from the queue
     */
    void clear() noexcept;

    /**
     * @brief Swap contents with another queue
     * @param other Queue to swap with
     */
    void swap(Queue& other) noexcept;

private:
    LinkedList<T> m_container;  ///< Underlying container
};

} // namespace linear
} // namespace mylib

#endif // MYLIB_LINEAR_QUEUE_HPP