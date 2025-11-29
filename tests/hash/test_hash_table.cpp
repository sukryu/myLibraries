/**
 * @file test_hash_table.cpp
 * @brief Test suite for HashTable class
 * @author Jinhyeok
 * @date 2025-11-29
 * @version 1.0.0
 */

#include "hash/hash_table.hpp"
#include <iostream>
#include <cassert>
#include <vector>
#include <string>
#include <cmath>

using namespace mylib::hash;

// Test result counter
int tests_passed = 0;
int tests_failed = 0;

// Helper macro for testing
#define TEST(name) \
    std::cout << "Testing: " << name << "... "; \
    try {

#define END_TEST \
        std::cout << "✓ PASSED" << std::endl; \
        tests_passed++; \
    } catch (const std::exception& e) { \
        std::cout << "✗ FAILED: " << e.what() << std::endl; \
        tests_failed++; \
    }

// Test functions

void test_default_constructor() {
    TEST("Default constructor")
    HashTable<int, int> ht;
    assert(ht.empty());
    assert(ht.size() == 0);
    assert(ht.bucket_count() > 0);
    assert(ht.load_factor() == 0.0f);
    END_TEST
}

void test_initializer_list() {
    TEST("Initializer list constructor")
    HashTable<std::string, int> ht = {
        {"one", 1}, {"two", 2}, {"three", 3}
    };
    assert(ht.size() == 3);
    assert(!ht.empty());
    assert(ht.contains("one"));
    assert(ht.contains("two"));
    assert(ht["three"] == 3);
    END_TEST
}

void test_insert_and_access() {
    TEST("Insert and operator[] access")
    HashTable<std::string, int> ht;
    
    // Test standard insert
    auto result = ht.insert("apple", 100);
    assert(result.first == true);  // Success
    assert(result.second == true); // Was inserted
    assert(ht.size() == 1);
    assert(ht["apple"] == 100);

    // Test duplicate insert
    result = ht.insert("apple", 200);
    assert(result.first == true);
    assert(result.second == false); // Not inserted
    assert(ht["apple"] == 100);     // Value unchanged

    // Test operator[] insertion
    ht["banana"] = 200;
    assert(ht.size() == 2);
    assert(ht["banana"] == 200);

    // Test operator[] modification
    ht["banana"] = 300;
    assert(ht["banana"] == 300);
    
    // Test insert_or_assign
    bool inserted = ht.insert_or_assign("cherry", 400);
    assert(inserted == true);
    assert(ht["cherry"] == 400);
    
    inserted = ht.insert_or_assign("cherry", 500);
    assert(inserted == false); // Assigned
    assert(ht["cherry"] == 500);
    END_TEST
}

void test_emplace() {
    TEST("Emplace")
    HashTable<int, std::pair<int, int>> ht;
    
    // Emplace constructs the value type (std::pair in this case) in place
    // The Value type is pair<int,int>, the HashTable stores <Key, Value>
    
    ht.emplace(1, 10, 20); // Key 1, Value constructed with {10, 20}
    assert(ht.size() == 1);
    assert(ht[1].first == 10);
    assert(ht[1].second == 20);
    
    // Duplicate emplace
    auto res = ht.emplace(1, 30, 40);
    assert(res.second == false); // Not inserted
    assert(ht[1].first == 10);   // Unchanged
    END_TEST
}

void test_lookup_and_bounds() {
    TEST("Lookup methods (find, contains, at)")
    HashTable<int, int> ht = {{1, 10}, {2, 20}};

    // Contains
    assert(ht.contains(1));
    assert(!ht.contains(99));

    // Count
    assert(ht.count(1) == 1);
    assert(ht.count(99) == 0);

    // Find
    assert(ht.find(1) != nullptr);
    assert(*ht.find(1) == 10);
    assert(ht.find(99) == nullptr);

    // At
    assert(ht.at(1) == 10);
    try {
        ht.at(99);
        throw std::runtime_error("at() should throw out_of_range");
    } catch (const std::out_of_range&) {
        // Expected behavior
    }
    END_TEST
}

