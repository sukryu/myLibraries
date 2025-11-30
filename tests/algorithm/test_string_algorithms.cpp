/**
 * @file test_string_algorithms.cpp
 * @brief Test suite for string algorithms
 * @author Jinhyeok
 * @date 2025-11-30
 * @version 1.0.0
 */

#include "algorithm/string_algorithms.hpp"
#include <iostream>
#include <cassert>
#include <string>
#include <vector>

using namespace mylib::algorithm;

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
// KMP Tests
// ============================================

void test_kmp_basic() {
    TEST("KMP basic search")
    auto result = KMP::search("hello world", "world");
    
    assert(result.found());
    assert(result.count() == 1);
    assert(result.first().value() == 6);
    END_TEST
}

void test_kmp_multiple_matches() {
    TEST("KMP multiple matches")
    auto result = KMP::search("abababab", "ab");
    
    assert(result.found());
    assert(result.count() == 4);
    assert(result.positions[0] == 0);
    assert(result.positions[1] == 2);
    assert(result.positions[2] == 4);
    assert(result.positions[3] == 6);
    END_TEST
}

void test_kmp_no_match() {
    TEST("KMP no match")
    auto result = KMP::search("hello world", "xyz");
    
    assert(!result.found());
    assert(result.count() == 0);
    assert(!result.first().has_value());
    END_TEST
}

void test_kmp_empty_pattern() {
    TEST("KMP empty pattern")
    auto result = KMP::search("hello", "");
    
    assert(result.found());
    assert(result.count() == 6);  // Matches at every position including end
    END_TEST
}

void test_kmp_empty_text() {
    TEST("KMP empty text")
    auto result = KMP::search("", "pattern");
    
    assert(!result.found());
    END_TEST
}

void test_kmp_pattern_longer_than_text() {
    TEST("KMP pattern longer than text")
    auto result = KMP::search("hi", "hello");
    
    assert(!result.found());
    END_TEST
}

void test_kmp_full_match() {
    TEST("KMP pattern equals text")
    auto result = KMP::search("hello", "hello");
    
    assert(result.found());
    assert(result.count() == 1);
    assert(result.first().value() == 0);
    END_TEST
}

void test_kmp_overlapping_matches() {
    TEST("KMP overlapping matches")
    auto result = KMP::search("aaaa", "aa");
    
    assert(result.found());
    assert(result.count() == 3);  // positions: 0, 1, 2
    END_TEST
}

void test_kmp_contains() {
    TEST("KMP contains")
    assert(KMP::contains("hello world", "world"));
    assert(!KMP::contains("hello world", "xyz"));
    assert(KMP::contains("hello", ""));
    END_TEST
}

void test_kmp_count() {
    TEST("KMP count")
    assert(KMP::count("banana", "ana") == 2);
    assert(KMP::count("hello", "l") == 2);
    assert(KMP::count("hello", "x") == 0);
    END_TEST
}

void test_kmp_lps_array() {
    TEST("KMP LPS array computation")
    auto lps1 = KMP::compute_lps("AAAA");
    assert(lps1.size() == 4);
    assert(lps1[0] == 0);
    assert(lps1[1] == 1);
    assert(lps1[2] == 2);
    assert(lps1[3] == 3);
    
    auto lps2 = KMP::compute_lps("ABCABC");
    assert(lps2[0] == 0);
    assert(lps2[1] == 0);
    assert(lps2[2] == 0);
    assert(lps2[3] == 1);
    assert(lps2[4] == 2);
    assert(lps2[5] == 3);
    
    auto lps3 = KMP::compute_lps("AABAACAABAA");
    assert(lps3[0] == 0);
    assert(lps3[1] == 1);
    assert(lps3[9] == 4);
    assert(lps3[10] == 5);
    END_TEST
}

void test_kmp_reusable_matcher() {
    TEST("KMP reusable matcher")
    KMP matcher("test");
    
    auto r1 = matcher.match("this is a test");
    assert(r1.found());
    assert(r1.first().value() == 10);
    
    auto r2 = matcher.match("test test test");
    assert(r2.count() == 3);
    
    auto r3 = matcher.match("no match here");
    assert(!r3.found());
    END_TEST
}

void test_kmp_replace_all() {
    TEST("KMP replace_all")
    auto result = KMP::replace_all("hello world", "world", "universe");
    assert(result == "hello universe");
    
    auto result2 = KMP::replace_all("abab", "ab", "x");
    assert(result2 == "xx");
    
    auto result3 = KMP::replace_all("hello", "x", "y");
    assert(result3 == "hello");
    END_TEST
}

