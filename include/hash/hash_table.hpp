/**
 * @file hash_table.hpp
 * @brief Hash Table implementation using separate chaining
 * @author Jinhyeok
 * @date 2025-11-29
 * @version 1.0.0
 * 
 * Copyright (c) 2025 Jinhyeok
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef MYLIB_HASH_HASH_TABLE_HPP
#define MYLIB_HASH_HASH_TABLE_HPP

#include <cstddef>
#include <stdexcept>
#include <utility>
#include <initializer_list>
#include <functional>
#include <vector>
#include <list>

namespace mylib {
namespace hash {

/**
 * @class HashTable
 * @brief A hash table implementation using separate chaining for collision resolution
 * 
 * This class implements a hash table (hash map) data structure that stores
 * key-value pairs. It uses separate chaining with linked lists to handle
 * collisions. The table automatically resizes when the load factor exceeds
 * a threshold.
 * 
 * Average time complexity:
 * - Insert: O(1)
 * - Search: O(1)
 * - Delete: O(1)
 * 
 * Worst case (all keys hash to same bucket): O(n)
 * 
 * @tparam Key The type of keys
 * @tparam Value The type of values
 * @tparam Hash Hash function object type (default: std::hash<Key>)
 * @tparam KeyEqual Key equality comparison function (default: std::equal_to<Key>)
 */
template <typename Key, 
          typename Value, 
          typename Hash = std::hash<Key>,
          typename KeyEqual = std::equal_to<Key>>
class HashTable {
public:
    // Type aliases
    using key_type = Key;
    using mapped_type = Value;
    using value_type = std::pair<const Key, Value>;
    using size_type = std::size_t;
    using hasher = Hash;
    using key_equal = KeyEqual;
    using reference = value_type&;
    using const_reference = const value_type&;

private:
    /**
     * @struct Entry
     * @brief Internal entry structure for key-value pairs
     */
    struct Entry {
        Key key;
        Value value;
        
        Entry(const Key& k, const Value& v) : key(k), value(v) {}
        Entry(Key&& k, Value&& v) : key(std::move(k)), value(std::move(v)) {}
        Entry(const Key& k, Value&& v) : key(k), value(std::move(v)) {}
        Entry(Key&& k, const Value& v) : key(std::move(k)), value(v) {}
    };

    using Bucket = std::list<Entry>;

public:
    /**
     * @brief Default constructor
     * Creates an empty hash table with default bucket count
     */
    HashTable();

    /**
     * @brief Constructor with initial bucket count
     * @param bucket_count Initial number of buckets
     */
    explicit HashTable(size_type bucket_count);

    /**
     * @brief Constructor with bucket count and hasher
     * @param bucket_count Initial number of buckets
     * @param hash Hash function to use
     */
    HashTable(size_type bucket_count, const Hash& hash);

    /**
     * @brief Constructor with bucket count, hasher, and key equality
     * @param bucket_count Initial number of buckets
     * @param hash Hash function to use
     * @param equal Key equality function to use
     */
    HashTable(size_type bucket_count, const Hash& hash, const KeyEqual& equal);

    /**
     * @brief Initializer list constructor
     * @param init Initializer list of key-value pairs
     */
    HashTable(std::initializer_list<std::pair<Key, Value>> init);

    /**
     * @brief Copy constructor
     * @param other Hash table to copy from
     */
    HashTable(const HashTable& other);

    /**
     * @brief Move constructor
     * @param other Hash table to move from
     */
    HashTable(HashTable&& other) noexcept;

    /**
     * @brief Destructor
     */
    ~HashTable() = default;

    /**
     * @brief Copy assignment operator
     * @param other Hash table to copy from
     * @return Reference to this hash table
     */
    HashTable& operator=(const HashTable& other);

    /**
     * @brief Move assignment operator
     * @param other Hash table to move from
     * @return Reference to this hash table
     */
    HashTable& operator=(HashTable&& other) noexcept;

    // Capacity
    /**
     * @brief Check if hash table is empty
     * @return true if empty, false otherwise
     */
    bool empty() const noexcept;

    /**
     * @brief Get number of elements
     * @return Number of key-value pairs
     */
    size_type size() const noexcept;

    /**
     * @brief Get number of buckets
     * @return Current bucket count
     */
    size_type bucket_count() const noexcept;

    /**
     * @brief Get current load factor
     * @return size / bucket_count
     */
    float load_factor() const noexcept;

    /**
     * @brief Get maximum load factor before rehashing
     * @return Maximum load factor threshold
     */
    float max_load_factor() const noexcept;

    /**
     * @brief Set maximum load factor
     * @param ml New maximum load factor
     */
    void max_load_factor(float ml);

    // Element access
    /**
     * @brief Access or insert element with key
     * @param key Key to access/insert
     * @return Reference to value associated with key
     * 
     * If key doesn't exist, inserts default-constructed value.
     */
    Value& operator[](const Key& key);

    /**
     * @brief Access or insert element with key (move version)
     * @param key Key to access/insert
     * @return Reference to value associated with key
     */
    Value& operator[](Key&& key);

    /**
     * @brief Access element with bounds checking
     * @param key Key to access
     * @return Reference to value associated with key
     * @throws std::out_of_range if key not found
     */
    Value& at(const Key& key);
    const Value& at(const Key& key) const;

    // Modifiers
    /**
     * @brief Insert a key-value pair
     * @param key Key to insert
     * @param value Value to insert
     * @return pair of (success, was_inserted)
     */
    std::pair<bool, bool> insert(const Key& key, const Value& value);

