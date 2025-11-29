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
#include <string>
#include <vector>
#include <algorithm>
#include <random>

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

// ============================================
// Constructor Tests
// ============================================

void test_default_constructor() {
    TEST("Default constructor")
    HashTable<int, int> table;
    assert(table.empty());
    assert(table.size() == 0);
    assert(table.bucket_count() > 0);
    END_TEST
}

void test_bucket_count_constructor() {
    TEST("Constructor with bucket count")
    HashTable<int, int> table(32);
    assert(table.empty());
    assert(table.bucket_count() >= 32);
    END_TEST
}

void test_initializer_list() {
    TEST("Initializer list constructor")
    HashTable<std::string, int> table = {
        {"one", 1},
        {"two", 2},
        {"three", 3}
    };
    assert(table.size() == 3);
    assert(table["one"] == 1);
    assert(table["two"] == 2);
    assert(table["three"] == 3);
    END_TEST
}

void test_copy_constructor() {
    TEST("Copy constructor")
    HashTable<int, int> table1;
    table1[1] = 100;
    table1[2] = 200;
    table1[3] = 300;
    
    HashTable<int, int> table2(table1);
    assert(table2.size() == 3);
    assert(table2[1] == 100);
    assert(table2[2] == 200);
    
    // Verify deep copy
    table2[1] = 999;
    assert(table1[1] == 100);
    END_TEST
}

void test_move_constructor() {
    TEST("Move constructor")
    HashTable<int, int> table1;
    table1[1] = 100;
    table1[2] = 200;
    
    HashTable<int, int> table2(std::move(table1));
    assert(table2.size() == 2);
    assert(table2[1] == 100);
    assert(table1.empty());
    END_TEST
}

void test_copy_assignment() {
    TEST("Copy assignment operator")
    HashTable<int, int> table1;
    table1[1] = 100;
    table1[2] = 200;
    
    HashTable<int, int> table2;
    table2 = table1;
    assert(table2.size() == 2);
    assert(table2[1] == 100);
    
    table2[1] = 999;
    assert(table1[1] == 100);
    END_TEST
}

void test_move_assignment() {
    TEST("Move assignment operator")
    HashTable<int, int> table1;
    table1[1] = 100;
    table1[2] = 200;
    
    HashTable<int, int> table2;
    table2 = std::move(table1);
    assert(table2.size() == 2);
    assert(table2[1] == 100);
    assert(table1.empty());
    END_TEST
}

// ============================================
// Element Access Tests
// ============================================

void test_operator_brackets() {
    TEST("operator[] access and insert")
    HashTable<std::string, int> table;
    
    table["apple"] = 5;
    table["banana"] = 3;
    table["cherry"] = 7;
    
    assert(table.size() == 3);
    assert(table["apple"] == 5);
    assert(table["banana"] == 3);
    assert(table["cherry"] == 7);
    
    // Modify
    table["apple"] = 10;
    assert(table["apple"] == 10);
    END_TEST
}

void test_operator_brackets_default_insert() {
    TEST("operator[] inserts default value")
    HashTable<std::string, int> table;
    
    int& val = table["newkey"];
    assert(table.size() == 1);
    assert(val == 0);  // Default int value
    
    val = 42;
    assert(table["newkey"] == 42);
    END_TEST
}

void test_at_method() {
    TEST("at() method")
    HashTable<int, std::string> table;
    table[1] = "one";
    table[2] = "two";
    
    assert(table.at(1) == "one");
    assert(table.at(2) == "two");
    
    table.at(1) = "ONE";
    assert(table.at(1) == "ONE");
    END_TEST
}

void test_at_exception() {
    TEST("at() throws on missing key")
    HashTable<int, int> table;
    table[1] = 100;
    
    bool exception_thrown = false;
    try {
        table.at(999);
    } catch (const std::out_of_range&) {
        exception_thrown = true;
    }
    assert(exception_thrown);
    END_TEST
}

// ============================================
// Insert Tests
// ============================================

void test_insert_basic() {
    TEST("Basic insert")
    HashTable<int, int> table;
    
    auto result = table.insert(1, 100);
    assert(result.first == true);   // Success
    assert(result.second == true);  // Was inserted
    assert(table.size() == 1);
    assert(table[1] == 100);
    END_TEST
}

