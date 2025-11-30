/**
 * @file test_trie.cpp
 * @brief Test suite for Trie (Prefix Tree) data structure
 * @author Jinhyeok
 * @date 2025-11-30
 * @version 1.0.0
 */

#include "tree/trie.hpp"
#include <iostream>
#include <cassert>
#include <string>
#include <vector>
#include <algorithm>
#include <set>

using namespace mylib::tree;

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

// Helper to check if vector contains element
template <typename T>
bool contains(const std::vector<T>& vec, const T& elem) {
    return std::find(vec.begin(), vec.end(), elem) != vec.end();
}

// ============================================
// Constructor Tests
// ============================================

void test_default_constructor() {
    TEST("Default constructor")
    Trie<> trie;
    
    assert(trie.empty());
    assert(trie.size() == 0);
    END_TEST
}

void test_initializer_list_constructor() {
    TEST("Initializer list constructor")
    Trie<> trie = {"apple", "banana", "cherry"};
    
    assert(trie.size() == 3);
    assert(trie.search("apple"));
    assert(trie.search("banana"));
    assert(trie.search("cherry"));
    END_TEST
}

void test_iterator_constructor() {
    TEST("Iterator constructor")
    std::vector<std::string> words = {"dog", "cat", "bird"};
    Trie<> trie(words.begin(), words.end());
    
    assert(trie.size() == 3);
    assert(trie.search("dog"));
    assert(trie.search("cat"));
    assert(trie.search("bird"));
    END_TEST
}

void test_copy_constructor() {
    TEST("Copy constructor")
    Trie<> original = {"hello", "world"};
    Trie<> copy(original);
    
    assert(copy.size() == 2);
    assert(copy.search("hello"));
    assert(copy.search("world"));
    
    // Modify original, copy should be unchanged
    original.insert("test");
    assert(original.size() == 3);
    assert(copy.size() == 2);
    END_TEST
}

void test_move_constructor() {
    TEST("Move constructor")
    Trie<> original = {"hello", "world"};
    Trie<> moved(std::move(original));
    
    assert(moved.size() == 2);
    assert(moved.search("hello"));
    assert(moved.search("world"));
    END_TEST
}

void test_copy_assignment() {
    TEST("Copy assignment")
    Trie<> trie1 = {"one", "two"};
    Trie<> trie2 = {"three"};
    
    trie2 = trie1;
    
    assert(trie2.size() == 2);
    assert(trie2.search("one"));
    assert(trie2.search("two"));
    assert(!trie2.search("three"));
    END_TEST
}

void test_move_assignment() {
    TEST("Move assignment")
    Trie<> trie1 = {"one", "two"};
    Trie<> trie2 = {"three"};
    
    trie2 = std::move(trie1);
    
    assert(trie2.size() == 2);
    assert(trie2.search("one"));
    assert(trie2.search("two"));
    END_TEST
}

// ============================================
// Basic Operation Tests
// ============================================

void test_insert_and_search() {
    TEST("Insert and search")
    Trie<> trie;
    
    trie.insert("hello");
    trie.insert("world");
    
    assert(trie.search("hello"));
    assert(trie.search("world"));
    assert(!trie.search("hell"));
    assert(!trie.search("helloo"));
    assert(!trie.search("wor"));
    END_TEST
}

void test_insert_duplicate() {
    TEST("Insert duplicate")
    Trie<> trie;
    
    trie.insert("test");
    trie.insert("test");
    trie.insert("test");
    
    assert(trie.size() == 1);  // Unique count
    assert(trie.word_count("test") == 3);  // Total insertions
    END_TEST
}

void test_contains() {
    TEST("Contains (alias for search)")
    Trie<> trie = {"apple", "app"};
    
    assert(trie.contains("apple"));
    assert(trie.contains("app"));
    assert(!trie.contains("ap"));
    assert(!trie.contains("application"));
    END_TEST
}

void test_remove() {
    TEST("Remove")
    Trie<> trie = {"apple", "app", "application"};
    
    assert(trie.size() == 3);
    
    bool removed = trie.remove("app");
    assert(removed);
    assert(trie.size() == 2);
    assert(!trie.search("app"));
    assert(trie.search("apple"));
    assert(trie.search("application"));
    END_TEST
}

void test_remove_nonexistent() {
    TEST("Remove nonexistent")
    Trie<> trie = {"hello"};
    
    bool removed = trie.remove("world");
    assert(!removed);
    assert(trie.size() == 1);
    END_TEST
}