void test_kmp_split() {
    TEST("KMP split")
    auto result = KMP::split("a,b,c,d", ",");
    assert(result.size() == 4);
    assert(result[0] == "a");
    assert(result[1] == "b");
    assert(result[2] == "c");
    assert(result[3] == "d");
    
    auto result2 = KMP::split("hello", ",");
    assert(result2.size() == 1);
    assert(result2[0] == "hello");
    
    auto result3 = KMP::split("a::b::c", "::");
    assert(result3.size() == 3);
    END_TEST
}

// ============================================
// Rabin-Karp Tests
// ============================================

void test_rabin_karp_basic() {
    TEST("Rabin-Karp basic search")
    auto result = RabinKarp::search("hello world", "world");
    
    assert(result.found());
    assert(result.count() == 1);
    assert(result.first().value() == 6);
    END_TEST
}

void test_rabin_karp_multiple_matches() {
    TEST("Rabin-Karp multiple matches")
    auto result = RabinKarp::search("abababab", "ab");
    
    assert(result.found());
    assert(result.count() == 4);
    END_TEST
}

void test_rabin_karp_no_match() {
    TEST("Rabin-Karp no match")
    auto result = RabinKarp::search("hello world", "xyz");
    
    assert(!result.found());
    END_TEST
}

void test_rabin_karp_contains() {
    TEST("Rabin-Karp contains")
    assert(RabinKarp::contains("hello world", "world"));
    assert(!RabinKarp::contains("hello world", "xyz"));
    END_TEST
}

void test_rabin_karp_reusable_matcher() {
    TEST("Rabin-Karp reusable matcher")
    RabinKarp matcher("test");
    
    auto r1 = matcher.match("this is a test");
    assert(r1.found());
    
    auto r2 = matcher.match("test test test");
    assert(r2.count() == 3);
    END_TEST
}

void test_rabin_karp_multiple_patterns() {
    TEST("Rabin-Karp multiple patterns")
    std::vector<std::string> patterns = {"cat", "dog", "bird"};
    auto result = RabinKarp::search_multiple("I have a cat and a dog and a bird", patterns);
    
    assert(result.found());
    assert(result.count() == 3);
    
    auto cat_positions = result.positions_for(0);
    assert(cat_positions.size() == 1);
    assert(cat_positions[0] == 9);
    
    auto dog_positions = result.positions_for(1);
    assert(dog_positions.size() == 1);
    END_TEST
}

void test_rabin_karp_multiple_patterns_same() {
    TEST("Rabin-Karp multiple same patterns")
    std::vector<std::string> patterns = {"ab", "ab"};
    auto result = RabinKarp::search_multiple("abab", patterns);
    
    assert(result.found());
    // Both patterns match at same positions
    END_TEST
}

void test_rabin_karp_multiple_patterns_different_lengths() {
    TEST("Rabin-Karp multiple patterns different lengths")
    std::vector<std::string> patterns = {"a", "ab", "abc"};
    auto result = RabinKarp::search_multiple("abc", patterns);
    
    assert(result.found());
    assert(result.count() >= 3);
    END_TEST
}

void test_rabin_karp_hash_computation() {
    TEST("Rabin-Karp hash computation")
    auto hash1 = RabinKarp::compute_hash_static("hello");
    auto hash2 = RabinKarp::compute_hash_static("hello");
    auto hash3 = RabinKarp::compute_hash_static("world");
    
    assert(hash1 == hash2);
    assert(hash1 != hash3);
    END_TEST
}

void test_rabin_karp_repeated_substrings() {
    TEST("Rabin-Karp find repeated substrings")
    auto result = RabinKarp::find_repeated_substrings("banana", 2);
    
    // "an" and "na" appear twice
    assert(result.size() >= 1);
    END_TEST
}

// ============================================
// String Utility Tests
// ============================================

void test_starts_with() {
    TEST("starts_with")
    assert(starts_with("hello world", "hello"));
    assert(starts_with("hello", "hello"));
    assert(starts_with("hello", ""));
    assert(!starts_with("hello", "world"));
    assert(!starts_with("hi", "hello"));
    END_TEST
}

void test_ends_with() {
    TEST("ends_with")
    assert(ends_with("hello world", "world"));
    assert(ends_with("hello", "hello"));
    assert(ends_with("hello", ""));
    assert(!ends_with("hello", "hi"));
    assert(!ends_with("hi", "hello"));
    END_TEST
}

