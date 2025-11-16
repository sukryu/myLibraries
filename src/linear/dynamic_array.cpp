/**
* @file dynamic_array.cpp
* @berif Implementation of DynamicArray class
* @author jinhyeok
* @date 2025-11-16
* @version 1.0.0
*/

#include "linear/dynamic_array.hpp"
#include <algorithm> // for std::move, std::copy
#include <chrono>
#include <cstdio>
#include <initializer_list>
#include <new> // for placement new
#include <stdexcept>

namespace mylib {
namespace linear {

// Constructor
template <typename T>
DynamicArray<T>::DynamicArray()
     : m_data(nullptr), m_size(0), m_capacity(0) {
} 

template <typename T>
DynamicArray<T>::DynamicArray(size_type initial_capacity)
     : m_data(nullptr), m_size(0), m_capacity(0) {
    if (initial_capacity > 0) {
        m_data = new T[initial_capacity];
    }
}

template <typename T>
DynamicArray<T>::DynamicArray(size_type count, const T& value)
    : m_data(nullptr), m_size(0), m_capacity(0) {
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
DynamicArray<T>::DynamicArray<T>::operator=(const DynamicArray& other) {
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
DynamicArray<T>::DynamicArray<T>::operator=(const DynamicArray&& other) {
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
        std::out_of_range("DynamicArray::at: index out of range");
    }
    return m_data[index];
}

template <typename T>
typename DynamicArray<T>::const_reference DynamicArray<T>::at(size_type index) const {
    if (index >= m_size) {
        std::out_of_range("DynamicArray::at index out of range");
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
typename DynamicArray<T>::reference DynamicArray<T>::data() noexcept {
    return m_data;
}

template <typename T>
typename DynamicArray<T>::const_reference DynamicArray<T>::data() const noexcept {
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


}
}


