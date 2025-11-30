/**
 * @file red_black_tree.hpp
 * @brief Red-Black Tree data structure implementation
 * @author Jinhyeok
 * @date 2025-11-30
 * @version 1.0.0
 * 
 * This file provides a comprehensive Red-Black Tree implementation with:
 * - Self-balancing binary search tree
 * - Guaranteed O(log n) operations
 * - Full iterator support
 * - Standard container interface
 * 
 * Red-Black Tree Properties:
 * 1. Every node is either red or black
 * 2. The root is always black
 * 3. Every leaf (NIL) is black
 * 4. If a node is red, both children are black
 * 5. All paths from root to leaves have same number of black nodes
 * 
 * Time Complexity:
 * - insert: O(log n)
 * - remove: O(log n)
 * - search: O(log n)
 * - min/max: O(log n)
 * 
 * Space Complexity: O(n)
 * 
 * Copyright (c) 2025 Jinhyeok
 * MIT License
 */

#ifndef MYLIB_TREE_RED_BLACK_TREE_HPP
#define MYLIB_TREE_RED_BLACK_TREE_HPP

#include <cstddef>
#include <functional>
#include <memory>
#include <utility>
#include <initializer_list>
#include <stdexcept>
#include <vector>
#include <queue>

namespace mylib {
namespace tree {

/**
 * @class RedBlackTree
 * @brief A self-balancing binary search tree with red-black coloring
 * 
 * @tparam T Value type
 * @tparam Compare Comparison function (default: std::less<T>)
 * 
 * Usage:
 * @code
 * RedBlackTree<int> tree;
 * tree.insert(5);
 * tree.insert(3);
 * tree.insert(7);
 * 
 * if (tree.contains(5)) {
 *     std::cout << "Found!" << std::endl;
 * }
 * 
 * for (const auto& val : tree) {
 *     std::cout << val << " ";
 * }
 * @endcode
 */
template <typename T, typename Compare = std::less<T>>
class RedBlackTree {
private:
    enum class Color { RED, BLACK };
    
    struct Node {
        T data;
        Color color;
        Node* parent;
        Node* left;
        Node* right;
        
        explicit Node(const T& value, Color c = Color::RED)
            : data(value), color(c), parent(nullptr), left(nullptr), right(nullptr) {}
        
        explicit Node(T&& value, Color c = Color::RED)
            : data(std::move(value)), color(c), parent(nullptr), left(nullptr), right(nullptr) {}
    };
    
public:
    // ============================================
    // Iterator
    // ============================================
    
    class iterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = const T*;
        using reference = const T&;
        
        iterator() : m_node(nullptr), m_tree(nullptr) {}
        iterator(Node* node, const RedBlackTree* tree) : m_node(node), m_tree(tree) {}
        
        reference operator*() const { return m_node->data; }
        pointer operator->() const { return &(m_node->data); }
        
        iterator& operator++() {
            if (m_node == nullptr) return *this;
            
            if (m_node->right != nullptr) {
                // Go to leftmost node in right subtree
                m_node = m_node->right;
                while (m_node->left != nullptr) {
                    m_node = m_node->left;
                }
            } else {
                // Go up until we come from left child
                Node* parent = m_node->parent;
                while (parent != nullptr && m_node == parent->right) {
                    m_node = parent;
                    parent = parent->parent;
                }
                m_node = parent;
            }
            return *this;
        }
        
        iterator operator++(int) {
            iterator tmp = *this;
            ++(*this);
            return tmp;
        }
        
        iterator& operator--() {
            if (m_node == nullptr) {
                // End iterator: go to maximum
                m_node = m_tree->m_root;
                if (m_node != nullptr) {
                    while (m_node->right != nullptr) {
                        m_node = m_node->right;
                    }
                }
                return *this;
            }
            
            if (m_node->left != nullptr) {
                // Go to rightmost node in left subtree
                m_node = m_node->left;
                while (m_node->right != nullptr) {
                    m_node = m_node->right;
                }
            } else {
                // Go up until we come from right child
                Node* parent = m_node->parent;
                while (parent != nullptr && m_node == parent->left) {
                    m_node = parent;
                    parent = parent->parent;
                }
                m_node = parent;
            }
            return *this;
        }
        
