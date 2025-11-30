/**
 * @file string_algorithms.hpp
 * @brief String pattern matching and manipulation algorithms
 * @author Jinhyeok
 * @date 2025-11-30
 * @version 1.0.0
 * 
 * This file provides various string algorithms with:
 * - Multiple pattern matching algorithms
 * - Flexible interfaces for different use cases
 * - Support for both std::string and string_view
 * - Detailed match result information
 * 
 * Algorithms included:
 * - KMP (Knuth-Morris-Pratt): Efficient single pattern matching
 * - Rabin-Karp: Hash-based pattern matching, good for multiple patterns
 * - Utility functions: LPS array, rolling hash
 * 
 * Copyright (c) 2025 Jinhyeok
 * MIT License
 */

#ifndef MYLIB_ALGORITHM_STRING_ALGORITHMS_HPP
#define MYLIB_ALGORITHM_STRING_ALGORITHMS_HPP

#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>
#include <vector>
#include <unordered_set>
#include <functional>
#include <optional>
#include <algorithm>

namespace mylib {
namespace algorithm {

// ============================================
// Result structures
// ============================================

/**
 * @struct MatchResult
 * @brief Result of a pattern matching operation
 */
struct MatchResult {
    std::vector<std::size_t> positions;  ///< All positions where pattern was found
    
    /**
     * @brief Check if pattern was found at least once
     */
    bool found() const {
        return !positions.empty();
    }
    
    /**
     * @brief Get the first match position
     */
    std::optional<std::size_t> first() const {
        if (positions.empty()) return std::nullopt;
        return positions.front();
    }
    
    /**
     * @brief Get the last match position
     */
    std::optional<std::size_t> last() const {
        if (positions.empty()) return std::nullopt;
        return positions.back();
    }
    
    /**
     * @brief Get number of matches
     */
    std::size_t count() const {
        return positions.size();
    }
};

/**
 * @struct MultiMatchResult
 * @brief Result of matching multiple patterns
 */
struct MultiMatchResult {
    std::vector<std::pair<std::size_t, std::size_t>> matches;  ///< (position, pattern_index)
    
    /**
     * @brief Check if any pattern was found
     */
    bool found() const {
        return !matches.empty();
    }
    
    /**
     * @brief Get total number of matches across all patterns
     */
    std::size_t count() const {
        return matches.size();
    }
    
    /**
     * @brief Get matches for a specific pattern index
     */
    std::vector<std::size_t> positions_for(std::size_t pattern_index) const {
        std::vector<std::size_t> result;
        for (const auto& [pos, idx] : matches) {
            if (idx == pattern_index) {
                result.push_back(pos);
            }
        }
        return result;
    }
};

// ============================================
// KMP (Knuth-Morris-Pratt) Algorithm
// ============================================

/**
 * @class KMP
 * @brief Knuth-Morris-Pratt string matching algorithm
 * 
 * Time Complexity: O(n + m) where n = text length, m = pattern length
 * Space Complexity: O(m) for the LPS array
 * 
 * Advantages:
 * - Linear time complexity
 * - No backtracking in text
 * - Efficient for single pattern matching
 * 
 * Usage:
 * @code
 * // Simple search
 * auto result = KMP::search("hello world", "world");
 * if (result.found()) {
 *     std::cout << "Found at: " << result.first().value() << std::endl;
 * }
 * 
 * // Find all occurrences
 * auto result = KMP::search("abababab", "ab");
 * // positions: {0, 2, 4, 6}
 * 
 * // Reusable matcher for same pattern
 * KMP matcher("pattern");
 * auto r1 = matcher.match("text with pattern");
 * auto r2 = matcher.match("another pattern here");
 * @endcode
 */
class KMP {
public:
    /**
     * @brief Construct KMP matcher with a pattern
     * @param pattern The pattern to search for
     */
    explicit KMP(std::string_view pattern)
        : m_pattern(pattern), m_lps(compute_lps(pattern)) {}
    
    /**
     * @brief Search for pattern in text
     * @param text The text to search in
     * @return MatchResult containing all match positions
     */
    MatchResult match(std::string_view text) const {
        MatchResult result;
        
        if (m_pattern.empty()) {
            // Empty pattern matches at every position
            for (std::size_t i = 0; i <= text.length(); ++i) {
                result.positions.push_back(i);
            }
            return result;
        }
        
        if (text.length() < m_pattern.length()) {
            return result;
        }
        
        std::size_t i = 0;  // Index in text
        std::size_t j = 0;  // Index in pattern
        
        while (i < text.length()) {
            if (text[i] == m_pattern[j]) {
                ++i;
                ++j;
                
                if (j == m_pattern.length()) {
                    // Pattern found
                    result.positions.push_back(i - j);
                    j = m_lps[j - 1];  // Continue searching for more matches
                }
            } else {
                if (j != 0) {
                    j = m_lps[j - 1];
                } else {
                    ++i;
                }
            }
        }
        
        return result;
    }
    