void test_insert_duplicate() {
    TEST("Insert duplicate key")
    HashTable<int, int> table;
    
    table.insert(1, 100);
    auto result = table.insert(1, 200);  // Duplicate
    
    assert(result.first == true);
    assert(result.second == false);  // Was NOT inserted
    assert(table[1] == 100);  // Original value unchanged
    assert(table.size() == 1);
    END_TEST
}

void test_insert_or_assign() {
    TEST("insert_or_assign")
    HashTable<int, int> table;
    
    bool inserted = table.insert_or_assign(1, 100);
    assert(inserted == true);
    assert(table[1] == 100);
    
    inserted = table.insert_or_assign(1, 200);
    assert(inserted == false);  // Was assigned, not inserted
    assert(table[1] == 200);
    END_TEST
}

void test_insert_multiple() {
    TEST("Insert multiple elements")
    HashTable<int, int> table;
    
    for (int i = 0; i < 100; ++i) {
        table.insert(i, i * 10);
    }
    
    assert(table.size() == 100);
    for (int i = 0; i < 100; ++i) {
        assert(table[i] == i * 10);
    }
    END_TEST
}

// ============================================
// Erase Tests
// ============================================

void test_erase_basic() {
    TEST("Basic erase")
    HashTable<int, int> table;
    table[1] = 100;
    table[2] = 200;
    table[3] = 300;
    
    bool erased = table.erase(2);
    assert(erased == true);
    assert(table.size() == 2);
    assert(!table.contains(2));
    assert(table.contains(1));
    assert(table.contains(3));
    END_TEST
}

void test_erase_nonexistent() {
    TEST("Erase non-existent key")
    HashTable<int, int> table;
    table[1] = 100;
    
    bool erased = table.erase(999);
    assert(erased == false);
    assert(table.size() == 1);
    END_TEST
}

void test_erase_all() {
    TEST("Erase all elements")
    HashTable<int, int> table;
    table[1] = 100;
    table[2] = 200;
    table[3] = 300;
    
    table.erase(1);
    table.erase(2);
    table.erase(3);
    
    assert(table.empty());
    assert(table.size() == 0);
    END_TEST
}

// ============================================
// Lookup Tests
// ============================================

void test_find() {
    TEST("find()")
    HashTable<std::string, int> table;
    table["apple"] = 5;
    table["banana"] = 3;
    
    int* found = table.find("apple");
    assert(found != nullptr);
    assert(*found == 5);
    
    found = table.find("orange");
    assert(found == nullptr);
    END_TEST
}

void test_contains() {
    TEST("contains()")
    HashTable<int, int> table;
    table[1] = 100;
    table[2] = 200;
    
    assert(table.contains(1));
    assert(table.contains(2));
    assert(!table.contains(3));
    assert(!table.contains(999));
    END_TEST
}

void test_count() {
    TEST("count()")
    HashTable<int, int> table;
    table[1] = 100;
    table[2] = 200;
    
    assert(table.count(1) == 1);
    assert(table.count(2) == 1);
    assert(table.count(3) == 0);
    END_TEST
}

// ============================================
// Capacity Tests
// ============================================

void test_empty() {
    TEST("empty()")
    HashTable<int, int> table;
    assert(table.empty());
    
    table[1] = 100;
    assert(!table.empty());
    
    table.erase(1);
    assert(table.empty());
    END_TEST
}

void test_size() {
    TEST("size()")
    HashTable<int, int> table;
    assert(table.size() == 0);
    
    table[1] = 100;
    assert(table.size() == 1);
    
    table[2] = 200;
    table[3] = 300;
    assert(table.size() == 3);
    
    table.erase(1);
    assert(table.size() == 2);
    END_TEST
}

void test_load_factor() {
    TEST("load_factor()")
    HashTable<int, int> table(10);
    
    assert(table.load_factor() == 0.0f);
    
    for (int i = 0; i < 5; ++i) {
        table[i] = i;
    }
    
    float lf = table.load_factor();
    assert(lf > 0.0f);
    END_TEST
}

void test_max_load_factor() {
    TEST("max_load_factor()")
    HashTable<int, int> table;
    
    float default_mlf = table.max_load_factor();
    assert(default_mlf > 0.0f && default_mlf <= 1.0f);
    
    table.max_load_factor(0.5f);
    assert(table.max_load_factor() == 0.5f);
    END_TEST
}

// ============================================
// Clear and Swap Tests
// ============================================

