/**
 * @file avl_tree.hpp
 * @brief Self-balancing AVL Tree implementation
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

#ifndef MYLIB_TREE_AVL_TREE_HPP
#define MYLIB_TREE_AVL_TREE_HPP

#include <cstddef>
#include <stdexcept>
#include <utility>
#include <initializer_list>
#include <functional>

namespace mylib {
namespace tree {

/**
 * @class AVLTree
 * @brief A self-balancing binary search tree
 * 
 * This class implements an AVL tree where the heights of the two child
 * subtrees of any node differ by at most one. This ensures O(log n)
 * time complexity for search, insert, and delete operations.
 * 
 * @tparam T The type of elements stored in the tree (must be comparable)
 */
template <typename T>
class AVLTree {
private:
    /**
     * @struct Node
     * @brief Internal node structure for the AVL tree
     */
    struct Node {
        T data;               ///< Data stored in the node
        Node* left;           ///< Pointer to left child
        Node* right;          ///< Pointer to right child
        int height;           ///< Height of the node (leaf = 1)
        
        /**
         * @brief Constructor with data
         * @param value Data to store
         */
        explicit Node(const T& value)
            : data(value), left(nullptr), right(nullptr), height(1) {}
        
        /**
         * @brief Constructor with data (move version)
         * @param value Data to move
         */
        explicit Node(T&& value)
            : data(std::move(value)), left(nullptr), right(nullptr), height(1) {}
    };

public:
    // Type aliases
    using value_type = T;
    using size_type = std::size_t;
    using reference = T&;
    using const_reference = const T&;

    /**
     * @brief Default constructor
     * Creates an empty AVL tree
     */
    AVLTree();

    /**
     * @brief Initializer list constructor
     * @param init Initializer list of elements to insert
     */
    AVLTree(std::initializer_list<T> init);

    /**
     * @brief Copy constructor
     * @param other Tree to copy from
     */
    AVLTree(const AVLTree& other);

    /**
     * @brief Move constructor
     * @param other Tree to move from
     */
    AVLTree(AVLTree&& other) noexcept;

    /**
     * @brief Destructor
     */
    ~AVLTree();

    /**
     * @brief Copy assignment operator
     * @param other Tree to copy from
     * @return Reference to this tree
     */
    AVLTree& operator=(const AVLTree& other);

    /**
     * @brief Move assignment operator
     * @param other Tree to move from
     * @return Reference to this tree
     */
    AVLTree& operator=(AVLTree&& other) noexcept;

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
     * 
     * After insertion, the tree is automatically rebalanced if needed.
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
     * 
     * After removal, the tree is automatically rebalanced if needed.
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
    void swap(AVLTree& other) noexcept;

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

    // AVL-specific operations
    /**
     * @brief Check if the tree is balanced (AVL property)
     * @return true if balanced, false otherwise
     */
    bool is_balanced() const;

    /**
     * @brief Check if the tree is a valid BST
     * @return true if valid BST, false otherwise
     */
    bool is_valid() const;

    /**
     * @brief Get the balance factor of the root
     * @return Balance factor (left height - right height)
     */
    int balance_factor() const noexcept;

private:
    Node* m_root;         ///< Pointer to root node
    size_type m_size;     ///< Current number of elements

    // Height helpers
    /**
     * @brief Get height of a node (handles nullptr)
     * @param node Node to get height of
     * @return Height of node, 0 if nullptr
     */
    int get_height(Node* node) const noexcept;

    /**
     * @brief Update height of a node based on children
     * @param node Node to update
     */
    void update_height(Node* node) noexcept;

    /**
     * @brief Get balance factor of a node
     * @param node Node to check
     * @return Balance factor (left height - right height)
     */
    int get_balance(Node* node) const noexcept;

    // Rotation operations
    /**
     * @brief Right rotation (LL case)
     * @param y Unbalanced node
     * @return New root of subtree after rotation
     * 
     *       y                x
     *      / \             /   \
     *     x   T3   -->    T1    y
     *    / \                   / \
     *   T1  T2                T2  T3
     */
    Node* rotate_right(Node* y);

    /**
     * @brief Left rotation (RR case)
     * @param x Unbalanced node
     * @return New root of subtree after rotation
     * 
     *     x                  y
     *    / \               /   \
     *   T1  y     -->     x    T3
     *      / \           / \
     *     T2  T3        T1  T2
     */
    Node* rotate_left(Node* x);

    /**
     * @brief Rebalance a node if needed
     * @param node Node to rebalance
     * @return New root of subtree after rebalancing
     */
    Node* rebalance(Node* node);

    // Recursive helpers
    /**
     * @brief Recursively insert a node
     * @param node Current node
     * @param value Value to insert
     * @param inserted Output: whether insertion occurred
     * @return New subtree root after insertion and rebalancing
     */
    Node* insert_recursive(Node* node, const T& value, bool& inserted);
    Node* insert_recursive(Node* node, T&& value, bool& inserted);

    /**
     * @brief Recursively remove a node
     * @param node Current node
     * @param value Value to remove
     * @param removed Output: whether removal occurred
     * @return New subtree root after removal and rebalancing
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

    // Traversal helpers
    void inorder_recursive(Node* node, std::function<void(const T&)>& visitor) const;
    void preorder_recursive(Node* node, std::function<void(const T&)>& visitor) const;
    void postorder_recursive(Node* node, std::function<void(const T&)>& visitor) const;

    // Validation helpers
    /**
     * @brief Check if subtree is balanced
     * @param node Subtree root
     * @return true if balanced
     */
    bool is_balanced_recursive(Node* node) const;

    /**
     * @brief Validate BST property
     * @param node Current node
     * @param min_val Minimum allowed value
     * @param max_val Maximum allowed value
     * @return true if valid BST
     */
    bool is_valid_recursive(Node* node, const T* min_val, const T* max_val) const;
};

} // namespace tree
} // namespace mylib

#endif // MYLIB_TREE_AVL_TREE_HPP