    /**
     * @brief Insert a key-value pair (move version)
     * @param key Key to insert
     * @param value Value to insert
     * @return pair of (success, was_inserted)
     */
    std::pair<bool, bool> insert(Key&& key, Value&& value);

    /**
     * @brief Insert or assign value to key
     * @param key Key to insert or update
     * @param value Value to assign
     * @return true if inserted, false if assigned to existing key
     */
    bool insert_or_assign(const Key& key, const Value& value);

    /**
     * @brief Insert or assign value to key (move version)
     * @param key Key to insert or update
     * @param value Value to assign
     * @return true if inserted, false if assigned to existing key
     */
    bool insert_or_assign(Key&& key, Value&& value);

    /**
     * @brief Construct element in-place
     * @param key Key to insert
     * @param args Arguments to forward to value constructor
     * @return pair of (success, was_inserted)
     */
    template <typename... Args>
    std::pair<bool, bool> emplace(const Key& key, Args&&... args);

    /**
     * @brief Remove element with key
     * @param key Key to remove
     * @return true if removed, false if not found
     */
    bool erase(const Key& key);

    /**
     * @brief Clear all elements
     */
    void clear() noexcept;

    /**
     * @brief Swap contents with another hash table
     * @param other Hash table to swap with
     */
    void swap(HashTable& other) noexcept;

    // Lookup
    /**
     * @brief Find element with key
     * @param key Key to search for
     * @return Pointer to value if found, nullptr otherwise
     */
    Value* find(const Key& key);
    const Value* find(const Key& key) const;

    /**
     * @brief Check if key exists
     * @param key Key to search for
     * @return true if key exists, false otherwise
     */
    bool contains(const Key& key) const;

    /**
     * @brief Count elements with key (0 or 1)
     * @param key Key to count
     * @return 1 if key exists, 0 otherwise
     */
    size_type count(const Key& key) const;

    // Bucket interface
    /**
     * @brief Get number of elements in bucket
     * @param n Bucket index
     * @return Number of elements in bucket n
     */
    size_type bucket_size(size_type n) const;

    /**
     * @brief Get bucket index for key
     * @param key Key to get bucket for
     * @return Bucket index where key would be stored
     */
    size_type bucket(const Key& key) const;

    // Hash policy
    /**
     * @brief Reserve space for at least count elements
     * @param count Minimum number of elements
     */
    void reserve(size_type count);

    /**
     * @brief Set number of buckets and rehash
     * @param count New bucket count
     */
    void rehash(size_type count);

    // Observers
    /**
     * @brief Get hash function
     * @return Hash function object
     */
    hasher hash_function() const;

    /**
     * @brief Get key equality function
     * @return Key equality function object
     */
    key_equal key_eq() const;

    // Iteration support
    /**
     * @brief Apply function to all key-value pairs
     * @param func Function to apply
     */
    void for_each(std::function<void(const Key&, Value&)> func);
    void for_each(std::function<void(const Key&, const Value&)> func) const;

    /**
     * @brief Get all keys
     * @return Vector of all keys
     */
    std::vector<Key> keys() const;

    /**
     * @brief Get all values
     * @return Vector of all values
     */
    std::vector<Value> values() const;

private:
    std::vector<Bucket> m_buckets;    ///< Array of buckets
    size_type m_size;                  ///< Number of elements
    float m_max_load_factor;           ///< Maximum load factor
    Hash m_hasher;                     ///< Hash function
    KeyEqual m_key_equal;              ///< Key equality function

    static constexpr size_type DEFAULT_BUCKET_COUNT = 16;
    static constexpr float DEFAULT_MAX_LOAD_FACTOR = 0.75f;

    /**
     * @brief Get bucket index for key
     * @param key Key to hash
     * @return Bucket index
     */
    size_type get_bucket_index(const Key& key) const;

    /**
     * @brief Find entry in bucket
     * @param bucket_index Bucket to search
     * @param key Key to find
     * @return Iterator to entry, or end() if not found
     */
    typename Bucket::iterator find_in_bucket(size_type bucket_index, const Key& key);
    typename Bucket::const_iterator find_in_bucket(size_type bucket_index, const Key& key) const;

    /**
     * @brief Check if rehash is needed and perform if necessary
     */
    void check_rehash();

    /**
     * @brief Get next prime number >= n (for bucket count)
     * @param n Minimum value
     * @return Next prime >= n
     */
    static size_type next_prime(size_type n);

    /**
     * @brief Check if n is prime
     * @param n Number to check
     * @return true if prime
     */
    static bool is_prime(size_type n);
};

// ============================================
// Template member function implementations
// ============================================

template <typename Key, typename Value, typename Hash, typename KeyEqual>
template <typename... Args>
std::pair<bool, bool> HashTable<Key, Value, Hash, KeyEqual>::emplace(
    const Key& key, Args&&... args) {
    
    size_type index = get_bucket_index(key);
    auto it = find_in_bucket(index, key);
    
    if (it != m_buckets[index].end()) {
        return {true, false};
    }
    
    check_rehash();
    index = get_bucket_index(key);
    m_buckets[index].emplace_back(key, Value(std::forward<Args>(args)...));
    ++m_size;
    return {true, true};
}

} // namespace hash
} // namespace mylib

#endif // MYLIB_HASH_HASH_TABLE_HPP