        iterator operator--(int) {
            iterator tmp = *this;
            --(*this);
            return tmp;
        }
        
        bool operator==(const iterator& other) const { return m_node == other.m_node; }
        bool operator!=(const iterator& other) const { return m_node != other.m_node; }
        
    private:
        Node* m_node;
        const RedBlackTree* m_tree;
        friend class RedBlackTree;
    };
    
    using const_iterator = iterator;
    
    // ============================================
    // Constructors and Destructor
    // ============================================
    
    /**
     * @brief Default constructor
     */
    RedBlackTree() : m_root(nullptr), m_size(0), m_compare() {}
    
    /**
     * @brief Constructor with custom comparator
     */
    explicit RedBlackTree(const Compare& comp) : m_root(nullptr), m_size(0), m_compare(comp) {}
    
    /**
     * @brief Initializer list constructor
     */
    RedBlackTree(std::initializer_list<T> init) : RedBlackTree() {
        for (const auto& value : init) {
            insert(value);
        }
    }
    
    /**
     * @brief Range constructor
     */
    template <typename InputIt>
    RedBlackTree(InputIt first, InputIt last) : RedBlackTree() {
        for (auto it = first; it != last; ++it) {
            insert(*it);
        }
    }
    
    /**
     * @brief Copy constructor
     */
    RedBlackTree(const RedBlackTree& other) : RedBlackTree() {
        if (other.m_root != nullptr) {
            m_root = copy_tree(other.m_root, nullptr);
            m_size = other.m_size;
        }
    }
    
    /**
     * @brief Move constructor
     */
    RedBlackTree(RedBlackTree&& other) noexcept
        : m_root(other.m_root), m_size(other.m_size), m_compare(std::move(other.m_compare)) {
        other.m_root = nullptr;
        other.m_size = 0;
    }
    
    /**
     * @brief Copy assignment
     */
    RedBlackTree& operator=(const RedBlackTree& other) {
        if (this != &other) {
            RedBlackTree temp(other);
            swap(temp);
        }
        return *this;
    }
    
    /**
     * @brief Move assignment
     */
    RedBlackTree& operator=(RedBlackTree&& other) noexcept {
        if (this != &other) {
            clear();
            m_root = other.m_root;
            m_size = other.m_size;
            m_compare = std::move(other.m_compare);
            other.m_root = nullptr;
            other.m_size = 0;
        }
        return *this;
    }
    
    /**
     * @brief Destructor
     */
    ~RedBlackTree() {
        clear();
    }
    
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
     * @brief Get number of elements
     */
    std::size_t size() const noexcept {
        return m_size;
    }
    
    // ============================================
    // Modifiers
    // ============================================
    
    /**
     * @brief Insert a value
     * @param value Value to insert
     * @return pair of iterator to inserted element and bool (true if inserted)
     */
    std::pair<iterator, bool> insert(const T& value) {
        return insert_impl(value);
    }
    
    /**
     * @brief Insert a value (move version)
     */
    std::pair<iterator, bool> insert(T&& value) {
        return insert_impl(std::move(value));
    }
    
    /**
     * @brief Emplace a value
     */
    template <typename... Args>
    std::pair<iterator, bool> emplace(Args&&... args) {
        return insert_impl(T(std::forward<Args>(args)...));
    }
    
    /**
     * @brief Remove a value
     * @param value Value to remove
     * @return true if removed, false if not found
     */
    bool erase(const T& value) {
        Node* node = find_node(value);
        if (node == nullptr) {
            return false;
        }
        
        erase_node(node);
        return true;
    }
    
    /**
     * @brief Remove element at iterator position
     * @return Iterator to next element
     */
    iterator erase(iterator pos) {
        if (pos.m_node == nullptr) {
            return end();
        }
        
        iterator next = pos;
        ++next;
        
        erase_node(pos.m_node);
        return next;
    }
    
    /**
     * @brief Clear all elements
     */
    void clear() noexcept {
        destroy_tree(m_root);
        m_root = nullptr;
        m_size = 0;
    }
    