void test_longest_common_prefix() {
    TEST("longest_common_prefix")
    assert(longest_common_prefix("hello", "help") == "hel");
    assert(longest_common_prefix("abc", "xyz") == "");
    assert(longest_common_prefix("test", "test") == "test");
    assert(longest_common_prefix("", "hello") == "");
    END_TEST
}

void test_longest_common_suffix() {
    TEST("longest_common_suffix")
    assert(longest_common_suffix("testing", "running") == "ing");
    assert(longest_common_suffix("abc", "xyz") == "");
    assert(longest_common_suffix("test", "test") == "test");
    END_TEST
}

void test_is_palindrome() {
    TEST("is_palindrome")
    assert(is_palindrome("racecar"));
    assert(is_palindrome("a"));
    assert(is_palindrome(""));
    assert(is_palindrome("abba"));
    assert(!is_palindrome("hello"));
    assert(!is_palindrome("ab"));
    END_TEST
}

void test_longest_palindrome() {
    TEST("longest_palindrome")
    assert(longest_palindrome("babad") == "bab" || longest_palindrome("babad") == "aba");
    assert(longest_palindrome("cbbd") == "bb");
    assert(longest_palindrome("a") == "a");
    assert(longest_palindrome("racecar") == "racecar");
    END_TEST
}

void test_edit_distance() {
    TEST("edit_distance")
    assert(edit_distance("kitten", "sitting") == 3);
    assert(edit_distance("", "") == 0);
    assert(edit_distance("hello", "hello") == 0);
    assert(edit_distance("abc", "") == 3);
    assert(edit_distance("", "abc") == 3);
    assert(edit_distance("a", "b") == 1);
    END_TEST
}

void test_lcs_length() {
    TEST("lcs_length")
    assert(lcs_length("ABCDGH", "AEDFHR") == 3);  // ADH
    assert(lcs_length("AGGTAB", "GXTXAYB") == 4); // GTAB
    assert(lcs_length("", "abc") == 0);
    assert(lcs_length("abc", "abc") == 3);
    END_TEST
}

void test_lcs() {
    TEST("lcs")
    assert(lcs("ABCDGH", "AEDFHR") == "ADH");
    assert(lcs("AGGTAB", "GXTXAYB") == "GTAB");
    assert(lcs("abc", "abc") == "abc");
    assert(lcs("", "abc") == "");
    END_TEST
}

// ============================================
// Edge Cases
// ============================================

void test_single_character_pattern() {
    TEST("Single character pattern")
    auto result = KMP::search("hello", "l");
    assert(result.count() == 2);
    assert(result.positions[0] == 2);
    assert(result.positions[1] == 3);
    END_TEST
}

void test_special_characters() {
    TEST("Special characters")
    auto result = KMP::search("hello@world.com", "@");
    assert(result.found());
    assert(result.first().value() == 5);
    
    auto result2 = RabinKarp::search("path/to/file", "/");
    assert(result2.count() == 2);
    END_TEST
}

void test_unicode_basic() {
    TEST("Unicode basic (ASCII subset)")
    // Test with extended ASCII
    auto result = KMP::search("cafe", "fe");
    assert(result.found());
    END_TEST
}

void test_long_pattern() {
    TEST("Long pattern")
    std::string text = "The quick brown fox jumps over the lazy dog";
    std::string pattern = "quick brown fox";
    
    auto result = KMP::search(text, pattern);
    assert(result.found());
    assert(result.first().value() == 4);
    END_TEST
}

void test_repeated_characters() {
    TEST("Repeated characters")
    auto result = KMP::search("aaaaaaaaaab", "aaab");
    assert(result.found());
    assert(result.first().value() == 7);
    END_TEST
}

// ============================================
// Large Scale Tests
// ============================================

void test_kmp_large_text() {
    TEST("KMP large text (10000 chars)")
    std::string text(10000, 'a');
    text[5000] = 'b';
    
    auto result = KMP::search(text, "ab");
    assert(result.found());
    assert(result.first().value() == 4999);
    END_TEST
}

void test_rabin_karp_large_text() {
    TEST("Rabin-Karp large text (10000 chars)")
    std::string text(10000, 'a');
    text[5000] = 'b';
    
    auto result = RabinKarp::search(text, "ab");
    assert(result.found());
    assert(result.first().value() == 4999);
    END_TEST
}

