/**
 * @file skip_list.hpp
 * @brief Skip List data structure implementation
 * @author Jinhyeok
 * @date 2025-12-02
 * @version 1.0.0
 * 
 * Skip List is a probabilistic data structure that allows O(log n) search,
 * insertion, and deletion operations on average, similar to balanced trees
 * but with simpler implementation.
 * 
 * Key Features:
 * - Probabilistic balancing (no rotations needed)
 * - Multiple levels of forward pointers
 * - Random level assignment using coin flips
 * - Average O(log n) time complexity
 * - Simple to implement compared to Red-Black or AVL trees
 * 
 * Time Complexity (average):
 * - search: O(log n)
 * - insert: O(log n)
 * - remove: O(log n)
 * - lower_bound/upper_bound: O(log n)
 * 
 * Space Complexity: O(n)
 * 
 * Copyright (c) 2025 Jinhyeok
 * MIT License
 */

#ifndef MYLIB_TREE_SKIP_LIST_HPP
#define MYLIB_TREE_SKIP_LIST_HPP

#include <vector>
#include <functional>
#include <random>
#include <memory>
#include <optional>
#include <stdexcept>
#include <algorithm>
#include <limits>

namespace mylib {
namespace tree {

/**
 * @class SkipList
 * @brief A probabilistic balanced search structure
 * 
 * Skip List maintains multiple levels of linked lists where:
 * - Level 0 contains all elements
 * - Each higher level contains approximately half the elements of the level below
 * - Search starts from the highest level and drops down when needed
 * 
 * @tparam T Value type
 * @tparam Compare Comparison function (default: std::less<T>)
 * 
 * Usage:
 * @code
 * SkipList<int> list;
 * list.insert(5);
 * list.insert(3);
 * list.insert(7);
 * 
 * if (list.find(5)) {
 *     std::cout << "Found 5" << std::endl;
 * }
 * 
 * list.remove(3);
 * 
 * // Iterator support
 * for (const auto& val : list) {
 *     std::cout << val << " ";
 * }
 * @endcode
 */
template <typename T, typename Compare = std::less<T>>
class SkipList {
private:
    /**
     * @struct Node
     * @brief Internal node structure
     */
    struct Node {
        T value;
        std::vector<Node*> forward;  // Forward pointers for each level
        
        Node(const T& val, int level)
            : value(val), forward(level + 1, nullptr) {}
        
        Node(T&& val, int level)
            : value(std::move(val)), forward(level + 1, nullptr) {}
    };
    
    Node* m_head;                    // Sentinel head node
    int m_max_level;                 // Maximum level in the skip list
    int m_current_level;             // Current highest level with elements
    std::size_t m_size;              // Number of elements
    Compare m_compare;               // Comparison function
    double m_probability;            // Probability for level promotion (default: 0.5)
    
    // Random number generation
    std::mt19937 m_rng;
    std::uniform_real_distribution<double> m_dist;
    
public:
    // ============================================
    // Iterator
    // ============================================
    
    class Iterator {
    private:
        Node* m_node;
        
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = const T*;
        using reference = const T&;
        
        Iterator(Node* node) : m_node(node) {}
        
        reference operator*() const { return m_node->value; }
        pointer operator->() const { return &m_node->value; }
        
        Iterator& operator++() {
            m_node = m_node->forward[0];
            return *this;
        }
        
        Iterator operator++(int) {
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }
        
        bool operator==(const Iterator& other) const {
            return m_node == other.m_node;
        }
        
        bool operator!=(const Iterator& other) const {
            return m_node != other.m_node;
        }
    };
    
    using iterator = Iterator;
    using const_iterator = Iterator;
    
    // ============================================
    // Constructors
    // ============================================
    
    /**
     * @brief Default constructor
     * @param max_level Maximum number of levels (default: 16)
     * @param probability Probability for level promotion (default: 0.5)
     * @param compare Comparison function
     */
    explicit SkipList(int max_level = 16,
                      double probability = 0.5,
                      const Compare& compare = Compare())
        : m_max_level(max_level),
          m_current_level(0),
          m_size(0),
          m_compare(compare),
          m_probability(probability),
          m_rng(std::random_device{}()),
          m_dist(0.0, 1.0) {
        
        if (max_level < 1) {
            throw std::invalid_argument("max_level must be at least 1");
        }
        if (probability <= 0.0 || probability >= 1.0) {
            throw std::invalid_argument("probability must be in (0, 1)");
        }
        
        // Create sentinel head node with lowest possible value
        m_head = new Node(T(), max_level);
    }
    
    /**
     * @brief Constructor from initializer list
     */
    SkipList(std::initializer_list<T> init,
             int max_level = 16,
             double probability = 0.5,
             const Compare& compare = Compare())
        : SkipList(max_level, probability, compare) {
        for (const auto& val : init) {
            insert(val);
        }
    }
    