    /**
     * @brief Swap contents with another tree
     */
    void swap(RedBlackTree& other) noexcept {
        std::swap(m_root, other.m_root);
        std::swap(m_size, other.m_size);
        std::swap(m_compare, other.m_compare);
    }
    
    // ============================================
    // Lookup
    // ============================================
    
    /**
     * @brief Find an element
     * @return Iterator to element, or end() if not found
     */
    iterator find(const T& value) const {
        Node* node = find_node(value);
        return iterator(node, this);
    }
    
    /**
     * @brief Check if value exists
     */
    bool contains(const T& value) const {
        return find_node(value) != nullptr;
    }
    
    /**
     * @brief Count occurrences (0 or 1 for set)
     */
    std::size_t count(const T& value) const {
        return contains(value) ? 1 : 0;
    }
    
    /**
     * @brief Get minimum value
     * @throws std::runtime_error if tree is empty
     */
    const T& min() const {
        if (empty()) {
            throw std::runtime_error("Tree is empty");
        }
        return find_min(m_root)->data;
    }
    
    /**
     * @brief Get maximum value
     * @throws std::runtime_error if tree is empty
     */
    const T& max() const {
        if (empty()) {
            throw std::runtime_error("Tree is empty");
        }
        return find_max(m_root)->data;
    }
    
    /**
     * @brief Find first element not less than value
     */
    iterator lower_bound(const T& value) const {
        Node* result = nullptr;
        Node* current = m_root;
        
        while (current != nullptr) {
            if (!m_compare(current->data, value)) {
                result = current;
                current = current->left;
            } else {
                current = current->right;
            }
        }
        
        return iterator(result, this);
    }
    
    /**
     * @brief Find first element greater than value
     */
    iterator upper_bound(const T& value) const {
        Node* result = nullptr;
        Node* current = m_root;
        
        while (current != nullptr) {
            if (m_compare(value, current->data)) {
                result = current;
                current = current->left;
            } else {
                current = current->right;
            }
        }
        
        return iterator(result, this);
    }
    
    // ============================================
    // Iterators
    // ============================================
    
    /**
     * @brief Get iterator to beginning (minimum element)
     */
    iterator begin() const {
        if (m_root == nullptr) {
            return end();
        }
        return iterator(find_min(m_root), this);
    }
    
    /**
     * @brief Get iterator to end
     */
    iterator end() const {
        return iterator(nullptr, this);
    }
    
    const_iterator cbegin() const { return begin(); }
    const_iterator cend() const { return end(); }
    
    // ============================================
    // Traversals
    // ============================================
    
    /**
     * @brief Inorder traversal
     */
    template <typename Func>
    void inorder(Func func) const {
        inorder_impl(m_root, func);
    }
    
    /**
     * @brief Preorder traversal
     */
    template <typename Func>
    void preorder(Func func) const {
        preorder_impl(m_root, func);
    }
    
    /**
     * @brief Postorder traversal
     */
    template <typename Func>
    void postorder(Func func) const {
        postorder_impl(m_root, func);
    }
    
    /**
     * @brief Level order traversal
     */
    template <typename Func>
    void level_order(Func func) const {
        if (m_root == nullptr) return;
        
        std::queue<Node*> q;
        q.push(m_root);
        
        while (!q.empty()) {
            Node* node = q.front();
            q.pop();
            
            func(node->data);
            
            if (node->left != nullptr) q.push(node->left);
            if (node->right != nullptr) q.push(node->right);
        }
    }
    
    /**
     * @brief Get all elements as vector (inorder)
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
     * @brief Get height of the tree
     */
    std::size_t height() const {
        return height_impl(m_root);
    }
    
    /**
     * @brief Get black height (number of black nodes from root to any leaf)
     */
    std::size_t black_height() const {
        return black_height_impl(m_root);
    }
    
    /**
     * @brief Verify red-black tree properties
     * @return true if all properties are satisfied
     */
    bool verify() const {
        if (m_root == nullptr) return true;
        
        // Property 2: Root is black
        if (m_root->color != Color::BLACK) return false;
        
        // Check other properties
        int black_count = -1;
        return verify_impl(m_root, 0, black_count);
    }
    