    /**
     * @brief Check if pattern exists in text (stops at first match)
     * @param text The text to search in
     * @return true if pattern is found
     */
    bool contains(std::string_view text) const {
        if (m_pattern.empty()) return true;
        if (text.length() < m_pattern.length()) return false;
        
        std::size_t i = 0;
        std::size_t j = 0;
        
        while (i < text.length()) {
            if (text[i] == m_pattern[j]) {
                ++i;
                ++j;
                if (j == m_pattern.length()) {
                    return true;
                }
            } else {
                if (j != 0) {
                    j = m_lps[j - 1];
                } else {
                    ++i;
                }
            }
        }
        
        return false;
    }
    
    /**
     * @brief Get the first occurrence position
     */
    std::optional<std::size_t> find_first(std::string_view text) const {
        if (m_pattern.empty()) return 0;
        if (text.length() < m_pattern.length()) return std::nullopt;
        
        std::size_t i = 0;
        std::size_t j = 0;
        
        while (i < text.length()) {
            if (text[i] == m_pattern[j]) {
                ++i;
                ++j;
                if (j == m_pattern.length()) {
                    return i - j;
                }
            } else {
                if (j != 0) {
                    j = m_lps[j - 1];
                } else {
                    ++i;
                }
            }
        }
        
        return std::nullopt;
    }
    
    /**
     * @brief Get the pattern
     */
    const std::string& pattern() const {
        return m_pattern;
    }
    
    /**
     * @brief Get the LPS (Longest Proper Prefix Suffix) array
     */
    const std::vector<std::size_t>& lps() const {
        return m_lps;
    }
    
    // ============================================
    // Static convenience functions
    // ============================================
    
    /**
     * @brief Static search function for one-off searches
     */
    static MatchResult search(std::string_view text, std::string_view pattern) {
        KMP kmp(pattern);
        return kmp.match(text);
    }
    
    /**
     * @brief Static contains check
     */
    static bool contains(std::string_view text, std::string_view pattern) {
        KMP kmp(pattern);
        return kmp.contains(text);
    }
    
    /**
     * @brief Compute LPS array for a pattern (exposed for educational purposes)
     */
    static std::vector<std::size_t> compute_lps(std::string_view pattern) {
        std::size_t m = pattern.length();
        std::vector<std::size_t> lps(m, 0);
        
        if (m == 0) return lps;
        
        std::size_t len = 0;  // Length of previous longest prefix suffix
        std::size_t i = 1;
        
        while (i < m) {
            if (pattern[i] == pattern[len]) {
                ++len;
                lps[i] = len;
                ++i;
            } else {
                if (len != 0) {
                    len = lps[len - 1];
                } else {
                    lps[i] = 0;
                    ++i;
                }
            }
        }
        
        return lps;
    }
    
    /**
     * @brief Count occurrences of pattern in text
     */
    static std::size_t count(std::string_view text, std::string_view pattern) {
        return search(text, pattern).count();
    }
    
    /**
     * @brief Replace all occurrences of pattern with replacement
     */
    static std::string replace_all(std::string_view text, 
                                   std::string_view pattern,
                                   std::string_view replacement) {
        if (pattern.empty()) {
            return std::string(text);
        }
        
        auto matches = search(text, pattern);
        if (matches.positions.empty()) {
            return std::string(text);
        }
        
        std::string result;
        result.reserve(text.length());
        
        std::size_t last_pos = 0;
        for (std::size_t pos : matches.positions) {
            result.append(text.substr(last_pos, pos - last_pos));
            result.append(replacement);
            last_pos = pos + pattern.length();
        }
        result.append(text.substr(last_pos));
        
        return result;
    }
    
