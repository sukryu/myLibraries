/**
 * @file dynamic_array_impl.hpp
 * @brief Implementation of DynamicArray member functions
 * @author Jinhyeok
 * @date 2025-12-06
 * @version 2.0.0
 */

#ifndef MYLIB_LINEAR_DYNAMIC_ARRAY_IMPL_HPP
#define MYLIB_LINEAR_DYNAMIC_ARRAY_IMPL_HPP

#include "dynamic_array.hpp"
#include <algorithm>
#include <memory>
#include <new>

namespace mylib {
namespace linear {

// ============================================
// Constructors & Destructor
// ============================================

template <typename T>
DynamicArray<T>::DynamicArray() noexcept
    : m_data(nullptr), m_size(0), m_capacity(0) {
}

template <typename T>
DynamicArray<T>::DynamicArray(size_type initial_capacity)
    : m_data(nullptr), m_size(0), m_capacity(0) {
    if (initial_capacity > 0) {
        m_data = new T[initial_capacity];
        m_capacity = initial_capacity;
    }
}

template <typename T>
DynamicArray<T>::DynamicArray(size_type count, const T& value)
    : m_data(nullptr), m_size(0), m_capacity(0) {
    if (count > 0) {
        m_data = new T[count];
        m_capacity = count;
        
        try {
            for (size_type i = 0; i < count; ++i) {
                m_data[i] = value;
                ++m_size;
            }
        } catch (...) {
            // Exception safety: clean up and rethrow
            delete[] m_data;
            m_data = nullptr;
            m_size = 0;
            m_capacity = 0;
            throw;
        }
    }
}

template <typename T>
DynamicArray<T>::DynamicArray(std::initializer_list<T> init)
    : m_data(nullptr), m_size(0), m_capacity(0) {
    if (init.size() > 0) {
        m_data = new T[init.size()];
        m_capacity = init.size();
        
        try {
            for (const auto& item : init) {
                m_data[m_size++] = item;
            }
        } catch (...) {
            delete[] m_data;
            m_data = nullptr;
            m_size = 0;
            m_capacity = 0;
            throw;
        }
    }
}

template <typename T>
DynamicArray<T>::DynamicArray(const DynamicArray& other)
    : m_data(nullptr), m_size(0), m_capacity(0) {
    if (other.m_capacity > 0) {
        m_data = new T[other.m_capacity];
        m_capacity = other.m_capacity;
        
        try {
            for (size_type i = 0; i < other.m_size; ++i) {
                m_data[i] = other.m_data[i];
                ++m_size;
            }
        } catch (...) {
            delete[] m_data;
            m_data = nullptr;
            m_size = 0;
            m_capacity = 0;
            throw;
        }
    }
}

template <typename T>
DynamicArray<T>::DynamicArray(DynamicArray&& other) noexcept
    : m_data(other.m_data)
    , m_size(other.m_size)
    , m_capacity(other.m_capacity) {
    // Leave other in valid but empty state
    other.m_data = nullptr;
    other.m_size = 0;
    other.m_capacity = 0;
}

template <typename T>
DynamicArray<T>::~DynamicArray() noexcept {
    delete[] m_data;
}

// ============================================
// Assignment Operators
// ============================================

template <typename T>
DynamicArray<T>& DynamicArray<T>::operator=(const DynamicArray& other) {
    if (this != &other) {
        // Create temporary and swap (strong exception guarantee)
        DynamicArray temp(other);
        swap(temp);
    }
    return *this;
}

template <typename T>
DynamicArray<T>& DynamicArray<T>::operator=(DynamicArray&& other) noexcept {
    if (this != &other) {
        // Clean up our resources
        delete[] m_data;
        
        // Take ownership of other's resources
        m_data = other.m_data;
        m_size = other.m_size;
        m_capacity = other.m_capacity;
        
        // Leave other in valid but empty state
        other.m_data = nullptr;
        other.m_size = 0;
        other.m_capacity = 0;
    }
    return *this;
}

template <typename T>
DynamicArray<T>& DynamicArray<T>::operator=(std::initializer_list<T> init) {
    // Create temporary and swap
    DynamicArray temp(init);
    swap(temp);
    return *this;
}

// ============================================
// Element Access
// ============================================

template <typename T>
typename DynamicArray<T>::reference DynamicArray<T>::at(size_type index) {
    if (index >= m_size) {
        throw std::out_of_range("DynamicArray::at: index out of range");
    }
    return m_data[index];
}

template <typename T>
typename DynamicArray<T>::const_reference DynamicArray<T>::at(size_type index) const {
    if (index >= m_size) {
        throw std::out_of_range("DynamicArray::at: index out of range");
    }
    return m_data[index];
}

template <typename T>
typename DynamicArray<T>::reference DynamicArray<T>::operator[](size_type index) noexcept {
    return m_data[index];
}

template <typename T>
typename DynamicArray<T>::const_reference DynamicArray<T>::operator[](size_type index) const noexcept {
    return m_data[index];
}

template <typename T>
typename DynamicArray<T>::reference DynamicArray<T>::front() noexcept {
    return m_data[0];
}

template <typename T>
typename DynamicArray<T>::const_reference DynamicArray<T>::front() const noexcept {
    return m_data[0];
}

template <typename T>
typename DynamicArray<T>::reference DynamicArray<T>::back() noexcept {
    return m_data[m_size - 1];
}

template <typename T>
typename DynamicArray<T>::const_reference DynamicArray<T>::back() const noexcept {
    return m_data[m_size - 1];
}

template <typename T>
typename DynamicArray<T>::pointer DynamicArray<T>::data() noexcept {
    return m_data;
}

template <typename T>
typename DynamicArray<T>::const_pointer DynamicArray<T>::data() const noexcept {
    return m_data;
}

// ============================================
// Iterators
// ============================================

template <typename T>
typename DynamicArray<T>::iterator DynamicArray<T>::begin() noexcept {
    return m_data;
}

template <typename T>
typename DynamicArray<T>::const_iterator DynamicArray<T>::begin() const noexcept {
    return m_data;
}

template <typename T>
typename DynamicArray<T>::const_iterator DynamicArray<T>::cbegin() const noexcept {
    return m_data;
}

template <typename T>
typename DynamicArray<T>::iterator DynamicArray<T>::end() noexcept {
    return m_data + m_size;
}

template <typename T>
typename DynamicArray<T>::const_iterator DynamicArray<T>::end() const noexcept {
    return m_data + m_size;
}

template <typename T>
typename DynamicArray<T>::const_iterator DynamicArray<T>::cend() const noexcept {
    return m_data + m_size;
}

template <typename T>
typename DynamicArray<T>::reverse_iterator DynamicArray<T>::rbegin() noexcept {
    return reverse_iterator(end());
}

template <typename T>
typename DynamicArray<T>::const_reverse_iterator DynamicArray<T>::rbegin() const noexcept {
    return const_reverse_iterator(end());
}

template <typename T>
typename DynamicArray<T>::const_reverse_iterator DynamicArray<T>::crbegin() const noexcept {
    return const_reverse_iterator(cend());
}

template <typename T>
typename DynamicArray<T>::reverse_iterator DynamicArray<T>::rend() noexcept {
    return reverse_iterator(begin());
}

template <typename T>
typename DynamicArray<T>::const_reverse_iterator DynamicArray<T>::rend() const noexcept {
    return const_reverse_iterator(begin());
}

template <typename T>
typename DynamicArray<T>::const_reverse_iterator DynamicArray<T>::crend() const noexcept {
    return const_reverse_iterator(cbegin());
}

// ============================================
// Capacity
// ============================================

template <typename T>
bool DynamicArray<T>::empty() const noexcept {
    return m_size == 0;
}

template <typename T>
typename DynamicArray<T>::size_type DynamicArray<T>::size() const noexcept {
    return m_size;
}

template <typename T>
typename DynamicArray<T>::size_type DynamicArray<T>::capacity() const noexcept {
    return m_capacity;
}

template <typename T>
void DynamicArray<T>::reserve(size_type new_capacity) {
    if (new_capacity > m_capacity) {
        reallocate(new_capacity);
    }
}

template <typename T>
void DynamicArray<T>::shrink_to_fit() {
    if (m_size < m_capacity) {
        if (m_size == 0) {
            delete[] m_data;
            m_data = nullptr;
            m_capacity = 0;
        } else {
            reallocate(m_size);
        }
    }
}

// ============================================
// Modifiers
// ============================================

template <typename T>
void DynamicArray<T>::clear() noexcept {
    m_size = 0;
    // Note: Capacity is unchanged (like std::vector)
}

template <typename T>
void DynamicArray<T>::push_back(const T& value) {
    if (m_size >= m_capacity) {
        grow();
    }
    m_data[m_size++] = value;
}

template <typename T>
void DynamicArray<T>::push_back(T&& value) {
    if (m_size >= m_capacity) {
        grow();
    }
    m_data[m_size++] = std::move(value);
}

template <typename T>
template <typename... Args>
typename DynamicArray<T>::reference DynamicArray<T>::emplace_back(Args&&... args) {
    if (m_size >= m_capacity) {
        grow();
    }
    
    // Placement new for in-place construction
    new (&m_data[m_size]) T(std::forward<Args>(args)...);
    ++m_size;
    
    return m_data[m_size - 1];
}

template <typename T>
void DynamicArray<T>::pop_back() noexcept {
    if (m_size > 0) {
        --m_size;
        // Explicitly destroy element (important for non-trivial types)
        m_data[m_size].~T();
    }
}

template <typename T>
void DynamicArray<T>::resize(size_type count) {
    if (count > m_capacity) {
        reallocate(count);
    }
    
    // Default-initialize new elements if growing
    for (size_type i = m_size; i < count; ++i) {
        new (&m_data[i]) T();
    }
    
    // Destroy excess elements if shrinking
    for (size_type i = count; i < m_size; ++i) {
        m_data[i].~T();
    }
    
    m_size = count;
}

template <typename T>
void DynamicArray<T>::resize(size_type count, const T& value) {
    if (count > m_capacity) {
        reallocate(count);
    }
    
    // Fill new elements if growing
    for (size_type i = m_size; i < count; ++i) {
        m_data[i] = value;
    }
    
    // Destroy excess elements if shrinking
    for (size_type i = count; i < m_size; ++i) {
        m_data[i].~T();
    }
    
    m_size = count;
}

template <typename T>
void DynamicArray<T>::swap(DynamicArray& other) noexcept {
    std::swap(m_data, other.m_data);
    std::swap(m_size, other.m_size);
    std::swap(m_capacity, other.m_capacity);
}

// ============================================
// Comparison Operators
// ============================================

template <typename T>
bool DynamicArray<T>::operator==(const DynamicArray& other) const {
    if (m_size != other.m_size) {
        return false;
    }
    return std::equal(begin(), end(), other.begin());
}

template <typename T>
bool DynamicArray<T>::operator!=(const DynamicArray& other) const {
    return !(*this == other);
}

template <typename T>
bool DynamicArray<T>::operator<(const DynamicArray& other) const {
    return std::lexicographical_compare(begin(), end(), other.begin(), other.end());
}

template <typename T>
bool DynamicArray<T>::operator<=(const DynamicArray& other) const {
    return !(other < *this);
}

template <typename T>
bool DynamicArray<T>::operator>(const DynamicArray& other) const {
    return other < *this;
}

template <typename T>
bool DynamicArray<T>::operator>=(const DynamicArray& other) const {
    return !(*this < other);
}

// ============================================
// Private Helper Methods
// ============================================

template <typename T>
typename DynamicArray<T>::size_type 
DynamicArray<T>::calculate_growth(size_type current, size_type required) const noexcept {
    // Use growth factor (1.5) or meet requirement, whichever is larger
    size_type geometric = static_cast<size_type>(current * GROWTH_FACTOR);
    return std::max(geometric, required);
}

template <typename T>
void DynamicArray<T>::grow() {
    size_type new_capacity = (m_capacity == 0) 
        ? DEFAULT_CAPACITY 
        : calculate_growth(m_capacity, m_capacity + 1);
    reallocate(new_capacity);
}

template <typename T>
void DynamicArray<T>::reallocate(size_type new_capacity) {
    if (new_capacity == 0) {
        delete[] m_data;
        m_data = nullptr;
        m_capacity = 0;
        m_size = 0;
        return;
    }
    
    // Allocate new storage
    T* new_data = new T[new_capacity];
    
    try {
        // Move or copy existing elements
        if constexpr (detail::is_nothrow_movable_v<T>) {
            // Move elements if noexcept
            for (size_type i = 0; i < m_size; ++i) {
                new_data[i] = std::move(m_data[i]);
            }
        } else {
            // Copy if move can throw
            for (size_type i = 0; i < m_size; ++i) {
                new_data[i] = m_data[i];
            }
        }
    } catch (...) {
        // Exception during copy/move - clean up and rethrow
        delete[] new_data;
        throw;
    }
    
    // Success - clean up old storage
    delete[] m_data;
    m_data = new_data;
    m_capacity = new_capacity;
    
    // Adjust size if shrinking
    if (m_size > new_capacity) {
        m_size = new_capacity;
    }
}

template <typename T>
void DynamicArray<T>::destroy_range(pointer first, pointer last) noexcept {
    for (pointer p = first; p != last; ++p) {
        p->~T();
    }
}

// ============================================
// Non-member Functions
// ============================================

template <typename T>
void swap(DynamicArray<T>& lhs, DynamicArray<T>& rhs) noexcept {
    lhs.swap(rhs);
}

} // namespace linear
} // namespace mylib

#endif // MYLIB_LINEAR_DYNAMIC_ARRAY_IMPL_HPP