    /**
     * @brief Get the root value (for testing)
     */
    const T& root() const {
        if (empty()) {
            throw std::runtime_error("Tree is empty");
        }
        return m_root->data;
    }
    
    /**
     * @brief Check if root is black
     */
    bool is_root_black() const {
        return m_root == nullptr || m_root->color == Color::BLACK;
    }

private:
    Node* m_root;
    std::size_t m_size;
    Compare m_compare;
    
    // ============================================
    // Helper Functions
    // ============================================
    
    /**
     * @brief Find node with given value
     */
    Node* find_node(const T& value) const {
        Node* current = m_root;
        
        while (current != nullptr) {
            if (m_compare(value, current->data)) {
                current = current->left;
            } else if (m_compare(current->data, value)) {
                current = current->right;
            } else {
                return current;
            }
        }
        
        return nullptr;
    }
    
    /**
     * @brief Find minimum node in subtree
     */
    static Node* find_min(Node* node) {
        while (node->left != nullptr) {
            node = node->left;
        }
        return node;
    }
    
    /**
     * @brief Find maximum node in subtree
     */
    static Node* find_max(Node* node) {
        while (node->right != nullptr) {
            node = node->right;
        }
        return node;
    }
    
    /**
     * @brief Get node color (nil nodes are black)
     */
    static Color get_color(Node* node) {
        return (node == nullptr) ? Color::BLACK : node->color;
    }
    
    /**
     * @brief Set node color
     */
    static void set_color(Node* node, Color color) {
        if (node != nullptr) {
            node->color = color;
        }
    }
    
    // ============================================
    // Rotations
    // ============================================
    
    /**
     * @brief Left rotation
     *       x                y
     *      / \              / \
     *     a   y    =>      x   c
     *        / \          / \
     *       b   c        a   b
     */
    void rotate_left(Node* x) {
        Node* y = x->right;
        x->right = y->left;
        
        if (y->left != nullptr) {
            y->left->parent = x;
        }
        
        y->parent = x->parent;
        
        if (x->parent == nullptr) {
            m_root = y;
        } else if (x == x->parent->left) {
            x->parent->left = y;
        } else {
            x->parent->right = y;
        }
        
        y->left = x;
        x->parent = y;
    }
    
    /**
     * @brief Right rotation
     *         y              x
     *        / \            / \
     *       x   c    =>    a   y
     *      / \                / \
     *     a   b              b   c
     */
    void rotate_right(Node* y) {
        Node* x = y->left;
        y->left = x->right;
        
        if (x->right != nullptr) {
            x->right->parent = y;
        }
        
        x->parent = y->parent;
        
        if (y->parent == nullptr) {
            m_root = x;
        } else if (y == y->parent->left) {
            y->parent->left = x;
        } else {
            y->parent->right = x;
        }
        
        x->right = y;
        y->parent = x;
    }
    
    // ============================================
    // Insert Implementation
    // ============================================
    
    template <typename ValueType>
    std::pair<iterator, bool> insert_impl(ValueType&& value) {
        // Standard BST insert
        Node* parent = nullptr;
        Node* current = m_root;
        
        while (current != nullptr) {
            parent = current;
            if (m_compare(value, current->data)) {
                current = current->left;
            } else if (m_compare(current->data, value)) {
                current = current->right;
            } else {
                // Duplicate found
                return {iterator(current, this), false};
            }
        }
        
        // Create new red node
        Node* new_node = new Node(std::forward<ValueType>(value), Color::RED);
        new_node->parent = parent;
        
        if (parent == nullptr) {
            m_root = new_node;
        } else if (m_compare(new_node->data, parent->data)) {
            parent->left = new_node;
        } else {
            parent->right = new_node;
        }
        
        ++m_size;
        
        // Fix red-black properties
        insert_fixup(new_node);
        
        return {iterator(new_node, this), true};
    }
    
