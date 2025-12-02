/**
 * @file b_tree.hpp
 * @brief B-Tree data structure implementation
 * @author Jinhyeok
 * @date 2025-12-02
 * @version 1.0.0
 * 
 * This file provides a comprehensive B-Tree implementation with:
 * - Self-balancing multiway search tree
 * - Configurable order (minimum degree)
 * - Guaranteed O(log n) operations
 * - Efficient disk-based storage support
 * 
 * B-Tree Properties:
 * 1. All leaves are at the same level
 * 2. B-Tree of order t (minimum degree) has:
 *    - Every node except root has at least (t-1) keys
 *    - Every node has at most (2t-1) keys
 *    - Every internal node has at least t children
 *    - Every internal node has at most 2t children
 * 3. All keys in a node are sorted
 * 4. Keys in child[i] < keys[i] < keys in child[i+1]
 * 
 * Time Complexity:
 * - search: O(log n)
 * - insert: O(log n)
 * - remove: O(log n)
 * 
 * Space Complexity: O(n)
 * 
 * Copyright (c) 2025 Jinhyeok
 * MIT License
 */

#ifndef MYLIB_TREE_B_TREE_HPP
#define MYLIB_TREE_B_TREE_HPP

#include <iostream>
#include <cstddef>
#include <vector>
#include <memory>
#include <utility>
#include <initializer_list>
#include <functional>
#include <stdexcept>
#include <algorithm>
#include <queue>
#include <optional>

namespace mylib {
namespace tree {

/**
 * @class BTree
 * @brief A self-balancing multiway search tree
 * 
 * B-Trees are designed for systems that read and write large blocks of data.
 * They are commonly used in databases and file systems.
 * 
 * @tparam T Value type (must be comparable)
 * @tparam Compare Comparison function (default: std::less<T>)
 * 
 * Usage:
 * @code
 * // Create B-Tree with minimum degree 3 (max 5 keys per node)
 * BTree<int> tree(3);
 * 
 * // Insert values
 * tree.insert(10);
 * tree.insert(20);
 * tree.insert(5);
 * 
 * // Search
 * if (tree.search(10)) {
 *     std::cout << "Found!" << std::endl;
 * }
 * 
 * // Remove
 * tree.remove(10);
 * 
 * // Traverse
 * tree.inorder([](int val) {
 *     std::cout << val << " ";
 * });
 * @endcode
 */
template <typename T, typename Compare = std::less<T>>
class BTree {
private:
    /**
     * @struct BTreeNode
     * @brief Internal node structure for B-Tree
     */
    struct BTreeNode {
        std::vector<T> keys;                           // Keys stored in node
        std::vector<std::unique_ptr<BTreeNode>> children;  // Child pointers
        bool is_leaf;                                  // True if leaf node
        
        /**
         * @brief Constructor
         */
        explicit BTreeNode(bool leaf = true) : is_leaf(leaf) {}
        
        /**
         * @brief Get number of keys
         */
        std::size_t size() const {
            return keys.size();
        }
        
        /**
         * @brief Check if node is full
         */
        bool is_full(std::size_t max_keys) const {
            return keys.size() >= max_keys;
        }
        
        /**
         * @brief Find index of first key >= target
         */
        template <typename Key, typename Comp>
        std::size_t find_key_index(const Key& key, Comp& comp) const {
            std::size_t idx = 0;
            while (idx < keys.size() && comp(keys[idx], key)) {
                ++idx;
            }
            return idx;
        }
    };
    
    std::unique_ptr<BTreeNode> m_root;  // Root node
    std::size_t m_min_degree;           // Minimum degree (t)
    std::size_t m_size;                 // Number of keys
    Compare m_compare;                  // Comparison function
    
public:
    // ============================================
    // Constructors and Destructor
    // ============================================
    