    /**
     * @brief Constructor from range
     */
    template <typename InputIt>
    SkipList(InputIt first, InputIt last,
             int max_level = 16,
             double probability = 0.5,
             const Compare& compare = Compare())
        : SkipList(max_level, probability, compare) {
        for (auto it = first; it != last; ++it) {
            insert(*it);
        }
    }
    
    /**
     * @brief Copy constructor
     */
    SkipList(const SkipList& other)
        : SkipList(other.m_max_level, other.m_probability, other.m_compare) {
        for (const auto& val : other) {
            insert(val);
        }
    }
    
    /**
     * @brief Move constructor
     */
    SkipList(SkipList&& other) noexcept
        : m_head(other.m_head),
          m_max_level(other.m_max_level),
          m_current_level(other.m_current_level),
          m_size(other.m_size),
          m_compare(std::move(other.m_compare)),
          m_probability(other.m_probability),
          m_rng(std::move(other.m_rng)),
          m_dist(std::move(other.m_dist)) {
        
        other.m_head = nullptr;
        other.m_size = 0;
        other.m_current_level = 0;
    }
    
    /**
     * @brief Destructor
     */
    ~SkipList() {
        if (m_head != nullptr) {
            clear();
            delete m_head;
        }
    }
    
    // ============================================
    // Assignment operators
    // ============================================
    
    SkipList& operator=(const SkipList& other) {
        if (this != &other) {
            SkipList tmp(other);
            swap(tmp);
        }
        return *this;
    }
    
    SkipList& operator=(SkipList&& other) noexcept {
        if (this != &other) {
            clear();
            delete m_head;
            
            m_head = other.m_head;
            m_max_level = other.m_max_level;
            m_current_level = other.m_current_level;
            m_size = other.m_size;
            m_compare = std::move(other.m_compare);
            m_probability = other.m_probability;
            m_rng = std::move(other.m_rng);
            m_dist = std::move(other.m_dist);
            
            other.m_head = nullptr;
            other.m_size = 0;
            other.m_current_level = 0;
        }
        return *this;
    }
    
    // ============================================
    // Modifiers
    // ============================================
    
    /**
     * @brief Insert value into skip list
     * @param value Value to insert
     * @return true if inserted, false if already exists
     */
    bool insert(const T& value) {
        std::vector<Node*> update(m_max_level + 1);
        Node* current = m_head;
        
        // Find position to insert
        for (int i = m_current_level; i >= 0; --i) {
            while (current->forward[i] != nullptr &&
                   m_compare(current->forward[i]->value, value)) {
                current = current->forward[i];
            }
            update[i] = current;
        }
        
        current = current->forward[0];
        
        // Check if value already exists
        if (current != nullptr && !m_compare(value, current->value) &&
            !m_compare(current->value, value)) {
            return false;  // Duplicate
        }
        
        // Generate random level
        int new_level = random_level();
        
        // Update current level if necessary
        if (new_level > m_current_level) {
            for (int i = m_current_level + 1; i <= new_level; ++i) {
                update[i] = m_head;
            }
            m_current_level = new_level;
        }
        
        // Create new node
        Node* new_node = new Node(value, new_level);
        
        // Insert node by updating pointers
        for (int i = 0; i <= new_level; ++i) {
            new_node->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = new_node;
        }
        
        ++m_size;
        return true;
    }
    
    /**
     * @brief Insert value (move version)
     */
    bool insert(T&& value) {
        std::vector<Node*> update(m_max_level + 1);
        Node* current = m_head;
        
        for (int i = m_current_level; i >= 0; --i) {
            while (current->forward[i] != nullptr &&
                   m_compare(current->forward[i]->value, value)) {
                current = current->forward[i];
            }
            update[i] = current;
        }
        
        current = current->forward[0];
        
        if (current != nullptr && !m_compare(value, current->value) &&
            !m_compare(current->value, value)) {
            return false;
        }
        
        int new_level = random_level();
        
        if (new_level > m_current_level) {
            for (int i = m_current_level + 1; i <= new_level; ++i) {
                update[i] = m_head;
            }
            m_current_level = new_level;
        }
        
        Node* new_node = new Node(std::move(value), new_level);
        
        for (int i = 0; i <= new_level; ++i) {
            new_node->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = new_node;
        }
        
        ++m_size;
        return true;
    }
    
    /**
     * @brief Remove value from skip list
     * @param value Value to remove
     * @return true if removed, false if not found
     */
    bool remove(const T& value) {
        std::vector<Node*> update(m_max_level + 1);
        Node* current = m_head;
        
        // Find the node to delete
        for (int i = m_current_level; i >= 0; --i) {
            while (current->forward[i] != nullptr &&
                   m_compare(current->forward[i]->value, value)) {
                current = current->forward[i];
            }
            update[i] = current;
        }
        
        current = current->forward[0];
        
        // Check if node exists
        if (current == nullptr || m_compare(value, current->value) ||
            m_compare(current->value, value)) {
            return false;  // Not found
        }
        
        // Remove node by updating pointers
        for (int i = 0; i <= m_current_level; ++i) {
            if (update[i]->forward[i] != current) {
                break;
            }
            update[i]->forward[i] = current->forward[i];
        }
        
        delete current;
        
        // Update current level
        while (m_current_level > 0 && m_head->forward[m_current_level] == nullptr) {
            --m_current_level;
        }
        
        --m_size;
        return true;
    }
    