    /**
     * @brief Fix red-black properties after insertion
     */
    void insert_fixup(Node* z) {
        while (z->parent != nullptr && z->parent->color == Color::RED) {
            if (z->parent == z->parent->parent->left) {
                Node* uncle = z->parent->parent->right;
                
                if (get_color(uncle) == Color::RED) {
                    // Case 1: Uncle is red
                    z->parent->color = Color::BLACK;
                    set_color(uncle, Color::BLACK);
                    z->parent->parent->color = Color::RED;
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->right) {
                        // Case 2: Uncle is black, z is right child
                        z = z->parent;
                        rotate_left(z);
                    }
                    // Case 3: Uncle is black, z is left child
                    z->parent->color = Color::BLACK;
                    z->parent->parent->color = Color::RED;
                    rotate_right(z->parent->parent);
                }
            } else {
                // Mirror cases
                Node* uncle = z->parent->parent->left;
                
                if (get_color(uncle) == Color::RED) {
                    // Case 1: Uncle is red
                    z->parent->color = Color::BLACK;
                    set_color(uncle, Color::BLACK);
                    z->parent->parent->color = Color::RED;
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->left) {
                        // Case 2: Uncle is black, z is left child
                        z = z->parent;
                        rotate_right(z);
                    }
                    // Case 3: Uncle is black, z is right child
                    z->parent->color = Color::BLACK;
                    z->parent->parent->color = Color::RED;
                    rotate_left(z->parent->parent);
                }
            }
        }
        
