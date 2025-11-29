/**
 * @file deque.cpp
 * @brief Implementation of Deque class
 * @author Jinhyeok
 * @date 2025-11-29
 * @version 1.0.0
 */

#include "linear/deque.hpp"

namespace mylib {
namespace linear {

// Constructors
template <typename T>
Deque<T>::Deque() : m_container() {
}

template <typename T>
Deque<T>::Deque(size_type count, const T& value)
    : m_container(count, value) {
}

template <typename T>
Deque<T>::Deque(std::initializer_list<T> init)
    : m_container(init) {
}

template <typename T>
Deque<T>::Deque(const Deque& other)
    : m_container(other.m_container) {
}

template <typename T>
Deque<T>::Deque(Deque&& other) noexcept
    : m_container(std::move(other.m_container)) {
}

// Assignment operators
template <typename T>
Deque<T>& Deque<T>::operator=(const Deque& other) {
    if (this != &other) {
        m_container = other.m_container;
    }
    return *this;
}

template <typename T>
Deque<T>& Deque<T>::operator=(Deque&& other) noexcept {
    if (this != &other) {
        m_container = std::move(other.m_container);
    }
    return *this;
}

// Element access
template <typename T>
typename Deque<T>::reference Deque<T>::at(size_type index) {
    return m_container.at(index);
}

template <typename T>
typename Deque<T>::const_reference Deque<T>::at(size_type index) const {
    return m_container.at(index);
}

template <typename T>
typename Deque<T>::reference Deque<T>::operator[](size_type index) {
    return m_container[index];
}

template <typename T>
typename Deque<T>::const_reference Deque<T>::operator[](size_type index) const {
    return m_container[index];
}

template <typename T>
typename Deque<T>::reference Deque<T>::front() {
    if (empty()) {
        throw std::out_of_range("Deque::front: deque is empty");
    }
    return m_container.front();
}

template <typename T>
typename Deque<T>::const_reference Deque<T>::front() const {
    if (empty()) {
        throw std::out_of_range("Deque::front: deque is empty");
    }
    return m_container.front();
}

template <typename T>
typename Deque<T>::reference Deque<T>::back() {
    if (empty()) {
        throw std::out_of_range("Deque::back: deque is empty");
    }
    return m_container.back();
}

template <typename T>
typename Deque<T>::const_reference Deque<T>::back() const {
    if (empty()) {
        throw std::out_of_range("Deque::back: deque is empty");
    }
    return m_container.back();
}

// Capacity
template <typename T>
bool Deque<T>::empty() const noexcept {
    return m_container.empty();
}

template <typename T>
typename Deque<T>::size_type Deque<T>::size() const noexcept {
    return m_container.size();
}

// Modifiers
template <typename T>
void Deque<T>::clear() noexcept {
    m_container.clear();
}

template <typename T>
void Deque<T>::push_front(const T& value) {
    m_container.push_front(value);
}

template <typename T>
void Deque<T>::push_front(T&& value) {
    m_container.push_front(std::move(value));
}

template <typename T>
void Deque<T>::push_back(const T& value) {
    m_container.push_back(value);
}

template <typename T>
void Deque<T>::push_back(T&& value) {
    m_container.push_back(std::move(value));
}

template <typename T>
void Deque<T>::pop_front() {
    if (empty()) {
        throw std::out_of_range("Deque::pop_front: deque is empty");
    }
    m_container.pop_front();
}

template <typename T>
void Deque<T>::pop_back() {
    if (empty()) {
        throw std::out_of_range("Deque::pop_back: deque is empty");
    }
    m_container.pop_back();
}

template <typename T>
void Deque<T>::insert(size_type index, const T& value) {
    m_container.insert(index, value);
}

template <typename T>
void Deque<T>::insert(size_type index, T&& value) {
    m_container.insert(index, std::move(value));
}

template <typename T>
void Deque<T>::erase(size_type index) {
    m_container.erase(index);
}

template <typename T>
void Deque<T>::resize(size_type count) {
    m_container.resize(count);
}

template <typename T>
void Deque<T>::resize(size_type count, const T& value) {
    m_container.resize(count, value);
}

template <typename T>
void Deque<T>::swap(Deque& other) noexcept {
    m_container.swap(other.m_container);
}

template <typename T>
void Deque<T>::reverse() noexcept {
    m_container.reverse();
}

template <typename T>
typename Deque<T>::size_type Deque<T>::remove(const T& value) {
    return m_container.remove(value);
}

template <typename T>
typename Deque<T>::size_type Deque<T>::find(const T& value) const {
    return m_container.find(value);
}

template <typename T>
bool Deque<T>::contains(const T& value) const {
    return m_container.contains(value);
}

// Explicit template instantiations for common types
template class Deque<int>;
template class Deque<double>;
template class Deque<float>;
template class Deque<char>;
template class Deque<long>;
template class Deque<long long>;
template class Deque<unsigned int>;
template class Deque<unsigned long>;
template class Deque<unsigned long long>;

} // namespace linear
} // namespace mylib