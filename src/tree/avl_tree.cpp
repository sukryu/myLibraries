/**
 * @file avl_tree.cpp
 * @brief Implementation of AVLTree class
 * @author Jinhyeok
 * @date 2025-11-29
 * @version 1.0.0
 */

#include "tree/avl_tree.hpp"
#include <queue>
#include <algorithm>

namespace mylib {
namespace tree {

// Constructors
template <typename T>
AVLTree<T>::AVLTree()
    : m_root(nullptr), m_size(0) {
}

template <typename T>
AVLTree<T>::AVLTree(std::initializer_list<T> init)
    : m_root(nullptr), m_size(0) {
    for (const auto& item : init) {
        insert(item);
    }
}

template <typename T>
AVLTree<T>::AVLTree(const AVLTree& other)
    : m_root(nullptr), m_size(0) {
    m_root = copy_tree(other.m_root);
    m_size = other.m_size;
}

template <typename T>
AVLTree<T>::AVLTree(AVLTree&& other) noexcept
    : m_root(other.m_root), m_size(other.m_size) {
    other.m_root = nullptr;
    other.m_size = 0;
}

template <typename T>
AVLTree<T>::~AVLTree() {
    destroy(m_root);
}

// Assignment operators
template <typename T>
AVLTree<T>& AVLTree<T>::operator=(const AVLTree& other) {
    if (this != &other) {
        destroy(m_root);
        m_root = copy_tree(other.m_root);
        m_size = other.m_size;
    }
    return *this;
}

template <typename T>
AVLTree<T>& AVLTree<T>::operator=(AVLTree&& other) noexcept {
    if (this != &other) {
        destroy(m_root);
        m_root = other.m_root;
        m_size = other.m_size;
        other.m_root = nullptr;
        other.m_size = 0;
    }
    return *this;
}

// Capacity
template <typename T>
bool AVLTree<T>::empty() const noexcept {
    return m_size == 0;
}

template <typename T>
typename AVLTree<T>::size_type AVLTree<T>::size() const noexcept {
    return m_size;
}

template <typename T>
typename AVLTree<T>::size_type AVLTree<T>::height() const noexcept {
    return static_cast<size_type>(get_height(m_root));
}

// Height helpers
template <typename T>
int AVLTree<T>::get_height(Node* node) const noexcept {
    return node ? node->height : 0;
}

template <typename T>
void AVLTree<T>::update_height(Node* node) noexcept {
    if (node) {
        node->height = 1 + std::max(get_height(node->left), get_height(node->right));
    }
}

template <typename T>
int AVLTree<T>::get_balance(Node* node) const noexcept {
    return node ? get_height(node->left) - get_height(node->right) : 0;
}

template <typename T>
int AVLTree<T>::balance_factor() const noexcept {
    return get_balance(m_root);
}

// Rotation operations
template <typename T>
typename AVLTree<T>::Node* AVLTree<T>::rotate_right(Node* y) {
    //       y                x
    //      / \             /   \
    //     x   T3   -->    T1    y
    //    / \                   / \
    //   T1  T2                T2  T3
    
    Node* x = y->left;
    Node* T2 = x->right;
    
    // Perform rotation
    x->right = y;
    y->left = T2;
    
    // Update heights (order matters: y first, then x)
    update_height(y);
    update_height(x);
    
    return x;  // New root
}

template <typename T>
typename AVLTree<T>::Node* AVLTree<T>::rotate_left(Node* x) {
    //     x                  y
    //    / \               /   \
    //   T1  y     -->     x    T3
    //      / \           / \
    //     T2  T3        T1  T2
    
    Node* y = x->right;
    Node* T2 = y->left;
    
    // Perform rotation
    y->left = x;
    x->right = T2;
    
    // Update heights (order matters: x first, then y)
    update_height(x);
    update_height(y);
    
    return y;  // New root
}

template <typename T>
typename AVLTree<T>::Node* AVLTree<T>::rebalance(Node* node) {
    if (!node) {
        return nullptr;
    }
    
    update_height(node);
    int balance = get_balance(node);
    
    // Left heavy (balance > 1)
    if (balance > 1) {
        // Left-Left case: single right rotation
        if (get_balance(node->left) >= 0) {
            return rotate_right(node);
        }
        // Left-Right case: left rotation on left child, then right rotation
        else {
            node->left = rotate_left(node->left);
            return rotate_right(node);
        }
    }
    
    // Right heavy (balance < -1)
    if (balance < -1) {
        // Right-Right case: single left rotation
        if (get_balance(node->right) <= 0) {
            return rotate_left(node);
        }
        // Right-Left case: right rotation on right child, then left rotation
        else {
            node->right = rotate_right(node->right);
            return rotate_left(node);
        }
    }
    
    // Already balanced
    return node;
}

// Modifiers
template <typename T>
bool AVLTree<T>::insert(const T& value) {
    bool inserted = false;
    m_root = insert_recursive(m_root, value, inserted);
    if (inserted) {
        ++m_size;
    }
    return inserted;
}

template <typename T>
bool AVLTree<T>::insert(T&& value) {
    bool inserted = false;
    m_root = insert_recursive(m_root, std::move(value), inserted);
    if (inserted) {
        ++m_size;
    }
    return inserted;
}

template <typename T>
bool AVLTree<T>::remove(const T& value) {
    bool removed = false;
    m_root = remove_recursive(m_root, value, removed);
    if (removed) {
        --m_size;
    }
    return removed;
}

template <typename T>
void AVLTree<T>::clear() noexcept {
    destroy(m_root);
    m_root = nullptr;
    m_size = 0;
}

template <typename T>
void AVLTree<T>::swap(AVLTree& other) noexcept {
    std::swap(m_root, other.m_root);
    std::swap(m_size, other.m_size);
}

// Recursive insert
template <typename T>
typename AVLTree<T>::Node* AVLTree<T>::insert_recursive(
    Node* node, const T& value, bool& inserted) {
    
    // Base case: found insertion point
    if (!node) {
        inserted = true;
        return new Node(value);
    }
    
    // BST insertion
    if (value < node->data) {
        node->left = insert_recursive(node->left, value, inserted);
    } else if (value > node->data) {
        node->right = insert_recursive(node->right, value, inserted);
    } else {
        // Duplicate, do not insert
        return node;
    }
    
    // Rebalance and return
    return rebalance(node);
}

template <typename T>
typename AVLTree<T>::Node* AVLTree<T>::insert_recursive(
    Node* node, T&& value, bool& inserted) {
    
    if (!node) {
        inserted = true;
        return new Node(std::move(value));
    }
    
    if (value < node->data) {
        node->left = insert_recursive(node->left, std::move(value), inserted);
    } else if (value > node->data) {
        node->right = insert_recursive(node->right, std::move(value), inserted);
    } else {
        return node;
    }
    
    return rebalance(node);
}

// Recursive remove
template <typename T>
typename AVLTree<T>::Node* AVLTree<T>::remove_recursive(
    Node* node, const T& value, bool& removed) {
    
    if (!node) {
        return nullptr;
    }
    
    // BST deletion
    if (value < node->data) {
        node->left = remove_recursive(node->left, value, removed);
    } else if (value > node->data) {
        node->right = remove_recursive(node->right, value, removed);
    } else {
        // Found node to delete
        removed = true;
        
        // Case 1 & 2: Node with one or no child
        if (!node->left || !node->right) {
            Node* child = node->left ? node->left : node->right;
            
            if (!child) {
                // No child case
                delete node;
                return nullptr;
            } else {
                // One child case
                delete node;
                return child;
            }
        }
        
        // Case 3: Node with two children
        // Get in-order successor (smallest in right subtree)
        Node* successor = find_min(node->right);
        node->data = successor->data;
        
        // Delete the successor
        bool dummy = false;
        node->right = remove_recursive(node->right, successor->data, dummy);
    }
    
    // Rebalance and return
    return rebalance(node);
}

// Lookup
template <typename T>
bool AVLTree<T>::contains(const T& value) const {
    return search(m_root, value) != nullptr;
}

template <typename T>
const T* AVLTree<T>::find(const T& value) const {
    Node* node = search(m_root, value);
    return node ? &(node->data) : nullptr;
}

template <typename T>
typename AVLTree<T>::const_reference AVLTree<T>::min() const {
    if (empty()) {
        throw std::out_of_range("AVLTree::min: tree is empty");
    }
    return find_min(m_root)->data;
}

template <typename T>
typename AVLTree<T>::const_reference AVLTree<T>::max() const {
    if (empty()) {
        throw std::out_of_range("AVLTree::max: tree is empty");
    }
    return find_max(m_root)->data;
}

// Search helpers
template <typename T>
typename AVLTree<T>::Node* AVLTree<T>::search(Node* node, const T& value) const {
    while (node) {
        if (value < node->data) {
            node = node->left;
        } else if (value > node->data) {
            node = node->right;
        } else {
            return node;
        }
    }
    return nullptr;
}

template <typename T>
typename AVLTree<T>::Node* AVLTree<T>::find_min(Node* node) const {
    while (node && node->left) {
        node = node->left;
    }
    return node;
}

template <typename T>
typename AVLTree<T>::Node* AVLTree<T>::find_max(Node* node) const {
    while (node && node->right) {
        node = node->right;
    }
    return node;
}

// Traversal
template <typename T>
void AVLTree<T>::inorder(std::function<void(const T&)> visitor) const {
    inorder_recursive(m_root, visitor);
}

template <typename T>
void AVLTree<T>::preorder(std::function<void(const T&)> visitor) const {
    preorder_recursive(m_root, visitor);
}

template <typename T>
void AVLTree<T>::postorder(std::function<void(const T&)> visitor) const {
    postorder_recursive(m_root, visitor);
}

template <typename T>
void AVLTree<T>::levelorder(std::function<void(const T&)> visitor) const {
    if (!m_root) {
        return;
    }
    
    std::queue<Node*> q;
    q.push(m_root);
    
    while (!q.empty()) {
        Node* current = q.front();
        q.pop();
        
        visitor(current->data);
        
        if (current->left) {
            q.push(current->left);
        }
        if (current->right) {
            q.push(current->right);
        }
    }
}

// Traversal helpers
template <typename T>
void AVLTree<T>::inorder_recursive(
    Node* node, std::function<void(const T&)>& visitor) const {
    
    if (node) {
        inorder_recursive(node->left, visitor);
        visitor(node->data);
        inorder_recursive(node->right, visitor);
    }
}

template <typename T>
void AVLTree<T>::preorder_recursive(
    Node* node, std::function<void(const T&)>& visitor) const {
    
    if (node) {
        visitor(node->data);
        preorder_recursive(node->left, visitor);
        preorder_recursive(node->right, visitor);
    }
}

template <typename T>
void AVLTree<T>::postorder_recursive(
    Node* node, std::function<void(const T&)>& visitor) const {
    
    if (node) {
        postorder_recursive(node->left, visitor);
        postorder_recursive(node->right, visitor);
        visitor(node->data);
    }
}

// Validation
template <typename T>
bool AVLTree<T>::is_balanced() const {
    return is_balanced_recursive(m_root);
}

template <typename T>
bool AVLTree<T>::is_balanced_recursive(Node* node) const {
    if (!node) {
        return true;
    }
    
    int balance = get_balance(node);
    if (balance < -1 || balance > 1) {
        return false;
    }
    
    return is_balanced_recursive(node->left) && is_balanced_recursive(node->right);
}

template <typename T>
bool AVLTree<T>::is_valid() const {
    return is_valid_recursive(m_root, nullptr, nullptr);
}

template <typename T>
bool AVLTree<T>::is_valid_recursive(
    Node* node, const T* min_val, const T* max_val) const {
    
    if (!node) {
        return true;
    }
    
    if (min_val && node->data <= *min_val) {
        return false;
    }
    if (max_val && node->data >= *max_val) {
        return false;
    }
    
    return is_valid_recursive(node->left, min_val, &(node->data)) &&
           is_valid_recursive(node->right, &(node->data), max_val);
}

// Memory management
template <typename T>
void AVLTree<T>::destroy(Node* node) noexcept {
    if (node) {
        destroy(node->left);
        destroy(node->right);
        delete node;
    }
}

template <typename T>
typename AVLTree<T>::Node* AVLTree<T>::copy_tree(Node* node) const {
    if (!node) {
        return nullptr;
    }
    
    Node* new_node = new Node(node->data);
    new_node->height = node->height;
    new_node->left = copy_tree(node->left);
    new_node->right = copy_tree(node->right);
    
    return new_node;
}

// Explicit template instantiations
template class AVLTree<int>;
template class AVLTree<double>;
template class AVLTree<float>;
template class AVLTree<char>;
template class AVLTree<long>;
template class AVLTree<long long>;
template class AVLTree<unsigned int>;
template class AVLTree<unsigned long>;
template class AVLTree<unsigned long long>;

} // namespace tree
} // namespace mylib