        m_root->color = Color::BLACK;
    }
    
    // ============================================
    // Delete Implementation
    // ============================================
    
    /**
     * @brief Transplant subtree
     */
    void transplant(Node* u, Node* v) {
        if (u->parent == nullptr) {
            m_root = v;
        } else if (u == u->parent->left) {
            u->parent->left = v;
        } else {
            u->parent->right = v;
        }
        
        if (v != nullptr) {
            v->parent = u->parent;
        }
    }
    
    /**
     * @brief Delete a node
     */
    void erase_node(Node* z) {
        Node* y = z;
        Node* x = nullptr;
        Node* x_parent = nullptr;
        Color y_original_color = y->color;
        
        if (z->left == nullptr) {
            x = z->right;
            x_parent = z->parent;
            transplant(z, z->right);
        } else if (z->right == nullptr) {
            x = z->left;
            x_parent = z->parent;
            transplant(z, z->left);
        } else {
            y = find_min(z->right);
            y_original_color = y->color;
            x = y->right;
            
            if (y->parent == z) {
                x_parent = y;
            } else {
                x_parent = y->parent;
                transplant(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }
            
            transplant(z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
        }
        
        delete z;
        --m_size;
        
        if (y_original_color == Color::BLACK) {
            erase_fixup(x, x_parent);
        }
    }
    
    /**
     * @brief Fix red-black properties after deletion
     */
    void erase_fixup(Node* x, Node* x_parent) {
        while (x != m_root && get_color(x) == Color::BLACK) {
            if (x == (x_parent ? x_parent->left : nullptr)) {
                Node* w = x_parent->right;
                
                if (get_color(w) == Color::RED) {
                    // Case 1: Sibling is red
                    set_color(w, Color::BLACK);
                    set_color(x_parent, Color::RED);
                    rotate_left(x_parent);
                    w = x_parent->right;
                }
                
                if (get_color(w ? w->left : nullptr) == Color::BLACK &&
                    get_color(w ? w->right : nullptr) == Color::BLACK) {
                    // Case 2: Sibling's children are both black
                    set_color(w, Color::RED);
                    x = x_parent;
                    x_parent = x ? x->parent : nullptr;
                } else {
                    if (get_color(w ? w->right : nullptr) == Color::BLACK) {
                        // Case 3: Sibling's right child is black
                        set_color(w ? w->left : nullptr, Color::BLACK);
                        set_color(w, Color::RED);
                        if (w) rotate_right(w);
                        w = x_parent->right;
                    }
                    // Case 4: Sibling's right child is red
                    set_color(w, get_color(x_parent));
                    set_color(x_parent, Color::BLACK);
                    set_color(w ? w->right : nullptr, Color::BLACK);
                    rotate_left(x_parent);
                    x = m_root;
                    x_parent = nullptr;
                }
            } else {
                // Mirror cases
                Node* w = x_parent->left;
                
                if (get_color(w) == Color::RED) {
                    set_color(w, Color::BLACK);
                    set_color(x_parent, Color::RED);
                    rotate_right(x_parent);
                    w = x_parent->left;
                }
                
                if (get_color(w ? w->right : nullptr) == Color::BLACK &&
                    get_color(w ? w->left : nullptr) == Color::BLACK) {
                    set_color(w, Color::RED);
                    x = x_parent;
                    x_parent = x ? x->parent : nullptr;
                } else {
                    if (get_color(w ? w->left : nullptr) == Color::BLACK) {
                        set_color(w ? w->right : nullptr, Color::BLACK);
                        set_color(w, Color::RED);
                        if (w) rotate_left(w);
                        w = x_parent->left;
                    }
                    set_color(w, get_color(x_parent));
                    set_color(x_parent, Color::BLACK);
                    set_color(w ? w->left : nullptr, Color::BLACK);
                    rotate_right(x_parent);
                    x = m_root;
                    x_parent = nullptr;
                }
            }
        }
        
        set_color(x, Color::BLACK);
    }
    
    // ============================================
    // Tree Operations
    // ============================================
    
    /**
     * @brief Deep copy a subtree
     */
    Node* copy_tree(Node* node, Node* parent) {
        if (node == nullptr) return nullptr;
        
        Node* new_node = new Node(node->data, node->color);
        new_node->parent = parent;
        new_node->left = copy_tree(node->left, new_node);
        new_node->right = copy_tree(node->right, new_node);
        
        return new_node;
    }
    
    /**
     * @brief Destroy a subtree
     */
    void destroy_tree(Node* node) {
        if (node == nullptr) return;
        
        destroy_tree(node->left);
        destroy_tree(node->right);
        delete node;
    }
    
    /**
     * @brief Calculate height of subtree
     */
    std::size_t height_impl(Node* node) const {
        if (node == nullptr) return 0;
        return 1 + std::max(height_impl(node->left), height_impl(node->right));
    }
    
    /**
     * @brief Calculate black height of subtree
     */
    std::size_t black_height_impl(Node* node) const {
        if (node == nullptr) return 1;  // NIL nodes count as black
        
        std::size_t left_bh = black_height_impl(node->left);
        return left_bh + (node->color == Color::BLACK ? 1 : 0);
    }
    
    /**
     * @brief Verify red-black properties
     */
    bool verify_impl(Node* node, int black_count, int& expected_black_count) const {
        if (node == nullptr) {
            // NIL node: check black height
            black_count++;  // NIL counts as black
            if (expected_black_count == -1) {
                expected_black_count = black_count;
            }
            return black_count == expected_black_count;
        }
        
        // Property 4: Red node must have black children
        if (node->color == Color::RED) {
            if (get_color(node->left) == Color::RED || 
                get_color(node->right) == Color::RED) {
                return false;
            }
        } else {
            black_count++;
        }
        
        // Check both subtrees
        return verify_impl(node->left, black_count, expected_black_count) &&
               verify_impl(node->right, black_count, expected_black_count);
    }
    
    // ============================================
    // Traversal Implementations
    // ============================================
    
    template <typename Func>
    void inorder_impl(Node* node, Func& func) const {
        if (node == nullptr) return;
        inorder_impl(node->left, func);
        func(node->data);
        inorder_impl(node->right, func);
    }
    
    template <typename Func>
    void preorder_impl(Node* node, Func& func) const {
        if (node == nullptr) return;
        func(node->data);
        preorder_impl(node->left, func);
        preorder_impl(node->right, func);
    }
    
    template <typename Func>
    void postorder_impl(Node* node, Func& func) const {
        if (node == nullptr) return;
        postorder_impl(node->left, func);
        postorder_impl(node->right, func);
        func(node->data);
    }
};

} // namespace tree
} // namespace mylib

#endif // MYLIB_TREE_RED_BLACK_TREE_HPP