    /**
     * @brief Split string by pattern
     */
    static std::vector<std::string> split(std::string_view text, 
                                          std::string_view delimiter) {
        std::vector<std::string> result;
        
        if (delimiter.empty()) {
            result.emplace_back(text);
            return result;
        }
        
        auto matches = search(text, delimiter);
        
        std::size_t last_pos = 0;
        for (std::size_t pos : matches.positions) {
            result.emplace_back(text.substr(last_pos, pos - last_pos));
            last_pos = pos + delimiter.length();
        }
        result.emplace_back(text.substr(last_pos));
        
        return result;
    }

private:
    std::string m_pattern;
    std::vector<std::size_t> m_lps;
};

// ============================================
// Rabin-Karp Algorithm
// ============================================

/**
 * @class RabinKarp
 * @brief Rabin-Karp string matching algorithm using rolling hash
 * 
 * Time Complexity: O(n + m) average, O(nm) worst case
 * Space Complexity: O(1) for single pattern, O(k) for k patterns
 * 
 * Advantages:
 * - Efficient for multiple pattern matching
 * - Good average case performance
 * - Can be easily extended for 2D pattern matching
 * 
 * Usage:
 * @code
 * // Single pattern search
 * auto result = RabinKarp::search("hello world", "world");
 * 
 * // Multiple pattern search
 * std::vector<std::string> patterns = {"cat", "dog", "bird"};
 * auto result = RabinKarp::search_multiple("I have a cat and a dog", patterns);
 * 
 * // Reusable matcher
 * RabinKarp matcher("pattern");
 * auto r1 = matcher.match("text with pattern");
 * @endcode
 */
class RabinKarp {
public:
    // Use large prime for better hash distribution
    static constexpr std::size_t DEFAULT_BASE = 256;
    static constexpr std::size_t DEFAULT_MOD = 1000000007ULL;
    
    /**
     * @brief Construct Rabin-Karp matcher with a pattern
     */
    explicit RabinKarp(std::string_view pattern,
                       std::size_t base = DEFAULT_BASE,
                       std::size_t mod = DEFAULT_MOD)
        : m_pattern(pattern)
        , m_base(base)
        , m_mod(mod)
        , m_pattern_hash(compute_hash(pattern))
        , m_h(compute_h(pattern.length())) {}
    
    /**
     * @brief Search for pattern in text
     */
    MatchResult match(std::string_view text) const {
        MatchResult result;
        
        if (m_pattern.empty()) {
            for (std::size_t i = 0; i <= text.length(); ++i) {
                result.positions.push_back(i);
            }
            return result;
        }
        
        std::size_t m = m_pattern.length();
        std::size_t n = text.length();
        
        if (n < m) return result;
        
        // Compute hash of first window
        std::size_t text_hash = 0;
        for (std::size_t i = 0; i < m; ++i) {
            text_hash = (text_hash * m_base + static_cast<unsigned char>(text[i])) % m_mod;
        }
        
        // Slide the pattern over text
        for (std::size_t i = 0; i <= n - m; ++i) {
            if (text_hash == m_pattern_hash) {
                // Hash match, verify character by character
                if (verify_match(text, i)) {
                    result.positions.push_back(i);
                }
            }
            
            // Compute hash for next window
            if (i < n - m) {
                text_hash = rolling_hash(text_hash, 
                                        static_cast<unsigned char>(text[i]),
                                        static_cast<unsigned char>(text[i + m]));
            }
        }
        
        return result;
    }
    
    /**
     * @brief Check if pattern exists in text
     */
    bool contains(std::string_view text) const {
        if (m_pattern.empty()) return true;
        
        std::size_t m = m_pattern.length();
        std::size_t n = text.length();
        
        if (n < m) return false;
        
        std::size_t text_hash = 0;
        for (std::size_t i = 0; i < m; ++i) {
            text_hash = (text_hash * m_base + static_cast<unsigned char>(text[i])) % m_mod;
        }
        
        for (std::size_t i = 0; i <= n - m; ++i) {
            if (text_hash == m_pattern_hash && verify_match(text, i)) {
                return true;
            }
            
            if (i < n - m) {
                text_hash = rolling_hash(text_hash,
                                        static_cast<unsigned char>(text[i]),
                                        static_cast<unsigned char>(text[i + m]));
            }
        }
        
        return false;
    }
    
    /**
     * @brief Get the pattern
     */
    const std::string& pattern() const {
        return m_pattern;
    }
    
    /**
     * @brief Get the pattern hash
     */
    std::size_t pattern_hash() const {
        return m_pattern_hash;
    }
    
    // ============================================
    // Static convenience functions
    // ============================================
    
    /**
     * @brief Static search function
     */
    static MatchResult search(std::string_view text, std::string_view pattern) {
        RabinKarp rk(pattern);
        return rk.match(text);
    }
    
