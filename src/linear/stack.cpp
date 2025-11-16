/**
 * @file stack.cpp
 * @brief Implementation of Stack class
 * @author Jinhyeok
 * @date 2025-11-16
 * @version 1.0.0
 */

#include "linear/stack.hpp"

namespace mylib {
namespace linear {

// Constructors
template <typename T>
Stack<T>::Stack() : m_container() {
}

template <typename T>
Stack<T>::Stack(size_type initial_capacity) 
    : m_container(initial_capacity) {
}

template <typename T>
Stack<T>::Stack(const Stack& other) 
    : m_container(other.m_container) {
}

template <typename T>
Stack<T>::Stack(Stack&& other) noexcept 
    : m_container(std::move(other.m_container)) {
}

// Assignment operators
template <typename T>
Stack<T>& Stack<T>::operator=(const Stack& other) {
    if (this != &other) {
        m_container = other.m_container;
    }
    return *this;
}

template <typename T>
Stack<T>& Stack<T>::operator=(Stack&& other) noexcept {
    if (this != &other) {
        m_container = std::move(other.m_container);
    }
    return *this;
}

// Element access
template <typename T>
typename Stack<T>::reference Stack<T>::top() {
    if (empty()) {
        throw std::out_of_range("Stack::top: stack is empty");
    }
    return m_container.back();
}

template <typename T>
typename Stack<T>::const_reference Stack<T>::top() const {
    if (empty()) {
        throw std::out_of_range("Stack::top: stack is empty");
    }
    return m_container.back();
}

// Capacity
template <typename T>
bool Stack<T>::empty() const noexcept {
    return m_container.empty();
}

template <typename T>
typename Stack<T>::size_type Stack<T>::size() const noexcept {
    return m_container.size();
}

template <typename T>
typename Stack<T>::size_type Stack<T>::capacity() const noexcept {
    return m_container.capacity();
}

// Modifiers
template <typename T>
void Stack<T>::push(const T& value) {
    m_container.push_back(value);
}

template <typename T>
void Stack<T>::push(T&& value) {
    m_container.push_back(std::move(value));
}

template <typename T>
void Stack<T>::pop() {
    if (empty()) {
        throw std::out_of_range("Stack::pop: stack is empty");
    }
    m_container.pop_back();
}

template <typename T>
void Stack<T>::clear() noexcept {
    m_container.clear();
}

template <typename T>
void Stack<T>::swap(Stack& other) noexcept {
    m_container.swap(other.m_container);
}

template <typename T>
void Stack<T>::reserve(size_type new_capacity) {
    m_container.reserve(new_capacity);
}

// Explicit template instantiations for common types
template class Stack<int>;
template class Stack<double>;
template class Stack<float>;
template class Stack<char>;
template class Stack<long>;
template class Stack<long long>;
template class Stack<unsigned int>;
template class Stack<unsigned long>;
template class Stack<unsigned long long>;

} // namespace linear
} // namespace mylib
