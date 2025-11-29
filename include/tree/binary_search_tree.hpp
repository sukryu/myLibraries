/**
 * @file binary_search_tree.hpp
 * @brief Binary Search Tree implementation with recursive operations
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

#ifndef MYLIB_TREE_BINARY_SEARCH_TREE_HPP
#define MYLIB_TREE_BINARY_SEARCH_TREE_HPP

#include <cstddef>
#include <stdexcept>
#include <utility>
#include <initializer_list>
#include <functional>

namespace mylib {
namespace tree {

/**
 * @class BinarySearchTree
 * @brief A binary search tree container with ordered elements
 * 
 * This class implements a binary search tree data structure where
 * left subtree contains smaller values and right subtree contains
 * larger values. Provides O(log n) average case for search, insert,
 * and delete operations.
 * 
 * @tparam T The type of elements stored in the tree (must be comparable)
 */
template <typename T>
class BinarySearchTree {
private:
    /**
     * @struct Node
     * @brief Internal node structure for the tree
     */
    struct Node {
        T data;           ///< Data stored in the node
        Node* left;       ///< Pointer to left child
        Node* right;      ///< Pointer to right child
        
        /**
         * @brief Constructor with data
         * @param value Data to store
         */
        explicit Node(const T& value)
            : data(value), left(nullptr), right(nullptr) {}
        
        /**
         * @brief Constructor with data (move version)
         * @param value Data to move
         */
        explicit Node(T&& value)
            : data(std::move(value)), left(nullptr), right(nullptr) {}
    };

public:
    // Type aliases
    using value_type = T;
    using size_type = std::size_t;
    using reference = T&;
    using const_reference = const T&;

    /**
     * @brief Default constructor
     * Creates an empty binary search tree
     */
    BinarySearchTree();

    /**
     * @brief Initializer list constructor
     * @param init Initializer list of elements to insert
     */
    BinarySearchTree(std::initializer_list<T> init);

    /**
     * @brief Copy constructor
     * @param other Tree to copy from
     */
    BinarySearchTree(const BinarySearchTree& other);

    /**
     * @brief Move constructor
     * @param other Tree to move from
     */
    BinarySearchTree(BinarySearchTree&& other) noexcept;

    /**
     * @brief Destructor
     */
    ~BinarySearchTree();

    /**
     * @brief Copy assignment operator
     * @param other Tree to copy from
     * @return Reference to this tree
     */
    BinarySearchTree& operator=(const BinarySearchTree& other);

    /**
     * @brief Move assignment operator
     * @param other Tree to move from
     * @return Reference to this tree
     */
    BinarySearchTree& operator=(BinarySearchTree&& other) noexcept;

    // Capacity
    /**
     * @brief Check if tree is empty
     * @return true if empty, false otherwise
     */
    bool empty() const noexcept;

    /**
     * @brief Get number of elements
     * @return Number of elements
     */
    size_type size() const noexcept;

    /**
     * @brief Get height of the tree
     * @return Height of the tree (0 for empty tree)
     */
    size_type height() const noexcept;

    // Modifiers
    /**
     * @brief Insert an element into the tree
     * @param value Value to insert
     * @return true if inserted, false if duplicate
     */
    bool insert(const T& value);

    /**
     * @brief Insert an element into the tree (move version)
     * @param value Value to move into tree
     * @return true if inserted, false if duplicate
     */
    bool insert(T&& value);

    /**
     * @brief Remove an element from the tree
     * @param value Value to remove
     * @return true if removed, false if not found
     */
    bool remove(const T& value);

    /**
     * @brief Clear all elements from the tree
     */
    void clear() noexcept;

    /**
     * @brief Swap contents with another tree
     * @param other Tree to swap with
     */
    void swap(BinarySearchTree& other) noexcept;

    // Lookup
    /**
     * @brief Search for an element in the tree
     * @param value Value to search for
     * @return true if found, false otherwise
     */
    bool contains(const T& value) const;

    /**
     * @brief Find an element and return pointer to it
     * @param value Value to find
     * @return Pointer to element, or nullptr if not found
     */
    const T* find(const T& value) const;

    /**
     * @brief Get the minimum element
     * @return Reference to minimum element
     * @throws std::out_of_range if tree is empty
     */
    const_reference min() const;

