/**
 * @file linked_list.cpp
 * @brief Implementation of LinkedList class
 * @author Jinhyeok
 * @date 2025-11-16
 * @version 1.0.0
 */

#include "linear/linked_list.hpp"

namespace mylib {
namespace linear {

// Constructors
template <typename T>
LinkedList<T>::LinkedList() 
    : m_head(nullptr), m_tail(nullptr), m_size(0) {
}

template <typename T>
LinkedList<T>::LinkedList(size_type count, const T& value)
    : m_head(nullptr), m_tail(nullptr), m_size(0) {
    for (size_type i = 0; i < count; ++i) {
        push_back(value);
    }
}

template <typename T>
LinkedList<T>::LinkedList(std::initializer_list<T> init)
    : m_head(nullptr), m_tail(nullptr), m_size(0) {
    for (const auto& item : init) {
        push_back(item);
    }
}

template <typename T>
LinkedList<T>::LinkedList(const LinkedList& other)
    : m_head(nullptr), m_tail(nullptr), m_size(0) {
    copy_from(other);
}

template <typename T>
LinkedList<T>::LinkedList(LinkedList&& other) noexcept
    : m_head(other.m_head), m_tail(other.m_tail), m_size(other.m_size) {
    other.m_head = nullptr;
    other.m_tail = nullptr;
    other.m_size = 0;
}

template <typename T>
LinkedList<T>::~LinkedList() {
    clear();
}

// Assignment operators
template <typename T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList& other) {
    if (this != &other) {
        clear();
        copy_from(other);
    }
    return *this;
}

template <typename T>
LinkedList<T>& LinkedList<T>::operator=(LinkedList&& other) noexcept {
    if (this != &other) {
        clear();
        m_head = other.m_head;
        m_tail = other.m_tail;
        m_size = other.m_size;
        
        other.m_head = nullptr;
        other.m_tail = nullptr;
        other.m_size = 0;
    }
    return *this;
}

// Element access
template <typename T>
typename LinkedList<T>::reference LinkedList<T>::at(size_type index) {
    if (index >= m_size) {
        throw std::out_of_range("LinkedList::at: index out of range");
    }
    return get_node(index)->data;
}

template <typename T>
typename LinkedList<T>::const_reference LinkedList<T>::at(size_type index) const {
    if (index >= m_size) {
        throw std::out_of_range("LinkedList::at: index out of range");
    }
    return get_node(index)->data;
}

template <typename T>
typename LinkedList<T>::reference LinkedList<T>::operator[](size_type index) {
    return get_node(index)->data;
}

template <typename T>
typename LinkedList<T>::const_reference LinkedList<T>::operator[](size_type index) const {
    return get_node(index)->data;
}

template <typename T>
typename LinkedList<T>::reference LinkedList<T>::front() {
    if (empty()) {
        throw std::out_of_range("LinkedList::front: list is empty");
    }
    return m_head->data;
}

template <typename T>
typename LinkedList<T>::const_reference LinkedList<T>::front() const {
    if (empty()) {
        throw std::out_of_range("LinkedList::front: list is empty");
    }
    return m_head->data;
}

template <typename T>
typename LinkedList<T>::reference LinkedList<T>::back() {
    if (empty()) {
        throw std::out_of_range("LinkedList::back: list is empty");
    }
    return m_tail->data;
}

template <typename T>
typename LinkedList<T>::const_reference LinkedList<T>::back() const {
    if (empty()) {
        throw std::out_of_range("LinkedList::back: list is empty");
    }
    return m_tail->data;
}

// Capacity
template <typename T>
bool LinkedList<T>::empty() const noexcept {
    return m_size == 0;
}

template <typename T>
typename LinkedList<T>::size_type LinkedList<T>::size() const noexcept {
    return m_size;
}

// Modifiers
template <typename T>
void LinkedList<T>::clear() noexcept {
    Node* current = m_head;
    while (current) {
        Node* next = current->next;
        delete current;
        current = next;
    }
    m_head = nullptr;
    m_tail = nullptr;
    m_size = 0;
}

template <typename T>
void LinkedList<T>::push_front(const T& value) {
    Node* new_node = new Node(value);
    
    if (empty()) {
        m_head = m_tail = new_node;
    } else {
        new_node->next = m_head;
        m_head->prev = new_node;
        m_head = new_node;
    }
    
    ++m_size;
}

template <typename T>
void LinkedList<T>::push_front(T&& value) {
    Node* new_node = new Node(std::move(value));
    
    if (empty()) {
        m_head = m_tail = new_node;
    } else {
        new_node->next = m_head;
        m_head->prev = new_node;
        m_head = new_node;
    }
    
    ++m_size;
}

template <typename T>
void LinkedList<T>::push_back(const T& value) {
    Node* new_node = new Node(value);
    
    if (empty()) {
        m_head = m_tail = new_node;
    } else {
        new_node->prev = m_tail;
        m_tail->next = new_node;
        m_tail = new_node;
    }
    
    ++m_size;
}

template <typename T>
void LinkedList<T>::push_back(T&& value) {
    Node* new_node = new Node(std::move(value));
    
    if (empty()) {
        m_head = m_tail = new_node;
    } else {
        new_node->prev = m_tail;
        m_tail->next = new_node;
        m_tail = new_node;
    }
    
    ++m_size;
}

template <typename T>
void LinkedList<T>::pop_front() {
    if (empty()) {
        throw std::out_of_range("LinkedList::pop_front: list is empty");
    }
    
    Node* old_head = m_head;
    m_head = m_head->next;
    
    if (m_head) {
        m_head->prev = nullptr;
    } else {
        m_tail = nullptr;
    }
    
    delete old_head;
    --m_size;
}

template <typename T>
void LinkedList<T>::pop_back() {
    if (empty()) {
        throw std::out_of_range("LinkedList::pop_back: list is empty");
    }
    
    Node* old_tail = m_tail;
    m_tail = m_tail->prev;
    
    if (m_tail) {
        m_tail->next = nullptr;
    } else {
        m_head = nullptr;
    }
    
    delete old_tail;
    --m_size;
}

template <typename T>
void LinkedList<T>::insert(size_type index, const T& value) {
    if (index > m_size) {
        throw std::out_of_range("LinkedList::insert: index out of range");
    }
    
    if (index == 0) {
        push_front(value);
        return;
    }
    
    if (index == m_size) {
        push_back(value);
        return;
    }
    
    Node* new_node = new Node(value);
    Node* current = get_node(index);
    
    new_node->next = current;
    new_node->prev = current->prev;
    current->prev->next = new_node;
    current->prev = new_node;
    
    ++m_size;
}

template <typename T>
void LinkedList<T>::insert(size_type index, T&& value) {
    if (index > m_size) {
        throw std::out_of_range("LinkedList::insert: index out of range");
    }
    
    if (index == 0) {
        push_front(std::move(value));
        return;
    }
    
    if (index == m_size) {
        push_back(std::move(value));
        return;
    }
    
    Node* new_node = new Node(std::move(value));
    Node* current = get_node(index);
    
    new_node->next = current;
    new_node->prev = current->prev;
    current->prev->next = new_node;
    current->prev = new_node;
    
    ++m_size;
}

template <typename T>
void LinkedList<T>::erase(size_type index) {
    if (index >= m_size) {
        throw std::out_of_range("LinkedList::erase: index out of range");
    }
    
    if (index == 0) {
        pop_front();
        return;
    }
    
    if (index == m_size - 1) {
        pop_back();
        return;
    }
    
    Node* current = get_node(index);
    current->prev->next = current->next;
    current->next->prev = current->prev;
    
    delete current;
    --m_size;
}

template <typename T>
void LinkedList<T>::resize(size_type count) {
    while (m_size > count) {
        pop_back();
    }
    
    while (m_size < count) {
        push_back(T());
    }
}

template <typename T>
void LinkedList<T>::resize(size_type count, const T& value) {
    while (m_size > count) {
        pop_back();
    }
    
    while (m_size < count) {
        push_back(value);
    }
}

template <typename T>
void LinkedList<T>::swap(LinkedList& other) noexcept {
    std::swap(m_head, other.m_head);
    std::swap(m_tail, other.m_tail);
    std::swap(m_size, other.m_size);
}

template <typename T>
void LinkedList<T>::reverse() noexcept {
    if (m_size <= 1) {
        return;
    }
    
    Node* current = m_head;
    Node* temp = nullptr;
    
    while (current) {
        temp = current->prev;
        current->prev = current->next;
        current->next = temp;
        current = current->prev;
    }
    
    temp = m_head;
    m_head = m_tail;
    m_tail = temp;
}

template <typename T>
typename LinkedList<T>::size_type LinkedList<T>::remove(const T& value) {
    size_type removed_count = 0;
    Node* current = m_head;
    
    while (current) {
        Node* next = current->next;
        
        if (current->data == value) {
            if (current == m_head) {
                pop_front();
            } else if (current == m_tail) {
                pop_back();
            } else {
                current->prev->next = current->next;
                current->next->prev = current->prev;
                delete current;
                --m_size;
            }
            ++removed_count;
        }
        
        current = next;
    }
    
    return removed_count;
}

template <typename T>
typename LinkedList<T>::size_type LinkedList<T>::find(const T& value) const {
    Node* current = m_head;
    size_type index = 0;
    
    while (current) {
        if (current->data == value) {
            return index;
        }
        current = current->next;
        ++index;
    }
    
    return m_size;
}

template <typename T>
bool LinkedList<T>::contains(const T& value) const {
    return find(value) != m_size;
}

// Private helper methods
template <typename T>
typename LinkedList<T>::Node* LinkedList<T>::get_node(size_type index) const {
    Node* current;
    
    // Optimize by starting from the closer end
    if (index < m_size / 2) {
        current = m_head;
        for (size_type i = 0; i < index; ++i) {
            current = current->next;
        }
    } else {
        current = m_tail;
        for (size_type i = m_size - 1; i > index; --i) {
            current = current->prev;
        }
    }
    
    return current;
}

template <typename T>
void LinkedList<T>::copy_from(const LinkedList& other) {
    Node* current = other.m_head;
    while (current) {
        push_back(current->data);
        current = current->next;
    }
}

// Explicit template instantiations for common types
template class LinkedList<int>;
template class LinkedList<double>;
template class LinkedList<float>;
template class LinkedList<char>;
template class LinkedList<long>;
template class LinkedList<long long>;
template class LinkedList<unsigned int>;
template class LinkedList<unsigned long>;
template class LinkedList<unsigned long long>;

} // namespace linear
} // namespace mylib
