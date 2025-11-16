/**
 * @file linked_list.hpp
 * @brief Doubly linked list implementation with dynamic node allocation
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

#ifndef MYLIB_LINEAR_LINKED_LIST_HPP
#define MYLIB_LINEAR_LINKED_LIST_HPP

#include <cstddef>
#include <stdexcept>
#include <utility>
#include <initializer_list>

namespace mylib {
namespace linear {

/**
 * @class LinkedList
 * @brief A doubly linked list container
 * 
 * This class implements a doubly linked list data structure with dynamic
 * memory allocation. It provides O(1) insertion/deletion at both ends
 * and O(n) access by index.
 * 
 * @tparam T The type of elements stored in the list
 */
template <typename T>
class LinkedList {
private:
    /**
     * @struct Node
     * @brief Internal node structure for the linked list
     */
    struct Node {
        T data;           ///< Data stored in the node
        Node* next;       ///< Pointer to next node
        Node* prev;       ///< Pointer to previous node
        
        /**
         * @brief Constructor with data
         * @param value Data to store
         */
        explicit Node(const T& value) 
            : data(value), next(nullptr), prev(nullptr) {}
        
        /**
         * @brief Constructor with data (move version)
         * @param value Data to move
         */
        explicit Node(T&& value) 
            : data(std::move(value)), next(nullptr), prev(nullptr) {}
    };

public:
    // Type aliases
    using value_type = T;
    using size_type = std::size_t;
    using reference = T&;
    using const_reference = const T&;

    /**
     * @brief Default constructor
     * Creates an empty linked list
     */
    LinkedList();

    /**
     * @brief Constructor with count and default value
     * @param count Number of elements
     * @param value Value to initialize elements with
     */
    LinkedList(size_type count, const T& value);

    /**
     * @brief Initializer list constructor
     * @param init Initializer list
     */
    LinkedList(std::initializer_list<T> init);

    /**
     * @brief Copy constructor
     * @param other List to copy from
     */
    LinkedList(const LinkedList& other);

    /**
     * @brief Move constructor
     * @param other List to move from
     */
    LinkedList(LinkedList&& other) noexcept;

    /**
     * @brief Destructor
     */
    ~LinkedList();

    /**
     * @brief Copy assignment operator
     * @param other List to copy from
     * @return Reference to this list
     */
    LinkedList& operator=(const LinkedList& other);

    /**
     * @brief Move assignment operator
     * @param other List to move from
     * @return Reference to this list
     */
    LinkedList& operator=(LinkedList&& other) noexcept;

    // Element access
    /**
     * @brief Access element at index
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
     * @throws std::out_of_range if list is empty
     */
    reference front();
    const_reference front() const;

    /**
     * @brief Access the last element
     * @return Reference to last element
     * @throws std::out_of_range if list is empty
     */
    reference back();
    const_reference back() const;

    // Capacity
    /**
     * @brief Check if list is empty
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
     * @throws std::out_of_range if list is empty
     */
    void pop_front();

    /**
     * @brief Remove last element
     * @throws std::out_of_range if list is empty
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
     * @brief Resize list to contain count elements
     * @param count New size
     */
    void resize(size_type count);

    /**
     * @brief Resize list with default value
     * @param count New size
     * @param value Value for new elements
     */
    void resize(size_type count, const T& value);

    /**
     * @brief Swap contents with another list
     * @param other List to swap with
     */
    void swap(LinkedList& other) noexcept;

    /**
     * @brief Reverse the list
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
     * @brief Check if value exists in list
     * @param value Value to check
     * @return true if value exists, false otherwise
     */
    bool contains(const T& value) const;

private:
    Node* m_head;         ///< Pointer to first node
    Node* m_tail;         ///< Pointer to last node
    size_type m_size;     ///< Current number of elements

    /**
     * @brief Get node at specific index
     * @param index Position of node
     * @return Pointer to node
     */
    Node* get_node(size_type index) const;

    /**
     * @brief Deep copy from another list
     * @param other List to copy from
     */
    void copy_from(const LinkedList& other);
};

} // namespace linear
} // namespace mylib

#endif // MYLIB_LINEAR_LINKED_LIST_HPP