void test_remove_with_count() {
    TEST("Remove with word count")
    Trie<> trie;
    
    trie.insert("test");
    trie.insert("test");
    trie.insert("test");
    
    assert(trie.word_count("test") == 3);
    
    trie.remove("test");  // Removes one instance
    assert(trie.word_count("test") == 2);
    assert(trie.search("test"));
    
    trie.remove("test");
    trie.remove("test");
    assert(trie.word_count("test") == 0);
    assert(!trie.search("test"));
    END_TEST
}

void test_erase() {
    TEST("Erase (alias for remove)")
    Trie<> trie = {"hello", "world"};
    
    bool erased = trie.erase("hello");
    assert(erased);
    assert(!trie.search("hello"));
    assert(trie.search("world"));
    END_TEST
}

void test_empty_string() {
    TEST("Empty string")
    Trie<> trie;
    
    trie.insert("");
    assert(trie.search(""));
    assert(trie.size() == 1);
    
    trie.remove("");
    assert(!trie.search(""));
    assert(trie.size() == 0);
    END_TEST
}

// ============================================
// Prefix Operation Tests
// ============================================

void test_starts_with() {
    TEST("Starts with")
    Trie<> trie = {"apple", "application", "apply", "banana"};
    
    assert(trie.starts_with("app"));
    assert(trie.starts_with("apple"));
    assert(trie.starts_with("appl"));
    assert(trie.starts_with("ban"));
    assert(!trie.starts_with("ora"));
    assert(!trie.starts_with("applex"));
    END_TEST
}

void test_get_all_with_prefix() {
    TEST("Get all with prefix")
    Trie<> trie = {"apple", "application", "apply", "banana", "band"};
    
    auto app_words = trie.get_all_with_prefix("app");
    assert(app_words.size() == 3);
    assert(contains(app_words, std::string("apple")));
    assert(contains(app_words, std::string("application")));
    assert(contains(app_words, std::string("apply")));
    
    auto ban_words = trie.get_all_with_prefix("ban");
    assert(ban_words.size() == 2);
    
    auto empty_prefix = trie.get_all_with_prefix("");
    assert(empty_prefix.size() == 5);
    
    auto no_match = trie.get_all_with_prefix("xyz");
    assert(no_match.empty());
    END_TEST
}

void test_autocomplete() {
    TEST("Autocomplete")
    Trie<> trie = {"apple", "application", "apply", "apt", "banana"};
    
    auto suggestions = trie.autocomplete("app", 2);
    assert(suggestions.size() == 2);
    
    auto all_suggestions = trie.autocomplete("app", 0);  // 0 = unlimited
    assert(all_suggestions.size() == 3);
    
    auto no_suggestions = trie.autocomplete("xyz");
    assert(no_suggestions.empty());
    END_TEST
}

void test_count_with_prefix() {
    TEST("Count with prefix")
    Trie<> trie = {"apple", "application", "apply", "banana"};
    
    assert(trie.count_with_prefix("app") == 3);
    assert(trie.count_with_prefix("apple") == 1);
    assert(trie.count_with_prefix("ban") == 1);
    assert(trie.count_with_prefix("") == 4);
    assert(trie.count_with_prefix("xyz") == 0);
    END_TEST
}

// ============================================
// Word Count Tests
// ============================================

void test_word_count() {
    TEST("Word count")
    Trie<> trie;
    
    trie.insert("hello");
    trie.insert("hello");
    trie.insert("world");
    
    assert(trie.word_count("hello") == 2);
    assert(trie.word_count("world") == 1);
    assert(trie.word_count("test") == 0);
    END_TEST
}

void test_most_frequent() {
    TEST("Most frequent")
    Trie<> trie;
    
    trie.insert("apple");
    trie.insert("apple");
    trie.insert("apple");
    trie.insert("banana");
    trie.insert("banana");
    trie.insert("cherry");
    
    auto top2 = trie.most_frequent(2);
    assert(top2.size() == 2);
    assert(top2[0].first == "apple");
    assert(top2[0].second == 3);
    assert(top2[1].first == "banana");
    assert(top2[1].second == 2);
    
    auto all = trie.most_frequent(0);  // 0 = all
    assert(all.size() == 3);
    END_TEST
}

// ============================================
// Pattern Matching Tests
// ============================================

void test_search_pattern() {
    TEST("Search pattern with wildcard")
    Trie<> trie = {"hello", "hallo", "hullo", "help", "held"};
    
    auto matches = trie.search_pattern("h.llo");
    assert(matches.size() == 3);
    assert(contains(matches, std::string("hello")));
    assert(contains(matches, std::string("hallo")));
    assert(contains(matches, std::string("hullo")));
    
    auto matches2 = trie.search_pattern("hel.");
    assert(matches2.size() == 2);
    assert(contains(matches2, std::string("help")));
    assert(contains(matches2, std::string("held")));
    
    // 5-letter words only: hello, hallo, hullo
    auto matches3 = trie.search_pattern(".....");
    assert(matches3.size() == 3);
    
    // 4-letter words only: help, held
    auto matches4 = trie.search_pattern("....");
    assert(matches4.size() == 2);
    END_TEST
}