    /**
     * @brief Constructor with minimum degree
     * @param min_degree Minimum degree t (must be >= 2)
     */
    explicit BTree(std::size_t min_degree = 3, const Compare& comp = Compare())
        : m_root(nullptr), m_min_degree(min_degree), m_size(0), m_compare(comp) {
        if (min_degree < 2) {
            throw std::invalid_argument("Minimum degree must be at least 2");
        }
    }
    
    /**
     * @brief Initializer list constructor
     */
    BTree(std::initializer_list<T> init, std::size_t min_degree = 3, const Compare& comp = Compare())
        : BTree(min_degree, comp) {
        for (const auto& value : init) {
            insert(value);
        }
    }
    
    /**
     * @brief Range constructor
     */
    template <typename InputIt>
    BTree(InputIt first, InputIt last, std::size_t min_degree = 3, const Compare& comp = Compare())
        : BTree(min_degree, comp) {
        for (auto it = first; it != last; ++it) {
            insert(*it);
        }
    }
    
    /**
     * @brief Copy constructor
     */
    BTree(const BTree& other)
        : m_root(nullptr), m_min_degree(other.m_min_degree), 
          m_size(other.m_size), m_compare(other.m_compare) {
        if (other.m_root) {
            m_root = copy_tree(other.m_root.get());
        }
    }
    
    /**
     * @brief Move constructor
     */
    BTree(BTree&& other) noexcept
        : m_root(std::move(other.m_root)),
          m_min_degree(other.m_min_degree),
          m_size(other.m_size),
          m_compare(std::move(other.m_compare)) {
        other.m_size = 0;
    }
    
    /**
     * @brief Copy assignment
     */
    BTree& operator=(const BTree& other) {
        if (this != &other) {
            BTree temp(other);
            swap(temp);
        }
        return *this;
    }
    
    /**
     * @brief Move assignment
     */
    BTree& operator=(BTree&& other) noexcept {
        if (this != &other) {
            m_root = std::move(other.m_root);
            m_min_degree = other.m_min_degree;
            m_size = other.m_size;
            m_compare = std::move(other.m_compare);
            other.m_size = 0;
        }
        return *this;
    }
    
    /**
     * @brief Destructor
     */
    ~BTree() = default;
    
    // ============================================
    // Capacity
    // ============================================
    
    /**
     * @brief Check if tree is empty
     */
    bool empty() const noexcept {
        return m_size == 0;
    }
    
    /**
     * @brief Get number of keys in tree
     */
    std::size_t size() const noexcept {
        return m_size;
    }
    
    /**
     * @brief Get minimum degree
     */
    std::size_t min_degree() const noexcept {
        return m_min_degree;
    }
    
    /**
     * @brief Get maximum keys per node
     */
    std::size_t max_keys() const noexcept {
        return 2 * m_min_degree - 1;
    }
    
    /**
     * @brief Get maximum children per node
     */
    std::size_t max_children() const noexcept {
        return 2 * m_min_degree;
    }
    
    // ============================================
    // Modifiers
    // ============================================
    
    /**
     * @brief Insert a key
     * @param key Key to insert
     * @return true if inserted, false if duplicate
     */
    bool insert(const T& key) {
        return insert_impl(key);
    }
    
    bool insert(T&& key) {
        return insert_impl(std::move(key));
    }
    
    /**
     * @brief Remove a key
     * @param key Key to remove
     * @return true if removed, false if not found
     */
    bool remove(const T& key) {
        if (!m_root) {
            return false;
        }
        
        bool removed = remove_from_node(m_root.get(), key);
        
        // If root is empty after deletion, make its only child the new root
        if (m_root->size() == 0) {
            if (!m_root->is_leaf && !m_root->children.empty()) {
                m_root = std::move(m_root->children[0]);
            } else {
                m_root.reset();
            }
        }
        
        if (removed) {
            --m_size;
        }
        
        return removed;
    }
    
    /**
     * @brief Clear all keys
     */
    void clear() noexcept {
        m_root.reset();
        m_size = 0;
    }
    
    // ============================================
    // Lookup
    // ============================================
    