    /**
     * @brief Clear all elements
     */
    void clear() {
        if (m_head == nullptr) return;
        
        Node* current = m_head->forward[0];
        while (current != nullptr) {
            Node* next = current->forward[0];
            delete current;
            current = next;
        }
        
        for (int i = 0; i <= m_max_level; ++i) {
            m_head->forward[i] = nullptr;
        }
        
        m_current_level = 0;
        m_size = 0;
    }
    
    // ============================================
    // Lookup operations
    // ============================================
    
    /**
     * @brief Find value in skip list
     * @param value Value to find
     * @return true if found
     */
    bool find(const T& value) const {
        Node* current = m_head;
        
        for (int i = m_current_level; i >= 0; --i) {
            while (current->forward[i] != nullptr &&
                   m_compare(current->forward[i]->value, value)) {
                current = current->forward[i];
            }
        }
        
        current = current->forward[0];
        
        return current != nullptr && !m_compare(value, current->value) &&
               !m_compare(current->value, value);
    }
    
    /**
     * @brief Check if value exists
     */
    bool contains(const T& value) const {
        return find(value);
    }
    
    /**
     * @brief Find first element not less than value
     * @return Optional containing the value if found
     */
    std::optional<T> lower_bound(const T& value) const {
        Node* current = m_head;
        
        for (int i = m_current_level; i >= 0; --i) {
            while (current->forward[i] != nullptr &&
                   m_compare(current->forward[i]->value, value)) {
                current = current->forward[i];
            }
        }
        
        current = current->forward[0];
        
        if (current != nullptr) {
            return current->value;
        }
        return std::nullopt;
    }
    
    /**
     * @brief Find first element greater than value
     * @return Optional containing the value if found
     */
    std::optional<T> upper_bound(const T& value) const {
        Node* current = m_head;
        
        for (int i = m_current_level; i >= 0; --i) {
            while (current->forward[i] != nullptr &&
                   !m_compare(value, current->forward[i]->value)) {
                current = current->forward[i];
            }
        }
        
        current = current->forward[0];
        
        if (current != nullptr) {
            return current->value;
        }
        return std::nullopt;
    }
    
    // ============================================
    // Capacity
    // ============================================
    
    std::size_t size() const noexcept { return m_size; }
    bool empty() const noexcept { return m_size == 0; }
    int max_level() const noexcept { return m_max_level; }
    int current_level() const noexcept { return m_current_level; }
    
    // ============================================
    // Iterators
    // ============================================
    
    iterator begin() const { return iterator(m_head->forward[0]); }
    iterator end() const { return iterator(nullptr); }
    const_iterator cbegin() const { return const_iterator(m_head->forward[0]); }
    const_iterator cend() const { return const_iterator(nullptr); }
    
    // ============================================
    // Utility
    // ============================================
    
    /**
     * @brief Swap with another skip list
     */
    void swap(SkipList& other) noexcept {
        std::swap(m_head, other.m_head);
        std::swap(m_max_level, other.m_max_level);
        std::swap(m_current_level, other.m_current_level);
        std::swap(m_size, other.m_size);
        std::swap(m_compare, other.m_compare);
        std::swap(m_probability, other.m_probability);
        std::swap(m_rng, other.m_rng);
        std::swap(m_dist, other.m_dist);
    }
    
    /**
     * @brief Get level distribution statistics
     * @return Vector where index i contains count of nodes at level i
     */
    std::vector<int> level_distribution() const {
        std::vector<int> dist(m_current_level + 1, 0);
        Node* current = m_head->forward[0];
        
        while (current != nullptr) {
            for (int i = 0; i < static_cast<int>(current->forward.size()); ++i) {
                ++dist[i];
            }
            current = current->forward[0];
        }
        
        return dist;
    }
    
    /**
     * @brief Convert to sorted vector
     */
    std::vector<T> to_vector() const {
        std::vector<T> result;
        result.reserve(m_size);
        
        for (const auto& val : *this) {
            result.push_back(val);
        }
        
        return result;
    }
    
private:
    /**
     * @brief Generate random level for new node
     */
    int random_level() {
        int level = 0;
        while (m_dist(m_rng) < m_probability && level < m_max_level) {
            ++level;
        }
        return level;
    }
};

} // namespace tree
} // namespace mylib

#endif // MYLIB_TREE_SKIP_LIST_HPP