    /**
     * @brief Static contains check
     */
    static bool contains(std::string_view text, std::string_view pattern) {
        RabinKarp rk(pattern);
        return rk.contains(text);
    }
    
    /**
     * @brief Search for multiple patterns simultaneously
     * 
     * This is where Rabin-Karp shines - searching for multiple patterns
     * in a single pass through the text.
     */
    static MultiMatchResult search_multiple(
            std::string_view text,
            const std::vector<std::string>& patterns) {
        
        MultiMatchResult result;
        
        if (patterns.empty() || text.empty()) {
            return result;
        }
        
        // Group patterns by length for efficient processing
        std::size_t min_len = SIZE_MAX;
        std::size_t max_len = 0;
        
        for (const auto& p : patterns) {
            if (!p.empty()) {
                min_len = std::min(min_len, p.length());
                max_len = std::max(max_len, p.length());
            }
        }
        
        if (min_len == SIZE_MAX) {
            return result;
        }
        
        // For each unique pattern length, compute hashes
        std::unordered_map<std::size_t, std::vector<std::pair<std::size_t, std::size_t>>> 
            hash_to_patterns;  // hash -> [(pattern_index, pattern_length)]
        
        for (std::size_t i = 0; i < patterns.size(); ++i) {
            if (patterns[i].empty()) continue;
            std::size_t hash = compute_hash_static(patterns[i]);
            hash_to_patterns[hash].emplace_back(i, patterns[i].length());
        }
        
        // Process text for each pattern length
        std::unordered_set<std::size_t> processed_lengths;
        
        for (const auto& p : patterns) {
            std::size_t len = p.length();
            if (len == 0 || len > text.length() || processed_lengths.count(len)) {
                continue;
            }
            processed_lengths.insert(len);
            
            std::size_t h = compute_h_static(len);
            std::size_t text_hash = 0;
            
            // Initial hash
            for (std::size_t i = 0; i < len; ++i) {
                text_hash = (text_hash * DEFAULT_BASE + 
                            static_cast<unsigned char>(text[i])) % DEFAULT_MOD;
            }
            
            // Slide through text
            for (std::size_t i = 0; i <= text.length() - len; ++i) {
                auto it = hash_to_patterns.find(text_hash);
                if (it != hash_to_patterns.end()) {
                    // Check all patterns with this hash and length
                    for (const auto& [pat_idx, pat_len] : it->second) {
                        if (pat_len == len) {
                            // Verify match
                            bool match = true;
                            for (std::size_t j = 0; j < len && match; ++j) {
                                if (text[i + j] != patterns[pat_idx][j]) {
                                    match = false;
                                }
                            }
                            if (match) {
                                result.matches.emplace_back(i, pat_idx);
                            }
                        }
                    }
                }
                
                // Rolling hash
                if (i < text.length() - len) {
                    text_hash = ((text_hash + DEFAULT_MOD - 
                                 (static_cast<unsigned char>(text[i]) * h) % DEFAULT_MOD) 
                                * DEFAULT_BASE + 
                                static_cast<unsigned char>(text[i + len])) % DEFAULT_MOD;
                }
            }
        }
        
        // Sort results by position
        std::sort(result.matches.begin(), result.matches.end());
        
        return result;
    }
    
    /**
     * @brief Compute hash of a string (exposed for educational purposes)
     */
    static std::size_t compute_hash_static(std::string_view str,
                                           std::size_t base = DEFAULT_BASE,
                                           std::size_t mod = DEFAULT_MOD) {
        std::size_t hash = 0;
        for (char c : str) {
            hash = (hash * base + static_cast<unsigned char>(c)) % mod;
        }
        return hash;
    }
    