    /**
     * @brief Search for a key
     * @param key Key to search
     * @return true if found
     */
    bool search(const T& key) const {
        return search_impl(m_root.get(), key);
    }
    
    /**
     * @brief Check if key exists (alias for search)
     */
    bool contains(const T& key) const {
        return search(key);
    }
    
    /**
     * @brief Find minimum key
     */
    std::optional<T> min() const {
        if (!m_root) {
            return std::nullopt;
        }
        
        BTreeNode* node = m_root.get();
        while (!node->is_leaf) {
            node = node->children[0].get();
        }
        
        return node->keys.empty() ? std::nullopt : std::make_optional(node->keys.front());
    }
    
    /**
     * @brief Find maximum key
     */
    std::optional<T> max() const {
        if (!m_root) {
            return std::nullopt;
        }
        
        BTreeNode* node = m_root.get();
        while (!node->is_leaf) {
            node = node->children.back().get();
        }
        
        return node->keys.empty() ? std::nullopt : std::make_optional(node->keys.back());
    }
    
    // ============================================
    // Traversals
    // ============================================
    
    /**
     * @brief Inorder traversal (sorted order)
     */
    template <typename Func>
    void inorder(Func func) const {
        if (m_root) {
            inorder_impl(m_root.get(), func);
        }
    }
    
    /**
     * @brief Level order traversal
     */
    template <typename Func>
    void level_order(Func func) const {
        if (!m_root) return;
        
        std::queue<BTreeNode*> q;
        q.push(m_root.get());
        
        while (!q.empty()) {
            BTreeNode* node = q.front();
            q.pop();
            
            // Process all keys in current node
            for (const auto& key : node->keys) {
                func(key);
            }
            
            // Enqueue children
            if (!node->is_leaf) {
                for (const auto& child : node->children) {
                    q.push(child.get());
                }
            }
        }
    }
    
    /**
     * @brief Convert tree to sorted vector
     */
    std::vector<T> to_vector() const {
        std::vector<T> result;
        result.reserve(m_size);
        
        inorder([&result](const T& val) {
            result.push_back(val);
        });
        
        return result;
    }
    
    // ============================================
    // Tree Properties
    // ============================================
    
    /**
     * @brief Get height of tree
     */
    std::size_t height() const {
        return height_impl(m_root.get());
    }
    
    /**
     * @brief Verify B-Tree properties
     */
    bool verify() const {
        if (!m_root) return true;
        
        // Check if all leaves are at same level
        std::size_t leaf_level = 0;
        return verify_impl(m_root.get(), 0, leaf_level);
    }
    
    /**
     * @brief Print tree structure (for debugging)
     */
    void print_tree() const {
        if (!m_root) {
            std::cout << "Empty tree" << std::endl;
            return;
        }
        print_tree_impl(m_root.get(), 0);
    }
    
    // ============================================
    // Utility
    // ============================================
    
    /**
     * @brief Swap with another tree
     */
    void swap(BTree& other) noexcept {
        using std::swap;
        swap(m_root, other.m_root);
        swap(m_min_degree, other.m_min_degree);
        swap(m_size, other.m_size);
        swap(m_compare, other.m_compare);
    }
    
private:
    // ============================================
    // Insert Implementation
    // ============================================
    
    template <typename ValueType>
    bool insert_impl(ValueType&& key) {
        // Create root if tree is empty
        if (!m_root) {
            m_root = std::make_unique<BTreeNode>(true);
            m_root->keys.push_back(std::forward<ValueType>(key));
            ++m_size;
            return true;
        }
        
        // If root is full, split it
        if (m_root->is_full(max_keys())) {
            auto new_root = std::make_unique<BTreeNode>(false);
            new_root->children.push_back(std::move(m_root));
            split_child(new_root.get(), 0);
            m_root = std::move(new_root);
        }
        
        // Insert into non-full root
        return insert_non_full(m_root.get(), std::forward<ValueType>(key));
    }
    