void test_matches_pattern() {
    TEST("Matches pattern")
    Trie<> trie = {"hello", "world"};
    
    assert(trie.matches_pattern("hello"));
    assert(trie.matches_pattern("h.llo"));
    assert(trie.matches_pattern("....."));
    assert(!trie.matches_pattern("h.ll"));
    assert(!trie.matches_pattern("......"));
    END_TEST
}

void test_pattern_multiple_wildcards() {
    TEST("Pattern with multiple wildcards")
    Trie<> trie = {"cat", "car", "cab", "can", "bat", "bar"};
    
    auto matches = trie.search_pattern("c..");
    assert(matches.size() == 4);
    
    auto matches2 = trie.search_pattern("..t");
    assert(matches2.size() == 2);
    assert(contains(matches2, std::string("cat")));
    assert(contains(matches2, std::string("bat")));
    END_TEST
}

// ============================================
// Utility Operation Tests
// ============================================

void test_get_all_words() {
    TEST("Get all words")
    Trie<> trie = {"apple", "banana", "cherry"};
    
    auto words = trie.get_all_words();
    assert(words.size() == 3);
    assert(contains(words, std::string("apple")));
    assert(contains(words, std::string("banana")));
    assert(contains(words, std::string("cherry")));
    END_TEST
}

void test_size_and_empty() {
    TEST("Size and empty")
    Trie<> trie;
    
    assert(trie.empty());
    assert(trie.size() == 0);
    
    trie.insert("hello");
    assert(!trie.empty());
    assert(trie.size() == 1);
    
    trie.insert("world");
    assert(trie.size() == 2);
    
    trie.remove("hello");
    assert(trie.size() == 1);
    END_TEST
}

void test_clear() {
    TEST("Clear")
    Trie<> trie = {"one", "two", "three"};
    
    assert(trie.size() == 3);
    
    trie.clear();
    
    assert(trie.empty());
    assert(trie.size() == 0);
    assert(!trie.search("one"));
    END_TEST
}

void test_longest_common_prefix() {
    TEST("Longest common prefix")
    Trie<> trie1 = {"flower", "flow", "flight"};
    assert(trie1.longest_common_prefix() == "fl");
    
    Trie<> trie2 = {"dog", "car", "race"};
    assert(trie2.longest_common_prefix() == "");
    
    Trie<> trie3 = {"test"};
    assert(trie3.longest_common_prefix() == "test");
    
    Trie<> trie4;
    assert(trie4.longest_common_prefix() == "");
    END_TEST
}

void test_longest_word() {
    TEST("Longest word")
    Trie<> trie = {"a", "abc", "abcdef", "ab"};
    
    assert(trie.longest_word() == "abcdef");
    
    Trie<> empty_trie;
    assert(empty_trie.longest_word() == "");
    END_TEST
}

void test_shortest_word() {
    TEST("Shortest word")
    Trie<> trie = {"abc", "abcdef", "ab", "a"};
    
    assert(trie.shortest_word() == "a");
    
    Trie<> empty_trie;
    assert(empty_trie.shortest_word() == "");
    END_TEST
}

void test_for_each() {
    TEST("For each")
    Trie<> trie = {"apple", "banana", "cherry"};
    
    std::vector<std::string> collected;
    trie.for_each([&collected](const std::string& word) {
        collected.push_back(word);
    });
    
    assert(collected.size() == 3);
    assert(contains(collected, std::string("apple")));
    assert(contains(collected, std::string("banana")));
    assert(contains(collected, std::string("cherry")));
    END_TEST
}

// ============================================
// Fuzzy Search Tests
// ============================================

void test_fuzzy_search_exact() {
    TEST("Fuzzy search exact match")
    Trie<> trie = {"hello", "world", "help"};
    
    auto results = trie.fuzzy_search("hello", 0);
    assert(results.size() == 1);
    assert(results[0].first == "hello");
    assert(results[0].second == 0);
    END_TEST
}

void test_fuzzy_search_distance_1() {
    TEST("Fuzzy search distance 1")
    Trie<> trie = {"hello", "hallo", "help", "world"};
    
    auto results = trie.fuzzy_search("hello", 1);
    assert(results.size() == 2);  // hello (0), hallo (1)
    assert(results[0].first == "hello");
    assert(results[0].second == 0);
    assert(results[1].first == "hallo");
    assert(results[1].second == 1);
    END_TEST
}