    /**
     * @brief Find all substrings of given length that appear more than once
     */
    static std::vector<std::string> find_repeated_substrings(
            std::string_view text, std::size_t length) {
        
        std::vector<std::string> result;
        
        if (length == 0 || length > text.length()) {
            return result;
        }
        
        std::unordered_map<std::size_t, std::vector<std::size_t>> hash_positions;
        
        std::size_t h = compute_h_static(length);
        std::size_t text_hash = 0;
        
        // Initial hash
        for (std::size_t i = 0; i < length; ++i) {
            text_hash = (text_hash * DEFAULT_BASE + 
                        static_cast<unsigned char>(text[i])) % DEFAULT_MOD;
        }
        hash_positions[text_hash].push_back(0);
        
        // Rolling hash
        for (std::size_t i = 1; i <= text.length() - length; ++i) {
            text_hash = ((text_hash + DEFAULT_MOD - 
                         (static_cast<unsigned char>(text[i - 1]) * h) % DEFAULT_MOD) 
                        * DEFAULT_BASE + 
                        static_cast<unsigned char>(text[i + length - 1])) % DEFAULT_MOD;
            hash_positions[text_hash].push_back(i);
        }
        
        // Find repeated substrings
        std::unordered_set<std::string> seen;
        for (const auto& [hash, positions] : hash_positions) {
            if (positions.size() > 1) {
                // Verify and add unique substrings
                for (std::size_t pos : positions) {
                    std::string sub(text.substr(pos, length));
                    if (seen.find(sub) == seen.end()) {
                        // Verify it actually appears multiple times
                        std::size_t count = 0;
                        for (std::size_t p : positions) {
                            if (text.substr(p, length) == sub) {
                                ++count;
                            }
                        }
                        if (count > 1) {
                            result.push_back(sub);
                            seen.insert(sub);
                        }
                    }
                }
            }
        }
        
        return result;
    }

private:
    std::string m_pattern;
    std::size_t m_base;
    std::size_t m_mod;
    std::size_t m_pattern_hash;
    std::size_t m_h;  // base^(m-1) % mod
    
    /**
     * @brief Compute hash of a string
     */
    std::size_t compute_hash(std::string_view str) const {
        std::size_t hash = 0;
        for (char c : str) {
            hash = (hash * m_base + static_cast<unsigned char>(c)) % m_mod;
        }
        return hash;
    }
    
    /**
     * @brief Compute h = base^(len-1) % mod
     */
    std::size_t compute_h(std::size_t len) const {
        if (len == 0) return 1;
        std::size_t h = 1;
        for (std::size_t i = 0; i < len - 1; ++i) {
            h = (h * m_base) % m_mod;
        }
        return h;
    }
    
    static std::size_t compute_h_static(std::size_t len,
                                        std::size_t base = DEFAULT_BASE,
                                        std::size_t mod = DEFAULT_MOD) {
        if (len == 0) return 1;
        std::size_t h = 1;
        for (std::size_t i = 0; i < len - 1; ++i) {
            h = (h * base) % mod;
        }
        return h;
    }
    
    /**
     * @brief Compute rolling hash for next window
     */
    std::size_t rolling_hash(std::size_t old_hash, 
                             unsigned char old_char, 
                             unsigned char new_char) const {
        std::size_t new_hash = old_hash;
        
        // Remove old character contribution
        new_hash = (new_hash + m_mod - (old_char * m_h) % m_mod) % m_mod;
        
        // Shift and add new character
        new_hash = (new_hash * m_base + new_char) % m_mod;
        
        return new_hash;
    }
    