    /**
     * @brief Get the maximum element
     * @return Reference to maximum element
     * @throws std::out_of_range if tree is empty
     */
    const_reference max() const;

    // Traversal
    /**
     * @brief In-order traversal (left, root, right)
     * @param visitor Function to call for each element
     * 
     * Elements are visited in sorted order.
     */
    void inorder(std::function<void(const T&)> visitor) const;

    /**
     * @brief Pre-order traversal (root, left, right)
     * @param visitor Function to call for each element
     */
    void preorder(std::function<void(const T&)> visitor) const;

    /**
     * @brief Post-order traversal (left, right, root)
     * @param visitor Function to call for each element
     */
    void postorder(std::function<void(const T&)> visitor) const;

    /**
     * @brief Level-order traversal (breadth-first)
     * @param visitor Function to call for each element
     */
    void levelorder(std::function<void(const T&)> visitor) const;

    // Advanced operations
    /**
     * @brief Find the in-order successor of a value
     * @param value Value to find successor of
     * @return Pointer to successor, or nullptr if none exists
     */
    const T* successor(const T& value) const;

    /**
     * @brief Find the in-order predecessor of a value
     * @param value Value to find predecessor of
     * @return Pointer to predecessor, or nullptr if none exists
     */
    const T* predecessor(const T& value) const;

    /**
     * @brief Check if the tree is a valid BST
     * @return true if valid BST, false otherwise
     */
    bool is_valid() const;

private:
    Node* m_root;         ///< Pointer to root node
    size_type m_size;     ///< Current number of elements

    // Helper functions for recursive operations
    
    /**
     * @brief Recursively insert a node
     * @param node Current node
     * @param value Value to insert
     * @return New subtree root after insertion
     */
    Node* insert_recursive(Node* node, const T& value, bool& inserted);
    Node* insert_recursive(Node* node, T&& value, bool& inserted);

    /**
     * @brief Recursively remove a node
     * @param node Current node
     * @param value Value to remove
     * @param removed Output: whether node was removed
     * @return New subtree root after removal
     */
    Node* remove_recursive(Node* node, const T& value, bool& removed);

    /**
     * @brief Find minimum node in subtree
     * @param node Subtree root
     * @return Pointer to minimum node
     */
    Node* find_min(Node* node) const;

    /**
     * @brief Find maximum node in subtree
     * @param node Subtree root
     * @return Pointer to maximum node
     */
    Node* find_max(Node* node) const;

    /**
     * @brief Recursively search for a value
     * @param node Current node
     * @param value Value to search for
     * @return Pointer to node if found, nullptr otherwise
     */
    Node* search(Node* node, const T& value) const;

    /**
     * @brief Calculate height of subtree
     * @param node Subtree root
     * @return Height of subtree
     */
    size_type height_recursive(Node* node) const noexcept;

    /**
     * @brief Recursively delete all nodes
     * @param node Subtree root
     */
    void destroy(Node* node) noexcept;

    /**
     * @brief Deep copy a subtree
     * @param node Subtree root to copy
     * @return New subtree root
     */
    Node* copy_tree(Node* node) const;

    /**
     * @brief In-order traversal helper
     * @param node Current node
     * @param visitor Function to call
     */
    void inorder_recursive(Node* node, std::function<void(const T&)>& visitor) const;

    /**
     * @brief Pre-order traversal helper
     * @param node Current node
     * @param visitor Function to call
     */
    void preorder_recursive(Node* node, std::function<void(const T&)>& visitor) const;

    /**
     * @brief Post-order traversal helper
     * @param node Current node
     * @param visitor Function to call
     */
    void postorder_recursive(Node* node, std::function<void(const T&)>& visitor) const;

    /**
     * @brief Validate BST property helper
     * @param node Current node
     * @param min_val Minimum allowed value (nullptr if no limit)
     * @param max_val Maximum allowed value (nullptr if no limit)
     * @return true if valid BST
     */
    bool is_valid_recursive(Node* node, const T* min_val, const T* max_val) const;
};

} // namespace tree
} // namespace mylib

#endif // MYLIB_TREE_BINARY_SEARCH_TREE_HPP