void test_clear() {
    TEST("clear()")
    HashTable<int, int> table;
    table[1] = 100;
    table[2] = 200;
    table[3] = 300;
    
    table.clear();
    assert(table.empty());
    assert(table.size() == 0);
    
    // Reuse after clear
    table[10] = 1000;
    assert(table.size() == 1);
    assert(table[10] == 1000);
    END_TEST
}

void test_swap() {
    TEST("swap()")
    HashTable<int, int> table1;
    table1[1] = 100;
    table1[2] = 200;
    
    HashTable<int, int> table2;
    table2[10] = 1000;
    
    table1.swap(table2);
    
    assert(table1.size() == 1);
    assert(table1[10] == 1000);
    assert(table2.size() == 2);
    assert(table2[1] == 100);
    END_TEST
}

// ============================================
// Rehash Tests
// ============================================

void test_rehash() {
    TEST("rehash()")
    HashTable<int, int> table;
    
    for (int i = 0; i < 10; ++i) {
        table[i] = i * 10;
    }
    
    size_t old_bucket_count = table.bucket_count();
    table.rehash(old_bucket_count * 2);
    
    assert(table.bucket_count() >= old_bucket_count * 2);
    assert(table.size() == 10);
    
    // Verify all elements still accessible
    for (int i = 0; i < 10; ++i) {
        assert(table[i] == i * 10);
    }
    END_TEST
}

void test_reserve() {
    TEST("reserve()")
    HashTable<int, int> table;
    
    table.reserve(100);
    
    for (int i = 0; i < 100; ++i) {
        table[i] = i;
    }
    
    assert(table.size() == 100);
    END_TEST
}

void test_auto_rehash() {
    TEST("Automatic rehashing on high load factor")
    HashTable<int, int> table(8);  // Small initial size
    size_t initial_buckets = table.bucket_count();
    
    // Insert many elements to trigger rehash
    for (int i = 0; i < 100; ++i) {
        table[i] = i;
    }
    
    assert(table.bucket_count() > initial_buckets);
    assert(table.load_factor() <= table.max_load_factor());
    
    // Verify all elements
    for (int i = 0; i < 100; ++i) {
        assert(table[i] == i);
    }
    END_TEST
}

// ============================================
// Bucket Interface Tests
// ============================================

void test_bucket() {
    TEST("bucket()")
    HashTable<int, int> table;
    table[1] = 100;
    
    size_t bucket_idx = table.bucket(1);
    assert(bucket_idx < table.bucket_count());
    END_TEST
}

void test_bucket_size() {
    TEST("bucket_size()")
    HashTable<int, int> table;
    table[1] = 100;
    
    size_t total_in_buckets = 0;
    for (size_t i = 0; i < table.bucket_count(); ++i) {
        total_in_buckets += table.bucket_size(i);
    }
    assert(total_in_buckets == table.size());
    END_TEST
}

// ============================================
// Iteration Tests
// ============================================

void test_for_each() {
    TEST("for_each()")
    HashTable<int, int> table;
    table[1] = 100;
    table[2] = 200;
    table[3] = 300;
    
    int sum = 0;
    table.for_each([&sum](const int& key, const int& value) {
        sum += value;
    });
    
    assert(sum == 600);
    END_TEST
}

void test_for_each_modify() {
    TEST("for_each() with modification")
    HashTable<int, int> table;
    table[1] = 100;
    table[2] = 200;
    table[3] = 300;
    
    table.for_each([](const int& key, int& value) {
        value *= 2;
    });
    
    assert(table[1] == 200);
    assert(table[2] == 400);
    assert(table[3] == 600);
    END_TEST
}

void test_keys() {
    TEST("keys()")
    HashTable<int, std::string> table;
    table[1] = "one";
    table[2] = "two";
    table[3] = "three";
    
    std::vector<int> keys = table.keys();
    assert(keys.size() == 3);
    
    std::sort(keys.begin(), keys.end());
    assert(keys[0] == 1);
    assert(keys[1] == 2);
    assert(keys[2] == 3);
    END_TEST
}

void test_values() {
    TEST("values()")
    HashTable<int, int> table;
    table[1] = 100;
    table[2] = 200;
    table[3] = 300;
    
    std::vector<int> values = table.values();
    assert(values.size() == 3);
    
    std::sort(values.begin(), values.end());
    assert(values[0] == 100);
    assert(values[1] == 200);
    assert(values[2] == 300);
    END_TEST
}

