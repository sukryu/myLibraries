/**
 * @file heap.cpp
 * @brief Implementation of Heap class
 * @author Jinhyeok
 * @date 2025-11-29
 * @version 1.0.0
 */

#include "tree/heap.hpp"
#include <algorithm>

namespace mylib {
namespace tree {

// Constructors
template <typename T, typename Compare>
Heap<T, Compare>::Heap()
    : m_data(), m_comp() {
}

template <typename T, typename Compare>
Heap<T, Compare>::Heap(const Compare& comp)
    : m_data(), m_comp(comp) {
}

template <typename T, typename Compare>
Heap<T, Compare>::Heap(size_type initial_capacity)
    : m_data(), m_comp() {
    m_data.reserve(initial_capacity);
}

template <typename T, typename Compare>
Heap<T, Compare>::Heap(size_type initial_capacity, const Compare& comp)
    : m_data(), m_comp(comp) {
    m_data.reserve(initial_capacity);
}

template <typename T, typename Compare>
Heap<T, Compare>::Heap(std::initializer_list<T> init)
    : m_data(init), m_comp() {
    heapify();
}

template <typename T, typename Compare>
Heap<T, Compare>::Heap(std::initializer_list<T> init, const Compare& comp)
    : m_data(init), m_comp(comp) {
    heapify();
}

template <typename T, typename Compare>
Heap<T, Compare>::Heap(const Heap& other)
    : m_data(other.m_data), m_comp(other.m_comp) {
}

template <typename T, typename Compare>
Heap<T, Compare>::Heap(Heap&& other) noexcept
    : m_data(std::move(other.m_data)), m_comp(std::move(other.m_comp)) {
}

// Assignment operators
template <typename T, typename Compare>
Heap<T, Compare>& Heap<T, Compare>::operator=(const Heap& other) {
    if (this != &other) {
        m_data = other.m_data;
        m_comp = other.m_comp;
    }
    return *this;
}

template <typename T, typename Compare>
Heap<T, Compare>& Heap<T, Compare>::operator=(Heap&& other) noexcept {
    if (this != &other) {
        m_data = std::move(other.m_data);
        m_comp = std::move(other.m_comp);
    }
    return *this;
}

// Capacity
template <typename T, typename Compare>
bool Heap<T, Compare>::empty() const noexcept {
    return m_data.empty();
}

template <typename T, typename Compare>
typename Heap<T, Compare>::size_type Heap<T, Compare>::size() const noexcept {
    return m_data.size();
}

template <typename T, typename Compare>
typename Heap<T, Compare>::size_type Heap<T, Compare>::capacity() const noexcept {
    return m_data.capacity();
}

template <typename T, typename Compare>
void Heap<T, Compare>::reserve(size_type new_capacity) {
    m_data.reserve(new_capacity);
}

// Element access
template <typename T, typename Compare>
typename Heap<T, Compare>::const_reference Heap<T, Compare>::top() const {
    if (empty()) {
        throw std::out_of_range("Heap::top: heap is empty");
    }
    return m_data[0];
}

// Modifiers
template <typename T, typename Compare>
void Heap<T, Compare>::push(const T& value) {
    m_data.push_back(value);
    sift_up(m_data.size() - 1);
}

template <typename T, typename Compare>
void Heap<T, Compare>::push(T&& value) {
    m_data.push_back(std::move(value));
    sift_up(m_data.size() - 1);
}

template <typename T, typename Compare>
void Heap<T, Compare>::pop() {
    if (empty()) {
        throw std::out_of_range("Heap::pop: heap is empty");
    }
    
    // Move last element to root and remove last
    m_data[0] = std::move(m_data.back());
    m_data.pop_back();
    
    // Restore heap property
    if (!empty()) {
        sift_down(0);
    }
}

template <typename T, typename Compare>
T Heap<T, Compare>::extract() {
    if (empty()) {
        throw std::out_of_range("Heap::extract: heap is empty");
    }
    
    T result = std::move(m_data[0]);
    
    // Move last element to root and remove last
    m_data[0] = std::move(m_data.back());
    m_data.pop_back();
    
    // Restore heap property
    if (!empty()) {
        sift_down(0);
    }
    
    return result;
}

template <typename T, typename Compare>
void Heap<T, Compare>::clear() noexcept {
    m_data.clear();
}

template <typename T, typename Compare>
void Heap<T, Compare>::swap(Heap& other) noexcept {
    m_data.swap(other.m_data);
    std::swap(m_comp, other.m_comp);
}

// Validation
template <typename T, typename Compare>
bool Heap<T, Compare>::is_valid() const {
    for (size_type i = 0; i < m_data.size(); ++i) {
        size_type left = left_child(i);
        size_type right = right_child(i);
        
        // For max heap: parent should not be less than children
        // compare(parent, child) should be false
        if (left < m_data.size() && compare(m_data[i], m_data[left])) {
            return false;
        }
        if (right < m_data.size() && compare(m_data[i], m_data[right])) {
            return false;
        }
    }
    return true;
}

template <typename T, typename Compare>
const std::vector<T>& Heap<T, Compare>::data() const noexcept {
    return m_data;
}

// Index calculations
template <typename T, typename Compare>
typename Heap<T, Compare>::size_type Heap<T, Compare>::parent(size_type index) noexcept {
    return (index - 1) / 2;
}

template <typename T, typename Compare>
typename Heap<T, Compare>::size_type Heap<T, Compare>::left_child(size_type index) noexcept {
    return 2 * index + 1;
}

template <typename T, typename Compare>
typename Heap<T, Compare>::size_type Heap<T, Compare>::right_child(size_type index) noexcept {
    return 2 * index + 2;
}

template <typename T, typename Compare>
bool Heap<T, Compare>::has_parent(size_type index) noexcept {
    return index > 0;
}

template <typename T, typename Compare>
bool Heap<T, Compare>::has_left_child(size_type index) const noexcept {
    return left_child(index) < m_data.size();
}

template <typename T, typename Compare>
bool Heap<T, Compare>::has_right_child(size_type index) const noexcept {
    return right_child(index) < m_data.size();
}

// Heap maintenance
template <typename T, typename Compare>
void Heap<T, Compare>::sift_up(size_type index) {
    // While not at root and parent is "less than" current (for max heap)
    while (has_parent(index) && compare(m_data[parent(index)], m_data[index])) {
        std::swap(m_data[parent(index)], m_data[index]);
        index = parent(index);
    }
}

template <typename T, typename Compare>
void Heap<T, Compare>::sift_down(size_type index) {
    while (has_left_child(index)) {
        // Find the "larger" child (for max heap)
        size_type larger_child = left_child(index);
        
        if (has_right_child(index) && compare(m_data[larger_child], m_data[right_child(index)])) {
            larger_child = right_child(index);
        }
        
        // If current is not "less than" the larger child, heap property is satisfied
        if (!compare(m_data[index], m_data[larger_child])) {
            break;
        }
        
        std::swap(m_data[index], m_data[larger_child]);
        index = larger_child;
    }
}

template <typename T, typename Compare>
void Heap<T, Compare>::heapify() {
    if (m_data.size() <= 1) {
        return;
    }
    
    // Start from last non-leaf node and sift down each node
    // Last non-leaf node is parent of last element
    for (size_type i = parent(m_data.size() - 1) + 1; i > 0; --i) {
        sift_down(i - 1);
    }
}

template <typename T, typename Compare>
bool Heap<T, Compare>::compare(const T& a, const T& b) const {
    return m_comp(a, b);
}

// Explicit template instantiations for common types

// MaxHeap (default: std::less)
template class Heap<int, std::less<int>>;
template class Heap<double, std::less<double>>;
template class Heap<float, std::less<float>>;
template class Heap<char, std::less<char>>;
template class Heap<long, std::less<long>>;
template class Heap<long long, std::less<long long>>;
template class Heap<unsigned int, std::less<unsigned int>>;
template class Heap<unsigned long, std::less<unsigned long>>;
template class Heap<unsigned long long, std::less<unsigned long long>>;

// MinHeap (std::greater)
template class Heap<int, std::greater<int>>;
template class Heap<double, std::greater<double>>;
template class Heap<float, std::greater<float>>;
template class Heap<char, std::greater<char>>;
template class Heap<long, std::greater<long>>;
template class Heap<long long, std::greater<long long>>;
template class Heap<unsigned int, std::greater<unsigned int>>;
template class Heap<unsigned long, std::greater<unsigned long>>;
template class Heap<unsigned long long, std::greater<unsigned long long>>;

} // namespace tree
} // namespace mylib