void test_fuzzy_search_distance_2() {
    TEST("Fuzzy search distance 2")
    Trie<> trie = {"cat", "car", "card", "care", "bat", "rat"};
    
    auto results = trie.fuzzy_search("cat", 2);
    // cat (0), car (1), bat (1), rat (1), card (2), care (2)
    assert(results.size() >= 3);
    assert(results[0].first == "cat");
    assert(results[0].second == 0);
    END_TEST
}

// ============================================
// Edge Cases
// ============================================

void test_single_character_words() {
    TEST("Single character words")
    Trie<> trie;
    
    trie.insert("a");
    trie.insert("b");
    trie.insert("c");
    
    assert(trie.size() == 3);
    assert(trie.search("a"));
    assert(trie.search("b"));
    assert(trie.search("c"));
    assert(!trie.search("d"));
    END_TEST
}

void test_prefix_is_also_word() {
    TEST("Prefix is also a word")
    Trie<> trie = {"app", "apple", "application"};
    
    assert(trie.search("app"));
    assert(trie.search("apple"));
    assert(trie.search("application"));
    
    // Remove middle word
    trie.remove("apple");
    assert(trie.search("app"));
    assert(!trie.search("apple"));
    assert(trie.search("application"));
    END_TEST
}

void test_similar_words() {
    TEST("Similar words")
    Trie<> trie = {"test", "testing", "tested", "tester", "tests"};
    
    assert(trie.size() == 5);
    
    auto with_prefix = trie.get_all_with_prefix("test");
    assert(with_prefix.size() == 5);
    END_TEST
}

void test_special_characters() {
    TEST("Special characters")
    Trie<> trie;
    
    trie.insert("hello-world");
    trie.insert("hello_world");
    trie.insert("hello.world");
    
    assert(trie.search("hello-world"));
    assert(trie.search("hello_world"));
    assert(trie.search("hello.world"));
    END_TEST
}

void test_numbers_in_words() {
    TEST("Numbers in words")
    Trie<> trie;
    
    trie.insert("test123");
    trie.insert("123test");
    trie.insert("te123st");
    
    assert(trie.size() == 3);
    assert(trie.search("test123"));
    assert(trie.search("123test"));
    END_TEST
}

// ============================================
// Large Scale Tests
// ============================================

void test_large_insertion() {
    TEST("Large insertion (1000 words)")
    Trie<> trie;
    
    for (int i = 0; i < 1000; ++i) {
        trie.insert("word" + std::to_string(i));
    }
    
    assert(trie.size() == 1000);
    assert(trie.search("word0"));
    assert(trie.search("word500"));
    assert(trie.search("word999"));
    assert(!trie.search("word1000"));
    END_TEST
}

void test_large_prefix_query() {
    TEST("Large prefix query")
    Trie<> trie;
    
    // Insert words with common prefix
    for (int i = 0; i < 100; ++i) {
        trie.insert("prefix" + std::to_string(i));
    }
    for (int i = 0; i < 50; ++i) {
        trie.insert("other" + std::to_string(i));
    }
    
    auto prefix_words = trie.get_all_with_prefix("prefix");
    assert(prefix_words.size() == 100);
    
    auto other_words = trie.get_all_with_prefix("other");
    assert(other_words.size() == 50);
    END_TEST
}

void test_long_words() {
    TEST("Long words")
    Trie<> trie;
    
    std::string long_word(1000, 'a');
    trie.insert(long_word);
    
    assert(trie.search(long_word));
    assert(trie.starts_with(long_word.substr(0, 500)));
    END_TEST
}

// ============================================
// Practical Use Cases
// ============================================

void test_dictionary() {
    TEST("Dictionary use case")
    Trie<> dictionary = {
        "apple", "application", "apply", "banana", "band", "bandana",
        "cat", "car", "card", "care", "careful", "careless"
    };
    
    // Spell check
    assert(dictionary.search("apple"));
    assert(!dictionary.search("aple"));  // Misspelling
    
    // Find similar words
    auto suggestions = dictionary.fuzzy_search("aple", 1);
    assert(contains(suggestions, std::make_pair(std::string("apple"), std::size_t(1))));
    END_TEST
}

