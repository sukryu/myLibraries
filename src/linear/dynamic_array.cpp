/**
 * @file dynamic_array.cpp
 * @brief Implementation of DynamicArray class
 * @author jinhyeok
 * @date 2025-11-16
 * @version 1.0.0
 */

#include "linear/dynamic_array.hpp"
#include <algorithm>    // for std::copy, std::move
#include <new>          // for placement new

namespace mylib {
namespace linear {

// Constructors
template <typename T>
DynamicArray<T>::DynamicArray()
    : m_data(nullptr), m_size(0), m_capacity(0) {
}

template <typename T>
DynamicArray<T>::DynamicArray(size_type initial_capacity)
    : m_data(nullptr), m_size(0), m_capacity(initial_capacity) {
    if (initial_capacity > 0) {
        m_data = new T[initial_capacity];
    }
}

template <typename T>
DynamicArray<T>::DynamicArray(size_type count, const T& value)
    : m_data(nullptr), m_size(count), m_capacity(count) {
    if (count > 0) {
        m_data = new T[count];
        for (size_type i = 0; i < count; ++i) {
            m_data[i] = value;
        }
    }
}

template <typename T>
DynamicArray<T>::DynamicArray(std::initializer_list<T> init)
    : m_data(nullptr), m_size(init.size()), m_capacity(init.size()) {
    if (init.size() > 0) {
        m_data = new T[init.size()];
        size_type i = 0;
        for (const auto& item : init) {
            m_data[i++] = item;
        }
    }
}

template <typename T>
DynamicArray<T>::DynamicArray(const DynamicArray& other)
    : m_data(nullptr), m_size(other.m_size), m_capacity(other.m_capacity) {
    if (other.m_capacity > 0) {
        m_data = new T[other.m_capacity];
        for (size_type i = 0; i < other.m_size; ++i) {
            m_data[i] = other.m_data[i];
        }
    }
}

template <typename T>
DynamicArray<T>::DynamicArray(DynamicArray&& other) noexcept
    : m_data(other.m_data), m_size(other.m_size), m_capacity(other.m_capacity) {
    other.m_data = nullptr;
    other.m_size = 0;
    other.m_capacity = 0;
}

template <typename T>
DynamicArray<T>::~DynamicArray() {
    delete[] m_data;
}

// Assignment operators
template <typename T>
DynamicArray<T>& DynamicArray<T>::operator=(const DynamicArray& other) {
    if (this != &other) {
        delete[] m_data;
        
        m_size = other.m_size;
        m_capacity = other.m_capacity;
        
        if (other.m_capacity > 0) {
            m_data = new T[other.m_capacity];
            for (size_type i = 0; i < other.m_size; ++i) {
                m_data[i] = other.m_data[i];
            }
        } else {
            m_data = nullptr;
        }
    }
    return *this;
}

template <typename T>
DynamicArray<T>& DynamicArray<T>::operator=(DynamicArray&& other) noexcept {
    if (this != &other) {
        delete[] m_data;
        
        m_data = other.m_data;
        m_size = other.m_size;
        m_capacity = other.m_capacity;
        
        other.m_data = nullptr;
        other.m_size = 0;
        other.m_capacity = 0;
    }
    return *this;
}

// Element access
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
typename DynamicArray<T>::reference DynamicArray<T>::operator[](size_type index) {
    return m_data[index];
}

template <typename T>
typename DynamicArray<T>::const_reference DynamicArray<T>::operator[](size_type index) const {
    return m_data[index];
}

template <typename T>
typename DynamicArray<T>::reference DynamicArray<T>::front() {
    return m_data[0];
}

template <typename T>
typename DynamicArray<T>::const_reference DynamicArray<T>::front() const {
    return m_data[0];
}

template <typename T>
typename DynamicArray<T>::reference DynamicArray<T>::back() {
    return m_data[m_size - 1];
}

template <typename T>
typename DynamicArray<T>::const_reference DynamicArray<T>::back() const {
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

// Capacity
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
        reallocate(m_size);
    }
}

// Modifiers
template <typename T>
void DynamicArray<T>::clear() noexcept {
    m_size = 0;
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
void DynamicArray<T>::pop_back() {
    if (m_size > 0) {
        --m_size;
    }
}

template <typename T>
void DynamicArray<T>::resize(size_type count) {
    if (count > m_capacity) {
        reallocate(count);
    }
    m_size = count;
}

template <typename T>
void DynamicArray<T>::resize(size_type count, const T& value) {
    size_type old_size = m_size;
    resize(count);
    for (size_type i = old_size; i < count; ++i) {
        m_data[i] = value;
    }
}

template <typename T>
void DynamicArray<T>::swap(DynamicArray& other) noexcept {
    std::swap(m_data, other.m_data);
    std::swap(m_size, other.m_size);
    std::swap(m_capacity, other.m_capacity);
}

// Private methods
template <typename T>
void DynamicArray<T>::grow() {
    size_type new_capacity = m_capacity == 0 
        ? DEFAULT_CAPACITY 
        : static_cast<size_type>(m_capacity * GROWTH_FACTOR);
    reallocate(new_capacity);
}

template <typename T>
void DynamicArray<T>::reallocate(size_type new_capacity) {
    T* new_data = new T[new_capacity];
    
    size_type elements_to_copy = (m_size < new_capacity) ? m_size : new_capacity;
    for (size_type i = 0; i < elements_to_copy; ++i) {
        new_data[i] = std::move(m_data[i]);
    }
    
    delete[] m_data;
    m_data = new_data;
    m_capacity = new_capacity;
    
    if (m_size > new_capacity) {
        m_size = new_capacity;
    }
}

// Explicit template instantiations for common types
template class DynamicArray<int>;
template class DynamicArray<double>;
template class DynamicArray<float>;
template class DynamicArray<char>;
template class DynamicArray<long>;
template class DynamicArray<long long>;
template class DynamicArray<unsigned int>;
template class DynamicArray<unsigned long>;
template class DynamicArray<unsigned long long>;

} // namespace linear
} // namespace mylib