void test_multiple_patterns_large() {
    TEST("Multiple patterns large")
    std::string text = "The quick brown fox jumps over the lazy dog. "
                       "The quick brown fox jumps over the lazy dog.";
    std::vector<std::string> patterns = {"quick", "brown", "fox", "lazy", "dog"};
    
    auto result = RabinKarp::search_multiple(text, patterns);
    assert(result.found());
    assert(result.count() == 10);  // Each pattern appears twice
    END_TEST
}

// ============================================
// Practical Use Cases
// ============================================

void test_dna_sequence() {
    TEST("DNA sequence pattern matching")
    std::string dna = "ATCGATCGATCGATCG";
    std::string pattern = "GATC";
    
    auto result = KMP::search(dna, pattern);
    assert(result.count() == 3);
    END_TEST
}

void test_log_parsing() {
    TEST("Log parsing")
    std::string log = "[ERROR] Connection failed\n[INFO] Retry attempt\n[ERROR] Timeout";
    
    auto result = KMP::search(log, "[ERROR]");
    assert(result.count() == 2);
    END_TEST
}

void test_find_urls() {
    TEST("Find URLs")
    std::string text = "Visit http://example.com or http://test.org for more info";
    
    auto result = KMP::search(text, "http://");
    assert(result.count() == 2);
    END_TEST
}

void test_word_boundary() {
    TEST("Word search")
    std::string text = "testing test tested tester";
    
    auto result = KMP::search(text, "test");
    assert(result.count() == 4);  // test appears in all words
    END_TEST
}

void test_csv_parsing() {
    TEST("CSV parsing")
    std::string csv = "name,age,city\nJohn,30,Seoul\nJane,25,Busan";
    
    auto lines = KMP::split(csv, "\n");
    assert(lines.size() == 3);
    
    auto fields = KMP::split(lines[0], ",");
    assert(fields.size() == 3);
    assert(fields[0] == "name");
    END_TEST
}

// ============================================
// Comparison Tests
// ============================================

void test_kmp_vs_rabin_karp() {
    TEST("KMP vs Rabin-Karp same results")
    std::string text = "abcabcabc";
    std::string pattern = "abc";
    
    auto kmp_result = KMP::search(text, pattern);
    auto rk_result = RabinKarp::search(text, pattern);
    
    assert(kmp_result.count() == rk_result.count());
    assert(kmp_result.positions == rk_result.positions);
    END_TEST
}

// Main test runner
int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "String Algorithms Test Suite" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;

    // KMP tests
    std::cout << "--- KMP Tests ---" << std::endl;
    test_kmp_basic();
    test_kmp_multiple_matches();
    test_kmp_no_match();
    test_kmp_empty_pattern();
    test_kmp_empty_text();
    test_kmp_pattern_longer_than_text();
    test_kmp_full_match();
    test_kmp_overlapping_matches();
    test_kmp_contains();
    test_kmp_count();
    test_kmp_lps_array();
    test_kmp_reusable_matcher();
    test_kmp_replace_all();
    test_kmp_split();

    // Rabin-Karp tests
    std::cout << std::endl << "--- Rabin-Karp Tests ---" << std::endl;
    test_rabin_karp_basic();
    test_rabin_karp_multiple_matches();
    test_rabin_karp_no_match();
    test_rabin_karp_contains();
    test_rabin_karp_reusable_matcher();
    test_rabin_karp_multiple_patterns();
    test_rabin_karp_multiple_patterns_same();
    test_rabin_karp_multiple_patterns_different_lengths();
    test_rabin_karp_hash_computation();
    test_rabin_karp_repeated_substrings();

    // Utility function tests
    std::cout << std::endl << "--- String Utility Tests ---" << std::endl;
    test_starts_with();
    test_ends_with();
    test_longest_common_prefix();
    test_longest_common_suffix();
    test_is_palindrome();
    test_longest_palindrome();
    test_edit_distance();
    test_lcs_length();
    test_lcs();

    // Edge cases
    std::cout << std::endl << "--- Edge Cases ---" << std::endl;
    test_single_character_pattern();
    test_special_characters();
    test_unicode_basic();
    test_long_pattern();
    test_repeated_characters();

    // Large scale tests
    std::cout << std::endl << "--- Large Scale Tests ---" << std::endl;
    test_kmp_large_text();
    test_rabin_karp_large_text();
    test_multiple_patterns_large();

    // Practical use cases
    std::cout << std::endl << "--- Practical Use Cases ---" << std::endl;
    test_dna_sequence();
    test_log_parsing();
    test_find_urls();
    test_word_boundary();
    test_csv_parsing();

    // Comparison tests
    std::cout << std::endl << "--- Comparison Tests ---" << std::endl;
    test_kmp_vs_rabin_karp();

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