    template <typename ValueType>
    bool insert_non_full(BTreeNode* node, ValueType&& key) {
        if (node->is_leaf) {
            // Find position to insert
            std::size_t i = 0;
            while (i < node->size() && m_compare(node->keys[i], key)) {
                ++i;
            }
            
            // Check for duplicate
            if (i < node->size() && !m_compare(key, node->keys[i]) && !m_compare(node->keys[i], key)) {
                return false;  // Duplicate found
            }
            
            // Insert key at position i
            node->keys.insert(node->keys.begin() + i, std::forward<ValueType>(key));
            ++m_size;
            return true;
        } else {
            // Find child to descend to
            std::size_t i = 0;
            while (i < node->size() && m_compare(node->keys[i], key)) {
                ++i;
            }
            
            // Check for duplicate in current node
            if (i < node->size() && !m_compare(key, node->keys[i]) && !m_compare(node->keys[i], key)) {
                return false;  // Duplicate found
            }
            
            // Split child if full
            if (node->children[i]->is_full(max_keys())) {
                split_child(node, i);
                
                // After split, determine which child to descend to
                if (i < node->size()) {
                    if (m_compare(node->keys[i], key)) {
                        ++i;
                    } else if (!m_compare(key, node->keys[i]) && !m_compare(node->keys[i], key)) {
                        return false;  // Duplicate is the middle key that was promoted
                    }
                }
            }
            
            return insert_non_full(node->children[i].get(), std::forward<ValueType>(key));
        }
    }
    
    /**
     * @brief Split a full child
     */
    void split_child(BTreeNode* parent, std::size_t index) {
        BTreeNode* full_child = parent->children[index].get();
        auto new_child = std::make_unique<BTreeNode>(full_child->is_leaf);
        
        std::size_t mid = m_min_degree - 1;
        
        // Copy second half of keys to new child
        new_child->keys.assign(
            full_child->keys.begin() + mid + 1,
            full_child->keys.end()
        );
        
        // Copy second half of children if not leaf
        if (!full_child->is_leaf) {
            new_child->children.reserve(m_min_degree);
            for (std::size_t i = mid + 1; i < full_child->children.size(); ++i) {
                new_child->children.push_back(std::move(full_child->children[i]));
            }
            full_child->children.resize(mid + 1);
        }
        
        // Move middle key to parent
        T middle_key = std::move(full_child->keys[mid]);
        full_child->keys.resize(mid);
        
        // Insert middle key and new child to parent
        parent->keys.insert(parent->keys.begin() + index, std::move(middle_key));
        parent->children.insert(parent->children.begin() + index + 1, std::move(new_child));
    }
    
    // ============================================
    // Remove Implementation
    // ============================================
    
    bool remove_from_node(BTreeNode* node, const T& key) {
        std::size_t idx = node->find_key_index(key, m_compare);
        
        // Case 1: Key is in this node
        if (idx < node->size() && !m_compare(key, node->keys[idx]) && !m_compare(node->keys[idx], key)) {
            if (node->is_leaf) {
                return remove_from_leaf(node, idx);
            } else {
                return remove_from_internal(node, idx);
            }
        } else {
            // Case 2: Key is not in this node
            if (node->is_leaf) {
                return false;  // Key not found
            }
            
            bool is_in_subtree = (idx < node->size()) && m_compare(key, node->keys[idx]);
            
            // If child has minimum keys, fill it first
            if (node->children[idx]->size() < m_min_degree) {
                fill_child(node, idx);
                
                // After filling, key position might change
                idx = node->find_key_index(key, m_compare);
                
                // Check if key moved to current node after filling
                if (idx < node->size() && !m_compare(key, node->keys[idx]) && !m_compare(node->keys[idx], key)) {
                    return remove_from_internal(node, idx);
                }
            }
            
            // Recalculate index if needed
            if (idx >= node->children.size()) {
                idx = node->children.size() - 1;
            }
            
            return remove_from_node(node->children[idx].get(), key);
        }
    }
    
