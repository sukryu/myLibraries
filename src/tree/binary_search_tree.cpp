/**
 * @file binary_search_tree.cpp
 * @brief Implementation of BinarySearchTree class
 * @author Jinhyeok
 * @date 2025-11-29
 * @version 1.0.0
 */

#include "tree/binary_search_tree.hpp"
#include <queue>  // for level-order traversal

namespace mylib {
namespace tree {

// Constructors
template <typename T>
BinarySearchTree<T>::BinarySearchTree()
    : m_root(nullptr), m_size(0) {
}

template <typename T>
BinarySearchTree<T>::BinarySearchTree(std::initializer_list<T> init)
    : m_root(nullptr), m_size(0) {
    for (const auto& item : init) {
        insert(item);
    }
}

template <typename T>
BinarySearchTree<T>::BinarySearchTree(const BinarySearchTree& other)
    : m_root(nullptr), m_size(0) {
    m_root = copy_tree(other.m_root);
    m_size = other.m_size;
}

template <typename T>
BinarySearchTree<T>::BinarySearchTree(BinarySearchTree&& other) noexcept
    : m_root(other.m_root), m_size(other.m_size) {
    other.m_root = nullptr;
    other.m_size = 0;
}

template <typename T>
BinarySearchTree<T>::~BinarySearchTree() {
    destroy(m_root);
}

// Assignment operators
template <typename T>
BinarySearchTree<T>& BinarySearchTree<T>::operator=(const BinarySearchTree& other) {
    if (this != &other) {
        destroy(m_root);
        m_root = copy_tree(other.m_root);
        m_size = other.m_size;
    }
    return *this;
}

template <typename T>
BinarySearchTree<T>& BinarySearchTree<T>::operator=(BinarySearchTree&& other) noexcept {
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
bool BinarySearchTree<T>::empty() const noexcept {
    return m_size == 0;
}

template <typename T>
typename BinarySearchTree<T>::size_type BinarySearchTree<T>::size() const noexcept {
    return m_size;
}

template <typename T>
typename BinarySearchTree<T>::size_type BinarySearchTree<T>::height() const noexcept {
    return height_recursive(m_root);
}

// Modifiers
template <typename T>
bool BinarySearchTree<T>::insert(const T& value) {
    bool inserted = false;
    m_root = insert_recursive(m_root, value, inserted);
    if (inserted) {
        ++m_size;
    }
    return inserted;
}

template <typename T>
bool BinarySearchTree<T>::insert(T&& value) {
    bool inserted = false;
    m_root = insert_recursive(m_root, std::move(value), inserted);
    if (inserted) {
        ++m_size;
    }
    return inserted;
}

template <typename T>
bool BinarySearchTree<T>::remove(const T& value) {
    bool removed = false;
    m_root = remove_recursive(m_root, value, removed);
    if (removed) {
        --m_size;
    }
    return removed;
}

template <typename T>
void BinarySearchTree<T>::clear() noexcept {
    destroy(m_root);
    m_root = nullptr;
    m_size = 0;
}

template <typename T>
void BinarySearchTree<T>::swap(BinarySearchTree& other) noexcept {
    std::swap(m_root, other.m_root);
    std::swap(m_size, other.m_size);
}

// Lookup
template <typename T>
bool BinarySearchTree<T>::contains(const T& value) const {
    return search(m_root, value) != nullptr;
}

template <typename T>
const T* BinarySearchTree<T>::find(const T& value) const {
    Node* node = search(m_root, value);
    return node ? &(node->data) : nullptr;
}

template <typename T>
typename BinarySearchTree<T>::const_reference BinarySearchTree<T>::min() const {
    if (empty()) {
        throw std::out_of_range("BinarySearchTree::min: tree is empty");
    }
    return find_min(m_root)->data;
}

template <typename T>
typename BinarySearchTree<T>::const_reference BinarySearchTree<T>::max() const {
    if (empty()) {
        throw std::out_of_range("BinarySearchTree::max: tree is empty");
    }
    return find_max(m_root)->data;
}

// Traversal
template <typename T>
void BinarySearchTree<T>::inorder(std::function<void(const T&)> visitor) const {
    inorder_recursive(m_root, visitor);
}

template <typename T>
void BinarySearchTree<T>::preorder(std::function<void(const T&)> visitor) const {
    preorder_recursive(m_root, visitor);
}

template <typename T>
void BinarySearchTree<T>::postorder(std::function<void(const T&)> visitor) const {
    postorder_recursive(m_root, visitor);
}

template <typename T>
void BinarySearchTree<T>::levelorder(std::function<void(const T&)> visitor) const {
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

// Advanced operations
template <typename T>
const T* BinarySearchTree<T>::successor(const T& value) const {
    Node* current = m_root;
    Node* successor_node = nullptr;
    
    while (current) {
        if (value < current->data) {
            // Current could be successor, go left to find smaller
            successor_node = current;
            current = current->left;
        } else if (value > current->data) {
            // Successor must be in right subtree
            current = current->right;
        } else {
            // Found the node
            if (current->right) {
                // Successor is minimum of right subtree
                return &(find_min(current->right)->data);
            }
            // Otherwise, successor is the last ancestor where we went left
            break;
        }
    }
    
    return successor_node ? &(successor_node->data) : nullptr;
}

template <typename T>
const T* BinarySearchTree<T>::predecessor(const T& value) const {
    Node* current = m_root;
    Node* predecessor_node = nullptr;
    
    while (current) {
        if (value > current->data) {
            // Current could be predecessor, go right to find larger
            predecessor_node = current;
            current = current->right;
        } else if (value < current->data) {
            // Predecessor must be in left subtree
            current = current->left;
        } else {
            // Found the node
            if (current->left) {
                // Predecessor is maximum of left subtree
                return &(find_max(current->left)->data);
            }
            // Otherwise, predecessor is the last ancestor where we went right
            break;
        }
    }
    
    return predecessor_node ? &(predecessor_node->data) : nullptr;
}

template <typename T>
bool BinarySearchTree<T>::is_valid() const {
    return is_valid_recursive(m_root, nullptr, nullptr);
}

// Private helper functions
template <typename T>
typename BinarySearchTree<T>::Node* BinarySearchTree<T>::insert_recursive(
    Node* node, const T& value, bool& inserted) {
    
    if (!node) {
        inserted = true;
        return new Node(value);
    }
    
    if (value < node->data) {
        node->left = insert_recursive(node->left, value, inserted);
    } else if (value > node->data) {
        node->right = insert_recursive(node->right, value, inserted);
    }
    // If equal, do nothing (no duplicates)
    
    return node;
}

template <typename T>
typename BinarySearchTree<T>::Node* BinarySearchTree<T>::insert_recursive(
    Node* node, T&& value, bool& inserted) {
    
    if (!node) {
        inserted = true;
        return new Node(std::move(value));
    }
    
    if (value < node->data) {
        node->left = insert_recursive(node->left, std::move(value), inserted);
    } else if (value > node->data) {
        node->right = insert_recursive(node->right, std::move(value), inserted);
    }
    // If equal, do nothing (no duplicates)
    
    return node;
}

template <typename T>
typename BinarySearchTree<T>::Node* BinarySearchTree<T>::remove_recursive(
    Node* node, const T& value, bool& removed) {
    
    if (!node) {
        return nullptr;
    }
    
    if (value < node->data) {
        node->left = remove_recursive(node->left, value, removed);
    } else if (value > node->data) {
        node->right = remove_recursive(node->right, value, removed);
    } else {
        // Found the node to remove
        removed = true;
        
        // Case 1: No children (leaf node)
        if (!node->left && !node->right) {
            delete node;
            return nullptr;
        }
        
        // Case 2: One child
        if (!node->left) {
            Node* right_child = node->right;
            delete node;
            return right_child;
        }
        if (!node->right) {
            Node* left_child = node->left;
            delete node;
            return left_child;
        }
        
        // Case 3: Two children
        // Find in-order successor (minimum in right subtree)
        Node* successor = find_min(node->right);
        node->data = successor->data;
        // Remove the successor from right subtree
        bool dummy = false;
        node->right = remove_recursive(node->right, successor->data, dummy);
    }
    
    return node;
}

template <typename T>
typename BinarySearchTree<T>::Node* BinarySearchTree<T>::find_min(Node* node) const {
    while (node && node->left) {
        node = node->left;
    }
    return node;
}

template <typename T>
typename BinarySearchTree<T>::Node* BinarySearchTree<T>::find_max(Node* node) const {
    while (node && node->right) {
        node = node->right;
    }
    return node;
}

template <typename T>
typename BinarySearchTree<T>::Node* BinarySearchTree<T>::search(
    Node* node, const T& value) const {
    
    while (node) {
        if (value < node->data) {
            node = node->left;
        } else if (value > node->data) {
            node = node->right;
        } else {
            return node;  // Found
        }
    }
    return nullptr;  // Not found
}

template <typename T>
typename BinarySearchTree<T>::size_type BinarySearchTree<T>::height_recursive(
    Node* node) const noexcept {
    
    if (!node) {
        return 0;
    }
    
    size_type left_height = height_recursive(node->left);
    size_type right_height = height_recursive(node->right);
    
    return 1 + (left_height > right_height ? left_height : right_height);
}

template <typename T>
void BinarySearchTree<T>::destroy(Node* node) noexcept {
    if (node) {
        destroy(node->left);
        destroy(node->right);
        delete node;
    }
}

template <typename T>
typename BinarySearchTree<T>::Node* BinarySearchTree<T>::copy_tree(Node* node) const {
    if (!node) {
        return nullptr;
    }
    
    Node* new_node = new Node(node->data);
    new_node->left = copy_tree(node->left);
    new_node->right = copy_tree(node->right);
    
    return new_node;
}

template <typename T>
void BinarySearchTree<T>::inorder_recursive(
    Node* node, std::function<void(const T&)>& visitor) const {
    
    if (node) {
        inorder_recursive(node->left, visitor);
        visitor(node->data);
        inorder_recursive(node->right, visitor);
    }
}

template <typename T>
void BinarySearchTree<T>::preorder_recursive(
    Node* node, std::function<void(const T&)>& visitor) const {
    
    if (node) {
        visitor(node->data);
        preorder_recursive(node->left, visitor);
        preorder_recursive(node->right, visitor);
    }
}

template <typename T>
void BinarySearchTree<T>::postorder_recursive(
    Node* node, std::function<void(const T&)>& visitor) const {
    
    if (node) {
        postorder_recursive(node->left, visitor);
        postorder_recursive(node->right, visitor);
        visitor(node->data);
    }
}

template <typename T>
bool BinarySearchTree<T>::is_valid_recursive(
    Node* node, const T* min_val, const T* max_val) const {
    
    if (!node) {
        return true;
    }
    
    // Check if current node violates BST property
    if (min_val && node->data <= *min_val) {
        return false;
    }
    if (max_val && node->data >= *max_val) {
        return false;
    }
    
    // Recursively check left and right subtrees
    return is_valid_recursive(node->left, min_val, &(node->data)) &&
           is_valid_recursive(node->right, &(node->data), max_val);
}

// Explicit template instantiations for common types
template class BinarySearchTree<int>;
template class BinarySearchTree<double>;
template class BinarySearchTree<float>;
template class BinarySearchTree<char>;
template class BinarySearchTree<long>;
template class BinarySearchTree<long long>;
template class BinarySearchTree<unsigned int>;
template class BinarySearchTree<unsigned long>;
template class BinarySearchTree<unsigned long long>;

} // namespace tree
} // namespace mylib