void test_copy_move() {
    TEST("Copy and Move semantics")
    HashTable<int, int> original = {{1, 1}, {2, 2}, {3, 3}};
    
    // Copy Constructor
    HashTable<int, int> copy(original);
    assert(copy.size() == 3);
    assert(copy[1] == 1);
    copy[1] = 100;
    assert(original[1] == 1); // Deep copy check

    // Move Constructor
    HashTable<int, int> moved(std::move(original));
    assert(moved.size() == 3);
    assert(moved.contains(2));
    assert(original.empty()); // Source should be empty

    // Assignment
    HashTable<int, int> assigned;
    assigned = moved;
    assert(assigned.size() == 3);
    assert(assigned.contains(3));
    END_TEST
}

void test_modifications() {
    TEST("Erase, Clear, Swap")
    HashTable<int, int> ht = {{1, 10}, {2, 20}, {3, 30}};

    // Erase
    assert(ht.erase(2) == true);
    assert(ht.size() == 2);
    assert(!ht.contains(2));
    assert(ht.erase(99) == false); // Erase non-existing

    // Swap
    HashTable<int, int> other = {{5, 50}};
    ht.swap(other);
    assert(ht.size() == 1);
    assert(ht.contains(5));
    assert(other.size() == 2);
    assert(other.contains(1));

    // Clear
    other.clear();
    assert(other.empty());
    assert(other.size() == 0);
    END_TEST
}

// void test_rehashing() {
//     TEST("Rehashing and Load Factor")
//     // Start with small bucket count
//     HashTable<int, int> ht(4); 
//     assert(ht.bucket_count() >= 4); // Might be next prime
    
//     size_type initial_buckets = ht.bucket_count();
    
//     // Insert enough elements to force rehash (default max load factor 0.75)
//     // If buckets = 5 (next prime after 4), limit is 3.75 -> 3 elements
//     // Inserting 10 elements should definitely trigger rehash
//     for(int i = 0; i < 20; ++i) {
//         ht.insert(i, i);
//     }
    
//     assert(ht.size() == 20);
//     assert(ht.bucket_count() > initial_buckets);
//     assert(ht.load_factor() <= ht.max_load_factor());
    
//     // Manual reserve
//     ht.reserve(100);
//     assert(ht.bucket_count() >= 100 / ht.max_load_factor());
    
//     // Verify data integrity after rehash
//     for(int i = 0; i < 20; ++i) {
//         assert(ht.contains(i));
//         assert(ht[i] == i);
//     }
//     END_TEST
// }

void test_iterators_and_views() {
    TEST("Iteration (keys, values, for_each)")
    HashTable<int, int> ht = {{1, 10}, {2, 20}, {3, 30}};

    // Keys
    auto keys = ht.keys();
    assert(keys.size() == 3);
    bool found_one = false;
    for(int k : keys) if(k == 1) found_one = true;
    assert(found_one);

    // Values
    auto values = ht.values();
    assert(values.size() == 3);

    // For Each
    int sum_keys = 0;
    int sum_values = 0;
    ht.for_each([&](const int& k, int& v) {
        sum_keys += k;
        sum_values += v;
        v += 1; // Modify value
    });

    assert(sum_keys == 1 + 2 + 3);
    assert(sum_values == 10 + 20 + 30);
    assert(ht[1] == 11); // Verify modification
    END_TEST
}

int main() {
    std::cout << "=======================================" << std::endl;
    std::cout << "Running HashTable Test Suite" << std::endl;
    std::cout << "=======================================" << std::endl;

    test_default_constructor();
    test_initializer_list();
    test_insert_and_access();
    test_emplace();
    test_lookup_and_bounds();
    test_copy_move();
    test_modifications();
    //test_rehashing();
    test_iterators_and_views();

    std::cout << "=======================================" << std::endl;
    std::cout << "Tests Summary" << std::endl;
    std::cout << "Passed: " << tests_passed << std::endl;
    std::cout << "Failed: " << tests_failed << std::endl;
    std::cout << "=======================================" << std::endl;

    return tests_failed == 0 ? 0 : 1;
}