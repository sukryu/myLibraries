/**
 * @file hash_table.cpp
 * @brief Implementation of HashTable class
 * @author Jinhyeok
 * @date 2025-11-29
 * @version 1.0.0
 */

#include "hash/hash_table.hpp"
#include <algorithm>
#include <cmath>

namespace mylib {
namespace hash {

// Constructors
template <typename Key, typename Value, typename Hash, typename KeyEqual>
HashTable<Key, Value, Hash, KeyEqual>::HashTable()
    : m_buckets(DEFAULT_BUCKET_COUNT)
    , m_size(0)
    , m_max_load_factor(DEFAULT_MAX_LOAD_FACTOR)
    , m_hasher()
    , m_key_equal() {
}

template <typename Key, typename Value, typename Hash, typename KeyEqual>
HashTable<Key, Value, Hash, KeyEqual>::HashTable(size_type bucket_count)
    : m_buckets(bucket_count > 0 ? bucket_count : DEFAULT_BUCKET_COUNT)
    , m_size(0)
    , m_max_load_factor(DEFAULT_MAX_LOAD_FACTOR)
    , m_hasher()
    , m_key_equal() {
}

template <typename Key, typename Value, typename Hash, typename KeyEqual>
HashTable<Key, Value, Hash, KeyEqual>::HashTable(size_type bucket_count, const Hash& hash)
    : m_buckets(bucket_count > 0 ? bucket_count : DEFAULT_BUCKET_COUNT)
    , m_size(0)
    , m_max_load_factor(DEFAULT_MAX_LOAD_FACTOR)
    , m_hasher(hash)
    , m_key_equal() {
}

template <typename Key, typename Value, typename Hash, typename KeyEqual>
HashTable<Key, Value, Hash, KeyEqual>::HashTable(
    size_type bucket_count, const Hash& hash, const KeyEqual& equal)
    : m_buckets(bucket_count > 0 ? bucket_count : DEFAULT_BUCKET_COUNT)
    , m_size(0)
    , m_max_load_factor(DEFAULT_MAX_LOAD_FACTOR)
    , m_hasher(hash)
    , m_key_equal(equal) {
}

template <typename Key, typename Value, typename Hash, typename KeyEqual>
HashTable<Key, Value, Hash, KeyEqual>::HashTable(
    std::initializer_list<std::pair<Key, Value>> init)
    : m_buckets(DEFAULT_BUCKET_COUNT)
    , m_size(0)
    , m_max_load_factor(DEFAULT_MAX_LOAD_FACTOR)
    , m_hasher()
    , m_key_equal() {
    for (const auto& pair : init) {
        insert(pair.first, pair.second);
    }
}

template <typename Key, typename Value, typename Hash, typename KeyEqual>
HashTable<Key, Value, Hash, KeyEqual>::HashTable(const HashTable& other)
    : m_buckets(other.m_buckets)
    , m_size(other.m_size)
    , m_max_load_factor(other.m_max_load_factor)
    , m_hasher(other.m_hasher)
    , m_key_equal(other.m_key_equal) {
}

template <typename Key, typename Value, typename Hash, typename KeyEqual>
HashTable<Key, Value, Hash, KeyEqual>::HashTable(HashTable&& other) noexcept
    : m_buckets(std::move(other.m_buckets))
    , m_size(other.m_size)
    , m_max_load_factor(other.m_max_load_factor)
    , m_hasher(std::move(other.m_hasher))
    , m_key_equal(std::move(other.m_key_equal)) {
    other.m_size = 0;
    other.m_buckets = std::vector<Bucket>(DEFAULT_BUCKET_COUNT);
}

// Assignment operators
template <typename Key, typename Value, typename Hash, typename KeyEqual>
HashTable<Key, Value, Hash, KeyEqual>& 
HashTable<Key, Value, Hash, KeyEqual>::operator=(const HashTable& other) {
    if (this != &other) {
        m_buckets = other.m_buckets;
        m_size = other.m_size;
        m_max_load_factor = other.m_max_load_factor;
        m_hasher = other.m_hasher;
        m_key_equal = other.m_key_equal;
    }
    return *this;
}

template <typename Key, typename Value, typename Hash, typename KeyEqual>
HashTable<Key, Value, Hash, KeyEqual>& 
HashTable<Key, Value, Hash, KeyEqual>::operator=(HashTable&& other) noexcept {
    if (this != &other) {
        m_buckets = std::move(other.m_buckets);
        m_size = other.m_size;
        m_max_load_factor = other.m_max_load_factor;
        m_hasher = std::move(other.m_hasher);
        m_key_equal = std::move(other.m_key_equal);
        
        other.m_size = 0;
        other.m_buckets = std::vector<Bucket>(DEFAULT_BUCKET_COUNT);
    }
    return *this;
}

// Capacity
template <typename Key, typename Value, typename Hash, typename KeyEqual>
bool HashTable<Key, Value, Hash, KeyEqual>::empty() const noexcept {
    return m_size == 0;
}

template <typename Key, typename Value, typename Hash, typename KeyEqual>
typename HashTable<Key, Value, Hash, KeyEqual>::size_type 
HashTable<Key, Value, Hash, KeyEqual>::size() const noexcept {
    return m_size;
}

template <typename Key, typename Value, typename Hash, typename KeyEqual>
typename HashTable<Key, Value, Hash, KeyEqual>::size_type 
HashTable<Key, Value, Hash, KeyEqual>::bucket_count() const noexcept {
    return m_buckets.size();
}

template <typename Key, typename Value, typename Hash, typename KeyEqual>
float HashTable<Key, Value, Hash, KeyEqual>::load_factor() const noexcept {
    return m_buckets.empty() ? 0.0f : static_cast<float>(m_size) / m_buckets.size();
}

template <typename Key, typename Value, typename Hash, typename KeyEqual>
float HashTable<Key, Value, Hash, KeyEqual>::max_load_factor() const noexcept {
    return m_max_load_factor;
}

template <typename Key, typename Value, typename Hash, typename KeyEqual>
void HashTable<Key, Value, Hash, KeyEqual>::max_load_factor(float ml) {
    m_max_load_factor = ml;
    check_rehash();
}

// Element access
template <typename Key, typename Value, typename Hash, typename KeyEqual>
Value& HashTable<Key, Value, Hash, KeyEqual>::operator[](const Key& key) {
    size_type index = get_bucket_index(key);
    auto it = find_in_bucket(index, key);
    
    if (it != m_buckets[index].end()) {
        return it->value;
    }
    
    // Insert default value
    check_rehash();
    index = get_bucket_index(key);  // Recalculate after potential rehash
    m_buckets[index].emplace_back(key, Value{});
    ++m_size;
    return m_buckets[index].back().value;
}

template <typename Key, typename Value, typename Hash, typename KeyEqual>
Value& HashTable<Key, Value, Hash, KeyEqual>::operator[](Key&& key) {
    size_type index = get_bucket_index(key);
    auto it = find_in_bucket(index, key);
    
    if (it != m_buckets[index].end()) {
        return it->value;
    }
    
    check_rehash();
    index = get_bucket_index(key);
    m_buckets[index].emplace_back(std::move(key), Value{});
    ++m_size;
    return m_buckets[index].back().value;
}

template <typename Key, typename Value, typename Hash, typename KeyEqual>
Value& HashTable<Key, Value, Hash, KeyEqual>::at(const Key& key) {
    size_type index = get_bucket_index(key);
    auto it = find_in_bucket(index, key);
    
    if (it == m_buckets[index].end()) {
        throw std::out_of_range("HashTable::at: key not found");
    }
    return it->value;
}

template <typename Key, typename Value, typename Hash, typename KeyEqual>
const Value& HashTable<Key, Value, Hash, KeyEqual>::at(const Key& key) const {
    size_type index = get_bucket_index(key);
    auto it = find_in_bucket(index, key);
    
    if (it == m_buckets[index].end()) {
        throw std::out_of_range("HashTable::at: key not found");
    }
    return it->value;
}

// Modifiers
template <typename Key, typename Value, typename Hash, typename KeyEqual>
std::pair<bool, bool> HashTable<Key, Value, Hash, KeyEqual>::insert(
    const Key& key, const Value& value) {
    
    size_type index = get_bucket_index(key);
    auto it = find_in_bucket(index, key);
    
    if (it != m_buckets[index].end()) {
        // Key already exists
        return {true, false};
    }
    
    check_rehash();
    index = get_bucket_index(key);
    m_buckets[index].emplace_back(key, value);
    ++m_size;
    return {true, true};
}

template <typename Key, typename Value, typename Hash, typename KeyEqual>
std::pair<bool, bool> HashTable<Key, Value, Hash, KeyEqual>::insert(
    Key&& key, Value&& value) {
    
    size_type index = get_bucket_index(key);
    auto it = find_in_bucket(index, key);
    
    if (it != m_buckets[index].end()) {
        return {true, false};
    }
    
    check_rehash();
    index = get_bucket_index(key);
    m_buckets[index].emplace_back(std::move(key), std::move(value));
    ++m_size;
    return {true, true};
}

template <typename Key, typename Value, typename Hash, typename KeyEqual>
bool HashTable<Key, Value, Hash, KeyEqual>::insert_or_assign(
    const Key& key, const Value& value) {
    
    size_type index = get_bucket_index(key);
    auto it = find_in_bucket(index, key);
    
    if (it != m_buckets[index].end()) {
        it->value = value;
        return false;  // Assigned
    }
    
    check_rehash();
    index = get_bucket_index(key);
    m_buckets[index].emplace_back(key, value);
    ++m_size;
    return true;  // Inserted
}

template <typename Key, typename Value, typename Hash, typename KeyEqual>
bool HashTable<Key, Value, Hash, KeyEqual>::insert_or_assign(
    Key&& key, Value&& value) {
    
    size_type index = get_bucket_index(key);
    auto it = find_in_bucket(index, key);
    
    if (it != m_buckets[index].end()) {
        it->value = std::move(value);
        return false;
    }
    
    check_rehash();
    index = get_bucket_index(key);
    m_buckets[index].emplace_back(std::move(key), std::move(value));
    ++m_size;
    return true;
}

template <typename Key, typename Value, typename Hash, typename KeyEqual>
bool HashTable<Key, Value, Hash, KeyEqual>::erase(const Key& key) {
    size_type index = get_bucket_index(key);
    auto& bucket = m_buckets[index];
    
    for (auto it = bucket.begin(); it != bucket.end(); ++it) {
        if (m_key_equal(it->key, key)) {
            bucket.erase(it);
            --m_size;
            return true;
        }
    }
    return false;
}

template <typename Key, typename Value, typename Hash, typename KeyEqual>
void HashTable<Key, Value, Hash, KeyEqual>::clear() noexcept {
    for (auto& bucket : m_buckets) {
        bucket.clear();
    }
    m_size = 0;
}

template <typename Key, typename Value, typename Hash, typename KeyEqual>
void HashTable<Key, Value, Hash, KeyEqual>::swap(HashTable& other) noexcept {
    m_buckets.swap(other.m_buckets);
    std::swap(m_size, other.m_size);
    std::swap(m_max_load_factor, other.m_max_load_factor);
    std::swap(m_hasher, other.m_hasher);
    std::swap(m_key_equal, other.m_key_equal);
}

// Lookup
template <typename Key, typename Value, typename Hash, typename KeyEqual>
Value* HashTable<Key, Value, Hash, KeyEqual>::find(const Key& key) {
    size_type index = get_bucket_index(key);
    auto it = find_in_bucket(index, key);
    
    if (it != m_buckets[index].end()) {
        return &(it->value);
    }
    return nullptr;
}

template <typename Key, typename Value, typename Hash, typename KeyEqual>
const Value* HashTable<Key, Value, Hash, KeyEqual>::find(const Key& key) const {
    size_type index = get_bucket_index(key);
    auto it = find_in_bucket(index, key);
    
    if (it != m_buckets[index].end()) {
        return &(it->value);
    }
    return nullptr;
}

template <typename Key, typename Value, typename Hash, typename KeyEqual>
bool HashTable<Key, Value, Hash, KeyEqual>::contains(const Key& key) const {
    return find(key) != nullptr;
}

template <typename Key, typename Value, typename Hash, typename KeyEqual>
typename HashTable<Key, Value, Hash, KeyEqual>::size_type 
HashTable<Key, Value, Hash, KeyEqual>::count(const Key& key) const {
    return contains(key) ? 1 : 0;
}

// Bucket interface
template <typename Key, typename Value, typename Hash, typename KeyEqual>
typename HashTable<Key, Value, Hash, KeyEqual>::size_type 
HashTable<Key, Value, Hash, KeyEqual>::bucket_size(size_type n) const {
    if (n >= m_buckets.size()) {
        return 0;
    }
    return m_buckets[n].size();
}

template <typename Key, typename Value, typename Hash, typename KeyEqual>
typename HashTable<Key, Value, Hash, KeyEqual>::size_type 
HashTable<Key, Value, Hash, KeyEqual>::bucket(const Key& key) const {
    return get_bucket_index(key);
}

// Hash policy
template <typename Key, typename Value, typename Hash, typename KeyEqual>
void HashTable<Key, Value, Hash, KeyEqual>::reserve(size_type count) {
    size_type needed_buckets = static_cast<size_type>(
        std::ceil(count / m_max_load_factor));
    if (needed_buckets > m_buckets.size()) {
        rehash(needed_buckets);
    }
}

template <typename Key, typename Value, typename Hash, typename KeyEqual>
void HashTable<Key, Value, Hash, KeyEqual>::rehash(size_type count) {
    size_type new_bucket_count = std::max(count, 
        static_cast<size_type>(std::ceil(m_size / m_max_load_factor)));
    new_bucket_count = next_prime(new_bucket_count);
    
    if (new_bucket_count == m_buckets.size()) {
        return;
    }
    
    std::vector<Bucket> new_buckets(new_bucket_count);
    
    for (auto& bucket : m_buckets) {
        for (auto& entry : bucket) {
            size_type new_index = m_hasher(entry.key) % new_bucket_count;
            new_buckets[new_index].emplace_back(std::move(entry.key), std::move(entry.value));
        }
    }
    
    m_buckets = std::move(new_buckets);
}

// Observers
template <typename Key, typename Value, typename Hash, typename KeyEqual>
typename HashTable<Key, Value, Hash, KeyEqual>::hasher 
HashTable<Key, Value, Hash, KeyEqual>::hash_function() const {
    return m_hasher;
}

template <typename Key, typename Value, typename Hash, typename KeyEqual>
typename HashTable<Key, Value, Hash, KeyEqual>::key_equal 
HashTable<Key, Value, Hash, KeyEqual>::key_eq() const {
    return m_key_equal;
}

// Iteration support
template <typename Key, typename Value, typename Hash, typename KeyEqual>
void HashTable<Key, Value, Hash, KeyEqual>::for_each(
    std::function<void(const Key&, Value&)> func) {
    
    for (auto& bucket : m_buckets) {
        for (auto& entry : bucket) {
            func(entry.key, entry.value);
        }
    }
}

template <typename Key, typename Value, typename Hash, typename KeyEqual>
void HashTable<Key, Value, Hash, KeyEqual>::for_each(
    std::function<void(const Key&, const Value&)> func) const {
    
    for (const auto& bucket : m_buckets) {
        for (const auto& entry : bucket) {
            func(entry.key, entry.value);
        }
    }
}

template <typename Key, typename Value, typename Hash, typename KeyEqual>
std::vector<Key> HashTable<Key, Value, Hash, KeyEqual>::keys() const {
    std::vector<Key> result;
    result.reserve(m_size);
    
    for (const auto& bucket : m_buckets) {
        for (const auto& entry : bucket) {
            result.push_back(entry.key);
        }
    }
    return result;
}

template <typename Key, typename Value, typename Hash, typename KeyEqual>
std::vector<Value> HashTable<Key, Value, Hash, KeyEqual>::values() const {
    std::vector<Value> result;
    result.reserve(m_size);
    
    for (const auto& bucket : m_buckets) {
        for (const auto& entry : bucket) {
            result.push_back(entry.value);
        }
    }
    return result;
}

// Private helpers
template <typename Key, typename Value, typename Hash, typename KeyEqual>
typename HashTable<Key, Value, Hash, KeyEqual>::size_type 
HashTable<Key, Value, Hash, KeyEqual>::get_bucket_index(const Key& key) const {
    return m_hasher(key) % m_buckets.size();
}

template <typename Key, typename Value, typename Hash, typename KeyEqual>
typename HashTable<Key, Value, Hash, KeyEqual>::Bucket::iterator 
HashTable<Key, Value, Hash, KeyEqual>::find_in_bucket(size_type bucket_index, const Key& key) {
    auto& bucket = m_buckets[bucket_index];
    for (auto it = bucket.begin(); it != bucket.end(); ++it) {
        if (m_key_equal(it->key, key)) {
            return it;
        }
    }
    return bucket.end();
}

template <typename Key, typename Value, typename Hash, typename KeyEqual>
typename HashTable<Key, Value, Hash, KeyEqual>::Bucket::const_iterator 
HashTable<Key, Value, Hash, KeyEqual>::find_in_bucket(size_type bucket_index, const Key& key) const {
    const auto& bucket = m_buckets[bucket_index];
    for (auto it = bucket.begin(); it != bucket.end(); ++it) {
        if (m_key_equal(it->key, key)) {
            return it;
        }
    }
    return bucket.end();
}

template <typename Key, typename Value, typename Hash, typename KeyEqual>
void HashTable<Key, Value, Hash, KeyEqual>::check_rehash() {
    if (load_factor() > m_max_load_factor) {
        rehash(m_buckets.size() * 2);
    }
}

template <typename Key, typename Value, typename Hash, typename KeyEqual>
typename HashTable<Key, Value, Hash, KeyEqual>::size_type 
HashTable<Key, Value, Hash, KeyEqual>::next_prime(size_type n) {
    if (n <= 2) return 2;
    if (n % 2 == 0) ++n;
    
    while (!is_prime(n)) {
        n += 2;
    }
    return n;
}

template <typename Key, typename Value, typename Hash, typename KeyEqual>
bool HashTable<Key, Value, Hash, KeyEqual>::is_prime(size_type n) {
    if (n < 2) return false;
    if (n == 2) return true;
    if (n % 2 == 0) return false;
    
    size_type sqrt_n = static_cast<size_type>(std::sqrt(n));
    for (size_type i = 3; i <= sqrt_n; i += 2) {
        if (n % i == 0) return false;
    }
    return true;
}

// Explicit template instantiations for common types
template class HashTable<int, int>;
template class HashTable<int, double>;
template class HashTable<int, std::string>;
template class HashTable<std::string, int>;
template class HashTable<std::string, double>;
template class HashTable<std::string, std::string>;
template class HashTable<long, long>;
template class HashTable<long long, long long>;

} // namespace hash
} // namespace mylib