// ============================================
// String Key Tests
// ============================================

void test_string_keys() {
    TEST("String keys")
    HashTable<std::string, int> table;
    
    table["hello"] = 1;
    table["world"] = 2;
    table["foo"] = 3;
    table["bar"] = 4;
    
    assert(table.size() == 4);
    assert(table["hello"] == 1);
    assert(table["world"] == 2);
    assert(table.contains("foo"));
    assert(!table.contains("baz"));
    END_TEST
}

void test_string_values() {
    TEST("String values")
    HashTable<int, std::string> table;
    
    table[1] = "one";
    table[2] = "two";
    table[3] = "three";
    
    assert(table[1] == "one");
    assert(table[2] == "two");
    assert(table.at(3) == "three");
    END_TEST
}

void test_string_string() {
    TEST("String to string mapping")
    HashTable<std::string, std::string> table;
    
    table["name"] = "John";
    table["city"] = "Seoul";
    table["country"] = "Korea";
    
    assert(table["name"] == "John");
    assert(table["city"] == "Seoul");
    assert(table.size() == 3);
    END_TEST
}

// ============================================
// Edge Cases
// ============================================

void test_single_element() {
    TEST("Single element operations")
    HashTable<int, int> table;
    
    table[42] = 100;
    assert(table.size() == 1);
    assert(table[42] == 100);
    assert(table.contains(42));
    
    table.erase(42);
    assert(table.empty());
    assert(!table.contains(42));
    END_TEST
}

void test_collision_handling() {
    TEST("Collision handling")
    // Use small bucket count to force collisions
    HashTable<int, int> table(4);
    
    // These will likely collide in a 4-bucket table
    for (int i = 0; i < 20; ++i) {
        table[i] = i * 10;
    }
    
    assert(table.size() == 20);
    
    // Verify all elements
    for (int i = 0; i < 20; ++i) {
        assert(table[i] == i * 10);
    }
    END_TEST
}

void test_erase_and_reinsert() {
    TEST("Erase and reinsert")
    HashTable<int, int> table;
    
    table[1] = 100;
    table.erase(1);
    assert(!table.contains(1));
    
    table[1] = 200;
    assert(table[1] == 200);
    END_TEST
}

// ============================================
// Stress Tests
// ============================================

void test_large_dataset() {
    TEST("Large dataset (stress test)")
    HashTable<int, int> table;
    const int count = 10000;
    
    // Insert
    for (int i = 0; i < count; ++i) {
        table[i] = i * 10;
    }
    assert(table.size() == count);
    
    // Verify
    for (int i = 0; i < count; ++i) {
        assert(table[i] == i * 10);
    }
    
    // Erase half
    for (int i = 0; i < count / 2; ++i) {
        table.erase(i);
    }
    assert(table.size() == count / 2);
    
    // Verify remaining
    for (int i = count / 2; i < count; ++i) {
        assert(table[i] == i * 10);
    }
    END_TEST
}

void test_random_operations() {
    TEST("Random operations")
    HashTable<int, int> table;
    std::mt19937 rng(42);
    std::uniform_int_distribution<int> dist(0, 1000);
    
    // Random inserts
    for (int i = 0; i < 500; ++i) {
        int key = dist(rng);
        table[key] = key * 2;
    }
    
    // Random lookups and erases
    for (int i = 0; i < 200; ++i) {
        int key = dist(rng);
        if (table.contains(key)) {
            table.erase(key);
        }
    }
    
    // Verify integrity
    table.for_each([](const int& key, const int& value) {
        assert(value == key * 2);
    });
    END_TEST
}

void test_sequential_keys() {
    TEST("Sequential keys")
    HashTable<int, int> table;
    
    for (int i = 0; i < 1000; ++i) {
        table[i] = i;
    }
    
    for (int i = 0; i < 1000; ++i) {
        assert(table[i] == i);
    }
    END_TEST
}

// ============================================
// Practical Use Cases
// ============================================

void test_word_frequency() {
    TEST("Word frequency counter")
    HashTable<std::string, int> freq;
    
    std::vector<std::string> words = {
        "apple", "banana", "apple", "cherry", 
        "banana", "apple", "date", "cherry"
    };
    
    for (const auto& word : words) {
        freq[word]++;
    }
    
    assert(freq["apple"] == 3);
    assert(freq["banana"] == 2);
    assert(freq["cherry"] == 2);
    assert(freq["date"] == 1);
    END_TEST
}