void test_autocomplete_system() {
    TEST("Autocomplete system")
    Trie<> trie;
    
    // Simulate search history with frequencies
    trie.insert("how to");
    trie.insert("how to");
    trie.insert("how to");
    trie.insert("how are you");
    trie.insert("how are you");
    trie.insert("how much");
    trie.insert("hello world");
    
    auto suggestions = trie.autocomplete("how", 3);
    assert(suggestions.size() == 3);
    
    // Most frequent should be available
    auto frequent = trie.most_frequent(2);
    assert(frequent[0].first == "how to");
    assert(frequent[0].second == 3);
    END_TEST
}

void test_contact_search() {
    TEST("Contact search")
    Trie<> contacts = {
        "John Smith", "John Doe", "Jane Doe", "Jane Smith",
        "Bob Johnson", "Bob Williams"
    };
    
    auto johns = contacts.get_all_with_prefix("John");
    assert(johns.size() == 2);
    
    auto janes = contacts.get_all_with_prefix("Jane");
    assert(janes.size() == 2);
    
    auto bobs = contacts.get_all_with_prefix("Bob");
    assert(bobs.size() == 2);
    END_TEST
}

void test_ip_prefix_matching() {
    TEST("IP prefix matching")
    Trie<> ip_trie = {
        "192.168.1.1",
        "192.168.1.2",
        "192.168.2.1",
        "10.0.0.1",
        "10.0.0.2"
    };
    
    auto local = ip_trie.get_all_with_prefix("192.168");
    assert(local.size() == 3);
    
    auto subnet = ip_trie.get_all_with_prefix("192.168.1");
    assert(subnet.size() == 2);
    END_TEST
}

void test_word_game() {
    TEST("Word game (find words matching pattern)")
    Trie<> dictionary = {
        "cat", "car", "can", "cap", "bat", "bar", "ban",
        "hat", "hot", "hit", "rat", "ran", "run"
    };
    
    // Find 3-letter words ending in 'at'
    auto at_words = dictionary.search_pattern("..t");
    assert(contains(at_words, std::string("cat")));
    assert(contains(at_words, std::string("bat")));
    assert(contains(at_words, std::string("hat")));
    assert(contains(at_words, std::string("hot")));
    assert(contains(at_words, std::string("hit")));
    
    // Find words starting with 'c' and ending with any letter
    auto c_words = dictionary.search_pattern("c..");
    assert(c_words.size() == 4);  // cat, car, can, cap
    END_TEST
}

// Main test runner
int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "Trie (Prefix Tree) Test Suite" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;

    // Constructor tests
    std::cout << "--- Constructor Tests ---" << std::endl;
    test_default_constructor();
    test_initializer_list_constructor();
    test_iterator_constructor();
    test_copy_constructor();
    test_move_constructor();
    test_copy_assignment();
    test_move_assignment();

    // Basic operation tests
    std::cout << std::endl << "--- Basic Operation Tests ---" << std::endl;
    test_insert_and_search();
    test_insert_duplicate();
    test_contains();
    test_remove();
    test_remove_nonexistent();
    test_remove_with_count();
    test_erase();
    test_empty_string();

    // Prefix operation tests
    std::cout << std::endl << "--- Prefix Operation Tests ---" << std::endl;
    test_starts_with();
    test_get_all_with_prefix();
    test_autocomplete();
    test_count_with_prefix();

    // Word count tests
    std::cout << std::endl << "--- Word Count Tests ---" << std::endl;
    test_word_count();
    test_most_frequent();

    // Pattern matching tests
    std::cout << std::endl << "--- Pattern Matching Tests ---" << std::endl;
    test_search_pattern();
    test_matches_pattern();
    test_pattern_multiple_wildcards();

    // Utility operation tests
    std::cout << std::endl << "--- Utility Operation Tests ---" << std::endl;
    test_get_all_words();
    test_size_and_empty();
    test_clear();
    test_longest_common_prefix();
    test_longest_word();
    test_shortest_word();
    test_for_each();

    // Fuzzy search tests
    std::cout << std::endl << "--- Fuzzy Search Tests ---" << std::endl;
    test_fuzzy_search_exact();
    test_fuzzy_search_distance_1();
    test_fuzzy_search_distance_2();

    // Edge cases
    std::cout << std::endl << "--- Edge Cases ---" << std::endl;
    test_single_character_words();
    test_prefix_is_also_word();
    test_similar_words();
    test_special_characters();
    test_numbers_in_words();

    // Large scale tests
    std::cout << std::endl << "--- Large Scale Tests ---" << std::endl;
    test_large_insertion();
    test_large_prefix_query();
    test_long_words();

    // Practical use cases
    std::cout << std::endl << "--- Practical Use Cases ---" << std::endl;
    test_dictionary();
    test_autocomplete_system();
    test_contact_search();
    test_ip_prefix_matching();
    test_word_game();

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