    bool remove_from_leaf(BTreeNode* node, std::size_t idx) {
        node->keys.erase(node->keys.begin() + idx);
        return true;
    }
    
    bool remove_from_internal(BTreeNode* node, std::size_t idx) {
        T key = node->keys[idx];
        
        if (node->children[idx]->size() >= m_min_degree) {
            // Case 2a: Predecessor has enough keys
            T pred = get_predecessor(node, idx);
            node->keys[idx] = pred;
            return remove_from_node(node->children[idx].get(), pred);
        } else if (node->children[idx + 1]->size() >= m_min_degree) {
            // Case 2b: Successor has enough keys
            T succ = get_successor(node, idx);
            node->keys[idx] = succ;
            return remove_from_node(node->children[idx + 1].get(), succ);
        } else {
            // Case 2c: Merge with sibling
            merge_children(node, idx);
            return remove_from_node(node->children[idx].get(), key);
        }
    }
    
    T get_predecessor(BTreeNode* node, std::size_t idx) {
        BTreeNode* curr = node->children[idx].get();
        while (!curr->is_leaf) {
            curr = curr->children.back().get();
        }
        return curr->keys.back();
    }
    
    T get_successor(BTreeNode* node, std::size_t idx) {
        BTreeNode* curr = node->children[idx + 1].get();
        while (!curr->is_leaf) {
            curr = curr->children[0].get();
        }
        return curr->keys.front();
    }
    
    void fill_child(BTreeNode* node, std::size_t idx) {
        // If previous sibling has extra keys, borrow from it
        if (idx != 0 && node->children[idx - 1]->size() >= m_min_degree) {
            borrow_from_prev(node, idx);
        }
        // If next sibling has extra keys, borrow from it
        else if (idx != node->children.size() - 1 && node->children[idx + 1]->size() >= m_min_degree) {
            borrow_from_next(node, idx);
        }
        // Merge with sibling
        else {
            if (idx != node->children.size() - 1) {
                merge_children(node, idx);
            } else {
                merge_children(node, idx - 1);
            }
        }
    }
    
    void borrow_from_prev(BTreeNode* node, std::size_t child_idx) {
        BTreeNode* child = node->children[child_idx].get();
        BTreeNode* sibling = node->children[child_idx - 1].get();
        
        // Move key from parent to child
        child->keys.insert(child->keys.begin(), node->keys[child_idx - 1]);
        
        // Move key from sibling to parent
        node->keys[child_idx - 1] = sibling->keys.back();
        sibling->keys.pop_back();
        
        // Move child pointer if not leaf
        if (!child->is_leaf) {
            child->children.insert(child->children.begin(), std::move(sibling->children.back()));
            sibling->children.pop_back();
        }
    }
    
    void borrow_from_next(BTreeNode* node, std::size_t child_idx) {
        BTreeNode* child = node->children[child_idx].get();
        BTreeNode* sibling = node->children[child_idx + 1].get();
        
        // Move key from parent to child
        child->keys.push_back(node->keys[child_idx]);
        
        // Move key from sibling to parent
        node->keys[child_idx] = sibling->keys.front();
        sibling->keys.erase(sibling->keys.begin());
        
        // Move child pointer if not leaf
        if (!child->is_leaf) {
            child->children.push_back(std::move(sibling->children.front()));
            sibling->children.erase(sibling->children.begin());
        }
    }
    
    void merge_children(BTreeNode* node, std::size_t idx) {
        BTreeNode* left_child = node->children[idx].get();
        BTreeNode* right_child = node->children[idx + 1].get();
        
        // Pull key from current node and merge with right child
        left_child->keys.push_back(node->keys[idx]);
        
        // Copy keys from right child
        left_child->keys.insert(
            left_child->keys.end(),
            right_child->keys.begin(),
            right_child->keys.end()
        );
        
        // Copy child pointers if not leaf
        if (!left_child->is_leaf) {
            for (auto& child : right_child->children) {
                left_child->children.push_back(std::move(child));
            }
        }
        
        // Remove key from current node
        node->keys.erase(node->keys.begin() + idx);
        
        // Remove right child
        node->children.erase(node->children.begin() + idx + 1);
    }
    