void test_cache_simulation() {
    TEST("Cache simulation")
    HashTable<std::string, std::string> cache;
    
    // Simulate caching
    auto get_or_compute = [&cache](const std::string& key) -> std::string {
        if (cache.contains(key)) {
            return cache[key];
        }
        std::string computed = "result_for_" + key;
        cache[key] = computed;
        return computed;
    };
    
    assert(get_or_compute("query1") == "result_for_query1");
    assert(cache.size() == 1);
    
    assert(get_or_compute("query1") == "result_for_query1");
    assert(cache.size() == 1);  // Still 1, was cached
    
    assert(get_or_compute("query2") == "result_for_query2");
    assert(cache.size() == 2);
    END_TEST
}

void test_two_sum_problem() {
    TEST("Two sum problem using hash table")
    std::vector<int> nums = {2, 7, 11, 15};
    int target = 9;
    
    HashTable<int, int> seen;  // value -> index
    int result_i = -1, result_j = -1;
    
    for (int i = 0; i < static_cast<int>(nums.size()); ++i) {
        int complement = target - nums[i];
        if (seen.contains(complement)) {
            result_i = *seen.find(complement);
            result_j = i;
            break;
        }
        seen[nums[i]] = i;
    }
    
    assert(result_i == 0);
    assert(result_j == 1);
    assert(nums[result_i] + nums[result_j] == target);
    END_TEST
}

// Main test runner
int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "HashTable Test Suite" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;

    // Constructor tests
    std::cout << "--- Constructor Tests ---" << std::endl;
    test_default_constructor();
    test_bucket_count_constructor();
    test_initializer_list();
    test_copy_constructor();
    test_move_constructor();
    test_copy_assignment();
    test_move_assignment();

    // Element access tests
    std::cout << std::endl << "--- Element Access Tests ---" << std::endl;
    test_operator_brackets();
    test_operator_brackets_default_insert();
    test_at_method();
    test_at_exception();

    // Insert tests
    std::cout << std::endl << "--- Insert Tests ---" << std::endl;
    test_insert_basic();
    test_insert_duplicate();
    test_insert_or_assign();
    test_insert_multiple();

    // Erase tests
    std::cout << std::endl << "--- Erase Tests ---" << std::endl;
    test_erase_basic();
    test_erase_nonexistent();
    test_erase_all();

    // Lookup tests
    std::cout << std::endl << "--- Lookup Tests ---" << std::endl;
    test_find();
    test_contains();
    test_count();

    // Capacity tests
    std::cout << std::endl << "--- Capacity Tests ---" << std::endl;
    test_empty();
    test_size();
    test_load_factor();
    test_max_load_factor();

    // Clear and swap tests
    std::cout << std::endl << "--- Clear and Swap Tests ---" << std::endl;
    test_clear();
    test_swap();

    // Rehash tests
    std::cout << std::endl << "--- Rehash Tests ---" << std::endl;
    test_rehash();
    test_reserve();
    test_auto_rehash();

    // Bucket interface tests
    std::cout << std::endl << "--- Bucket Interface Tests ---" << std::endl;
    test_bucket();
    test_bucket_size();

    // Iteration tests
    std::cout << std::endl << "--- Iteration Tests ---" << std::endl;
    test_for_each();
    test_for_each_modify();
    test_keys();
    test_values();

    // String tests
    std::cout << std::endl << "--- String Tests ---" << std::endl;
    test_string_keys();
    test_string_values();
    test_string_string();

    // Edge cases
    std::cout << std::endl << "--- Edge Cases ---" << std::endl;
    test_single_element();
    test_collision_handling();
    test_erase_and_reinsert();

    // Stress tests
    std::cout << std::endl << "--- Stress Tests ---" << std::endl;
    test_large_dataset();
    test_random_operations();
    test_sequential_keys();

    // Practical use cases
    std::cout << std::endl << "--- Practical Use Cases ---" << std::endl;
    test_word_frequency();
    test_cache_simulation();
    test_two_sum_problem();

    // Print summary
    std::cout << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Test Results" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "✓ Passed: " << tests_passed << std::endl;
    std::cout << "✗ Failed: " << tests_failed << std::endl;
    std::cout << "Total: " << (tests_passed + tests_failed) << std::endl;
    std::cout << "========================================" << std::endl;

    return tests_failed > 0 ? 1 : 0;
}