/**
 * @file queue.cpp
 * @brief Implementation of Queue class
 * @author Jinhyeok
 * @date 2025-11-29
 * @version 1.0.0
 */

#include "linear/queue.hpp"

namespace mylib {
namespace linear {

// Constructors
template <typename T>
Queue<T>::Queue() : m_container() {
}

template <typename T>
Queue<T>::Queue(const Queue& other) 
    : m_container(other.m_container) {
}

template <typename T>
Queue<T>::Queue(Queue&& other) noexcept 
    : m_container(std::move(other.m_container)) {
}

// Assignment operators
template <typename T>
Queue<T>& Queue<T>::operator=(const Queue& other) {
    if (this != &other) {
        m_container = other.m_container;
    }
    return *this;
}

template <typename T>
Queue<T>& Queue<T>::operator=(Queue&& other) noexcept {
    if (this != &other) {
        m_container = std::move(other.m_container);
    }
    return *this;
}

// Element access
template <typename T>
typename Queue<T>::reference Queue<T>::front() {
    if (empty()) {
        throw std::out_of_range("Queue::front: queue is empty");
    }
    return m_container.front();
}

template <typename T>
typename Queue<T>::const_reference Queue<T>::front() const {
    if (empty()) {
        throw std::out_of_range("Queue::front: queue is empty");
    }
    return m_container.front();
}

template <typename T>
typename Queue<T>::reference Queue<T>::back() {
    if (empty()) {
        throw std::out_of_range("Queue::back: queue is empty");
    }
    return m_container.back();
}

template <typename T>
typename Queue<T>::const_reference Queue<T>::back() const {
    if (empty()) {
        throw std::out_of_range("Queue::back: queue is empty");
    }
    return m_container.back();
}

// Capacity
template <typename T>
bool Queue<T>::empty() const noexcept {
    return m_container.empty();
}

template <typename T>
typename Queue<T>::size_type Queue<T>::size() const noexcept {
    return m_container.size();
}

// Modifiers
template <typename T>
void Queue<T>::push(const T& value) {
    m_container.push_back(value);
}

template <typename T>
void Queue<T>::push(T&& value) {
    m_container.push_back(std::move(value));
}

template <typename T>
void Queue<T>::pop() {
    if (empty()) {
        throw std::out_of_range("Queue::pop: queue is empty");
    }
    m_container.pop_front();
}

template <typename T>
void Queue<T>::clear() noexcept {
    m_container.clear();
}

template <typename T>
void Queue<T>::swap(Queue& other) noexcept {
    m_container.swap(other.m_container);
}

// Explicit template instantiations for common types
template class Queue<int>;
template class Queue<double>;
template class Queue<float>;
template class Queue<char>;
template class Queue<long>;
template class Queue<long long>;
template class Queue<unsigned int>;
template class Queue<unsigned long>;
template class Queue<unsigned long long>;

} // namespace linear
} // namespace mylib