    /**
     * @brief Verify character-by-character match
     */
    bool verify_match(std::string_view text, std::size_t pos) const {
        for (std::size_t i = 0; i < m_pattern.length(); ++i) {
            if (text[pos + i] != m_pattern[i]) {
                return false;
            }
        }
        return true;
    }
};

// ============================================
// String Utility Functions
// ============================================

/**
 * @brief Check if string starts with prefix
 */
inline bool starts_with(std::string_view text, std::string_view prefix) {
    if (prefix.length() > text.length()) return false;
    return text.substr(0, prefix.length()) == prefix;
}

/**
 * @brief Check if string ends with suffix
 */
inline bool ends_with(std::string_view text, std::string_view suffix) {
    if (suffix.length() > text.length()) return false;
    return text.substr(text.length() - suffix.length()) == suffix;
}

/**
 * @brief Compute longest common prefix of two strings
 */
inline std::string longest_common_prefix(std::string_view s1, std::string_view s2) {
    std::size_t len = std::min(s1.length(), s2.length());
    std::size_t i = 0;
    while (i < len && s1[i] == s2[i]) {
        ++i;
    }
    return std::string(s1.substr(0, i));
}

/**
 * @brief Compute longest common suffix of two strings
 */
inline std::string longest_common_suffix(std::string_view s1, std::string_view s2) {
    std::size_t len = std::min(s1.length(), s2.length());
    std::size_t i = 0;
    while (i < len && s1[s1.length() - 1 - i] == s2[s2.length() - 1 - i]) {
        ++i;
    }
    return std::string(s1.substr(s1.length() - i));
}

/**
 * @brief Check if string is palindrome
 */
inline bool is_palindrome(std::string_view str) {
    if (str.empty()) return true;
    std::size_t left = 0;
    std::size_t right = str.length() - 1;
    while (left < right) {
        if (str[left] != str[right]) return false;
        ++left;
        --right;
    }
    return true;
}

/**
 * @brief Find longest palindromic substring
 */
inline std::string longest_palindrome(std::string_view str) {
    if (str.empty()) return "";
    
    std::size_t start = 0;
    std::size_t max_len = 1;
    
    auto expand_around_center = [&str](std::size_t left, std::size_t right) 
            -> std::pair<std::size_t, std::size_t> {
        while (left > 0 && right < str.length() - 1 && 
               str[left - 1] == str[right + 1]) {
            --left;
            ++right;
        }
        return {left, right - left + 1};
    };
    
    for (std::size_t i = 0; i < str.length(); ++i) {
        // Odd length palindrome
        auto [s1, len1] = expand_around_center(i, i);
        if (len1 > max_len) {
            start = s1;
            max_len = len1;
        }
        
        // Even length palindrome
        if (i + 1 < str.length() && str[i] == str[i + 1]) {
            auto [s2, len2] = expand_around_center(i, i + 1);
            if (len2 > max_len) {
                start = s2;
                max_len = len2;
            }
        }
    }
    
    return std::string(str.substr(start, max_len));
}

/**
 * @brief Compute edit distance (Levenshtein distance) between two strings
 */
inline std::size_t edit_distance(std::string_view s1, std::string_view s2) {
    std::size_t m = s1.length();
    std::size_t n = s2.length();
    
    // Use two rows for space optimization
    std::vector<std::size_t> prev(n + 1), curr(n + 1);
    
    // Initialize first row
    for (std::size_t j = 0; j <= n; ++j) {
        prev[j] = j;
    }
    
    for (std::size_t i = 1; i <= m; ++i) {
        curr[0] = i;
        for (std::size_t j = 1; j <= n; ++j) {
            if (s1[i - 1] == s2[j - 1]) {
                curr[j] = prev[j - 1];
            } else {
                curr[j] = 1 + std::min({prev[j], curr[j - 1], prev[j - 1]});
            }
        }
        std::swap(prev, curr);
    }
    
    return prev[n];
}

/**
 * @brief Compute longest common subsequence length
 */
inline std::size_t lcs_length(std::string_view s1, std::string_view s2) {
    std::size_t m = s1.length();
    std::size_t n = s2.length();
    
    std::vector<std::size_t> prev(n + 1, 0), curr(n + 1, 0);
    
    for (std::size_t i = 1; i <= m; ++i) {
        for (std::size_t j = 1; j <= n; ++j) {
            if (s1[i - 1] == s2[j - 1]) {
                curr[j] = prev[j - 1] + 1;
            } else {
                curr[j] = std::max(prev[j], curr[j - 1]);
            }
        }
        std::swap(prev, curr);
    }
    
    return prev[n];
}

/**
 * @brief Get longest common subsequence string
 */
inline std::string lcs(std::string_view s1, std::string_view s2) {
    std::size_t m = s1.length();
    std::size_t n = s2.length();
    
    std::vector<std::vector<std::size_t>> dp(m + 1, std::vector<std::size_t>(n + 1, 0));
    
    for (std::size_t i = 1; i <= m; ++i) {
        for (std::size_t j = 1; j <= n; ++j) {
            if (s1[i - 1] == s2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else {
                dp[i][j] = std::max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }
    
    // Backtrack to find LCS
    std::string result;
    std::size_t i = m, j = n;
    while (i > 0 && j > 0) {
        if (s1[i - 1] == s2[j - 1]) {
            result = s1[i - 1] + result;
            --i;
            --j;
        } else if (dp[i - 1][j] > dp[i][j - 1]) {
            --i;
        } else {
            --j;
        }
    }
    
    return result;
}

// ============================================
// Convenience free functions
// ============================================

/**
 * @brief Find pattern in text using KMP
 */
inline MatchResult kmp_search(std::string_view text, std::string_view pattern) {
    return KMP::search(text, pattern);
}

/**
 * @brief Find pattern in text using Rabin-Karp
 */
inline MatchResult rabin_karp_search(std::string_view text, std::string_view pattern) {
    return RabinKarp::search(text, pattern);
}

/**
 * @brief Find multiple patterns in text
 */
inline MultiMatchResult search_patterns(std::string_view text,
                                        const std::vector<std::string>& patterns) {
    return RabinKarp::search_multiple(text, patterns);
}

} // namespace algorithm
} // namespace mylib

#endif // MYLIB_ALGORITHM_STRING_ALGORITHMS_HPP