    // ============================================
    // Search Implementation
    // ============================================
    
    bool search_impl(BTreeNode* node, const T& key) const {
        if (!node) return false;
        
        std::size_t i = 0;
        while (i < node->size() && m_compare(node->keys[i], key)) {
            ++i;
        }
        
        // Key found
        if (i < node->size() && !m_compare(key, node->keys[i]) && !m_compare(node->keys[i], key)) {
            return true;
        }
        
        // Key not found and this is leaf
        if (node->is_leaf) {
            return false;
        }
        
        // Search in appropriate child
        return search_impl(node->children[i].get(), key);
    }
    
    // ============================================
    // Traversal Implementation
    // ============================================
    
    template <typename Func>
    void inorder_impl(BTreeNode* node, Func& func) const {
        if (!node) return;
        
        std::size_t i;
        for (i = 0; i < node->size(); ++i) {
            if (!node->is_leaf) {
                inorder_impl(node->children[i].get(), func);
            }
            func(node->keys[i]);
        }
        
        if (!node->is_leaf) {
            inorder_impl(node->children[i].get(), func);
        }
    }
    
    // ============================================
    // Utility Implementation
    // ============================================
    
    std::unique_ptr<BTreeNode> copy_tree(const BTreeNode* node) {
        if (!node) return nullptr;
        
        auto new_node = std::make_unique<BTreeNode>(node->is_leaf);
        new_node->keys = node->keys;
        
        for (const auto& child : node->children) {
            new_node->children.push_back(copy_tree(child.get()));
        }
        
        return new_node;
    }
    
    std::size_t height_impl(BTreeNode* node) const {
        if (!node) return 0;
        if (node->is_leaf) return 1;
        return 1 + height_impl(node->children[0].get());
    }
    
    bool verify_impl(BTreeNode* node, std::size_t level, std::size_t& leaf_level) const {
        if (!node) return true;
        
        // Check key count constraints
        if (node != m_root.get()) {
            if (node->size() < m_min_degree - 1) {
                return false;  // Too few keys
            }
        }
        
        if (node->size() > max_keys()) {
            return false;  // Too many keys
        }
        
        // Check if keys are sorted
        for (std::size_t i = 1; i < node->size(); ++i) {
            if (!m_compare(node->keys[i - 1], node->keys[i])) {
                return false;
            }
        }
        
        // If leaf, check level consistency
        if (node->is_leaf) {
            if (leaf_level == 0) {
                leaf_level = level;
            }
            return level == leaf_level;
        }
        
        // Check children count
        if (node->children.size() != node->size() + 1) {
            return false;
        }
        
        // Recursively verify children
        for (const auto& child : node->children) {
            if (!verify_impl(child.get(), level + 1, leaf_level)) {
                return false;
            }
        }
        
        return true;
    }
    
    void print_tree_impl(BTreeNode* node, int level) const {
        if (!node) return;
        
        std::cout << "Level " << level << ": [";
        for (std::size_t i = 0; i < node->size(); ++i) {
            if (i > 0) std::cout << ", ";
            std::cout << node->keys[i];
        }
        std::cout << "]" << std::endl;
        
        if (!node->is_leaf) {
            for (const auto& child : node->children) {
                print_tree_impl(child.get(), level + 1);
            }
        }
    }
};

// ============================================
// Non-member functions
// ============================================

/**
 * @brief Swap two B-Trees
 */
template <typename T, typename Compare>
void swap(BTree<T, Compare>& lhs, BTree<T, Compare>& rhs) noexcept {
    lhs.swap(rhs);
}

} // namespace tree
} // namespace mylib

#endif // MYLIB_TREE_B_TREE_HPP