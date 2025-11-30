/**
 * @file trie.hpp
 * @brief Trie (Prefix Tree) data structure implementation
 * @author Jinhyeok
 * @date 2025-11-30
 * @version 1.0.0
 * 
 * This file provides a comprehensive Trie implementation with:
 * - Basic operations: insert, search, remove
 * - Prefix operations: starts_with, get_all_with_prefix
 * - Autocomplete functionality
 * - Pattern matching with wildcards
 * - Word counting and frequency tracking
 * 
 * Time Complexity:
 * - insert: O(m) where m = word length
 * - search: O(m)
 * - remove: O(m)
 * - starts_with: O(m)
 * - get_all_with_prefix: O(m + k) where k = total characters in results
 * 
 * Space Complexity: O(ALPHABET_SIZE * m * n) where n = number of words
 * 
 * Copyright (c) 2025 Jinhyeok
 * MIT License
 */

#ifndef MYLIB_TREE_TRIE_HPP
#define MYLIB_TREE_TRIE_HPP

#include <cstddef>
#include <string>
#include <string_view>
#include <vector>
#include <memory>
#include <unordered_map>
#include <functional>
#include <optional>
#include <queue>
#include <algorithm>

namespace mylib {
namespace tree {

/**
 * @class Trie
 * @brief A trie (prefix tree) for efficient string storage and retrieval
 * 
 * The Trie supports:
 * - Standard dictionary operations (insert, search, delete)
 * - Prefix-based queries (autocomplete, starts_with)
 * - Wildcard pattern matching
 * - Word frequency counting
 * 
 * @tparam CharT Character type (default: char)
 * 
 * Usage:
 * @code
 * Trie trie;
 * 
 * // Insert words
 * trie.insert("apple");
 * trie.insert("application");
 * trie.insert("apply");
 * 
 * // Search
 * bool found = trie.search("apple");      // true
 * bool prefix = trie.starts_with("app");  // true
 * 
 * // Autocomplete
 * auto suggestions = trie.autocomplete("app", 5);
 * // {"apple", "application", "apply"}
 * 
 * // Remove
 * trie.remove("apple");
 * @endcode
 */
template <typename CharT = char>
class Trie {
private:
    /**
     * @struct TrieNode
     * @brief Internal node structure for the trie
     */
    struct TrieNode {
        std::unordered_map<CharT, std::unique_ptr<TrieNode>> children;
        bool is_end_of_word = false;
        std::size_t word_count = 0;      ///< Number of times this word was inserted
        std::size_t prefix_count = 0;    ///< Number of words with this prefix
        
        TrieNode() = default;
    };
    
public:
    using string_type = std::basic_string<CharT>;
    using string_view_type = std::basic_string_view<CharT>;
    
    /**
     * @brief Default constructor
     */
    Trie() : m_root(std::make_unique<TrieNode>()), m_size(0) {}
    
    /**
     * @brief Construct with initializer list of words
     */
    Trie(std::initializer_list<string_type> words) : Trie() {
        for (const auto& word : words) {
            insert(word);
        }
    }
    
    /**
     * @brief Construct from iterator range
     */
    template <typename InputIt>
    Trie(InputIt first, InputIt last) : Trie() {
        for (auto it = first; it != last; ++it) {
            insert(*it);
        }
    }
    
    /**
     * @brief Copy constructor
     */
    Trie(const Trie& other) : Trie() {
        // Deep copy by inserting all words
        std::vector<string_type> words;
        other.get_all_words(words);
        for (const auto& word : words) {
            // Preserve word counts
            std::size_t count = other.word_count(word);
            for (std::size_t i = 0; i < count; ++i) {
                insert(word);
            }
        }
    }
    
    /**
     * @brief Move constructor
     */
    Trie(Trie&& other) noexcept = default;
    
    /**
     * @brief Copy assignment
     */
    Trie& operator=(const Trie& other) {
        if (this != &other) {
            Trie temp(other);
            std::swap(m_root, temp.m_root);
            std::swap(m_size, temp.m_size);
        }
        return *this;
    }
    
    /**
     * @brief Move assignment
     */
    Trie& operator=(Trie&& other) noexcept = default;
    
    /**
     * @brief Destructor
     */
    ~Trie() = default;
    
    // ============================================
    // Basic Operations
    // ============================================
    
    /**
     * @brief Insert a word into the trie
     * @param word The word to insert
     * 
     * Time Complexity: O(m) where m = word length
     */
    void insert(string_view_type word) {
        TrieNode* node = m_root.get();
        
        for (CharT ch : word) {
            node->prefix_count++;
            
            if (node->children.find(ch) == node->children.end()) {
                node->children[ch] = std::make_unique<TrieNode>();
            }
            node = node->children[ch].get();
        }
        
        node->prefix_count++;
        
        if (!node->is_end_of_word) {
            m_size++;
        }
        
        node->is_end_of_word = true;
        node->word_count++;
    }
    
    /**
     * @brief Search for an exact word in the trie
     * @param word The word to search for
     * @return true if the word exists
     * 
     * Time Complexity: O(m)
     */
    bool search(string_view_type word) const {
        const TrieNode* node = find_node(word);
        return node != nullptr && node->is_end_of_word;
    }
    
    /**
     * @brief Check if the trie contains a word (alias for search)
     */
    bool contains(string_view_type word) const {
        return search(word);
    }
    
    /**
     * @brief Remove a word from the trie
     * @param word The word to remove
     * @return true if the word was removed, false if not found
     * 
     * Time Complexity: O(m)
     */
    bool remove(string_view_type word) {
        // First check if word exists
        TrieNode* node = find_node_mutable(word);
        if (node == nullptr || !node->is_end_of_word) {
            return false;
        }
        
        // Word exists, now remove it
        remove_helper(m_root.get(), word, 0);
        return true;
    }
    
    /**
     * @brief Alias for remove
     */
    bool erase(string_view_type word) {
        return remove(word);
    }
    
    // ============================================
    // Prefix Operations
    // ============================================
    
    /**
     * @brief Check if any word starts with the given prefix
     * @param prefix The prefix to check
     * @return true if any word has this prefix
     * 
     * Time Complexity: O(m) where m = prefix length
     */
    bool starts_with(string_view_type prefix) const {
        return find_node(prefix) != nullptr;
    }
    
    /**
     * @brief Get all words that start with the given prefix
     * @param prefix The prefix to search for
     * @return Vector of words with the given prefix
     * 
     * Time Complexity: O(m + k) where k = total chars in results
     */
    std::vector<string_type> get_all_with_prefix(string_view_type prefix) const {
        std::vector<string_type> result;
        const TrieNode* node = find_node(prefix);
        
        if (node != nullptr) {
            string_type current_prefix(prefix);
            collect_words(node, current_prefix, result);
        }
        
        return result;
    }
    
    /**
     * @brief Get autocomplete suggestions for a prefix
     * @param prefix The prefix to complete
     * @param max_suggestions Maximum number of suggestions (0 = unlimited)
     * @return Vector of suggested words
     * 
     * Time Complexity: O(m + k) limited by max_suggestions
     */
    std::vector<string_type> autocomplete(string_view_type prefix, 
                                          std::size_t max_suggestions = 0) const {
        std::vector<string_type> result;
        const TrieNode* node = find_node(prefix);
        
        if (node != nullptr) {
            string_type current_prefix(prefix);
            collect_words_limited(node, current_prefix, result, max_suggestions);
        }
        
        return result;
    }
    
    /**
     * @brief Count words that start with the given prefix
     * @param prefix The prefix to count
     * @return Number of words with this prefix
     * 
     * Time Complexity: O(m)
     */
    std::size_t count_with_prefix(string_view_type prefix) const {
        const TrieNode* node = find_node(prefix);
        if (node == nullptr) return 0;
        
        // Count words in subtree
        std::size_t count = 0;
        count_words_in_subtree(node, count);
        return count;
    }
    
    // ============================================
    // Word Count Operations
    // ============================================
    
    /**
     * @brief Get the number of times a word was inserted
     * @param word The word to check
     * @return Number of times the word appears (0 if not found)
     */
    std::size_t word_count(string_view_type word) const {
        const TrieNode* node = find_node(word);
        if (node == nullptr || !node->is_end_of_word) return 0;
        return node->word_count;
    }
    
    /**
     * @brief Get the most frequent words
     * @param k Number of words to return
     * @return Vector of (word, count) pairs, sorted by frequency
     */
    std::vector<std::pair<string_type, std::size_t>> most_frequent(std::size_t k) const {
        std::vector<std::pair<string_type, std::size_t>> all_words;
        string_type current;
        collect_words_with_count(m_root.get(), current, all_words);
        
        // Sort by count (descending), then by word (ascending)
        std::sort(all_words.begin(), all_words.end(),
            [](const auto& a, const auto& b) {
                if (a.second != b.second) return a.second > b.second;
                return a.first < b.first;
            });
        
        if (k > 0 && all_words.size() > k) {
            all_words.resize(k);
        }
        
        return all_words;
    }
    
    // ============================================
    // Pattern Matching
    // ============================================
    
    /**
     * @brief Search with wildcard pattern
     * @param pattern Pattern where '.' matches any single character
     * @return Vector of matching words
     * 
     * Example: search_pattern("h.llo") matches "hello", "hallo"
     */
    std::vector<string_type> search_pattern(string_view_type pattern) const {
        std::vector<string_type> result;
        string_type current;
        search_pattern_helper(m_root.get(), pattern, 0, current, result);
        return result;
    }
    
    /**
     * @brief Check if any word matches the pattern
     * @param pattern Pattern where '.' matches any single character
     * @return true if any word matches
     */
    bool matches_pattern(string_view_type pattern) const {
        return matches_pattern_helper(m_root.get(), pattern, 0);
    }
    
    // ============================================
    // Utility Operations
    // ============================================
    
    /**
     * @brief Get all words in the trie
     * @return Vector of all words
     */
    std::vector<string_type> get_all_words() const {
        std::vector<string_type> result;
        string_type current;
        collect_words(m_root.get(), current, result);
        return result;
    }
    
    /**
     * @brief Get all words (output parameter version)
     */
    void get_all_words(std::vector<string_type>& words) const {
        string_type current;
        collect_words(m_root.get(), current, words);
    }
    
    /**
     * @brief Get the number of unique words in the trie
     */
    std::size_t size() const {
        return m_size;
    }
    
    /**
     * @brief Check if the trie is empty
     */
    bool empty() const {
        return m_size == 0;
    }
    
    /**
     * @brief Remove all words from the trie
     */
    void clear() {
        m_root = std::make_unique<TrieNode>();
        m_size = 0;
    }
    
    /**
     * @brief Get the longest common prefix of all words
     * @return The longest common prefix
     */
    string_type longest_common_prefix() const {
        string_type result;
        const TrieNode* node = m_root.get();
        
        while (node != nullptr) {
            // If node has multiple children or is end of word, stop
            if (node->children.size() != 1 || 
                (node->is_end_of_word && node != m_root.get())) {
                break;
            }
            
            auto it = node->children.begin();
            result += it->first;
            node = it->second.get();
        }
        
        return result;
    }
    
    /**
     * @brief Find the longest word in the trie
     */
    string_type longest_word() const {
        string_type result;
        string_type current;
        find_longest(m_root.get(), current, result);
        return result;
    }
    
    /**
     * @brief Find the shortest word in the trie
     */
    string_type shortest_word() const {
        if (empty()) return string_type();
        
        // BFS to find shortest word
        std::queue<std::pair<const TrieNode*, string_type>> q;
        q.push({m_root.get(), string_type()});
        
        while (!q.empty()) {
            auto [node, word] = q.front();
            q.pop();
            
            if (node->is_end_of_word && node != m_root.get()) {
                return word;
            }
            
            for (const auto& [ch, child] : node->children) {
                q.push({child.get(), word + ch});
            }
        }
        
        return string_type();
    }
    
    /**
     * @brief Traverse all words and apply a function
     * @param func Function to apply to each word
     */
    template <typename Func>
    void for_each(Func func) const {
        string_type current;
        for_each_helper(m_root.get(), current, func);
    }
    
    /**
     * @brief Get words within edit distance k from the given word
     * @param word The target word
     * @param max_distance Maximum edit distance
     * @return Vector of (word, distance) pairs
     */
    std::vector<std::pair<string_type, std::size_t>> 
    fuzzy_search(string_view_type word, std::size_t max_distance) const {
        std::vector<std::pair<string_type, std::size_t>> result;
        
        // Initialize first row of DP table
        std::vector<std::size_t> current_row(word.length() + 1);
        for (std::size_t i = 0; i <= word.length(); ++i) {
            current_row[i] = i;
        }
        
        string_type current_word;
        fuzzy_search_helper(m_root.get(), word, current_word, 
                           current_row, max_distance, result);
        
        // Sort by distance
        std::sort(result.begin(), result.end(),
            [](const auto& a, const auto& b) {
                return a.second < b.second;
            });
        
        return result;
    }

private:
    std::unique_ptr<TrieNode> m_root;
    std::size_t m_size;
    
    /**
     * @brief Find the node corresponding to a prefix
     */
    const TrieNode* find_node(string_view_type prefix) const {
        const TrieNode* node = m_root.get();
        
        for (CharT ch : prefix) {
            auto it = node->children.find(ch);
            if (it == node->children.end()) {
                return nullptr;
            }
            node = it->second.get();
        }
        
        return node;
    }
    
    /**
     * @brief Find node (non-const version)
     */
    TrieNode* find_node_mutable(string_view_type prefix) {
        TrieNode* node = m_root.get();
        
        for (CharT ch : prefix) {
            auto it = node->children.find(ch);
            if (it == node->children.end()) {
                return nullptr;
            }
            node = it->second.get();
        }
        
        return node;
    }
    
    /**
     * @brief Recursively collect all words from a node
     */
    void collect_words(const TrieNode* node, string_type& current,
                       std::vector<string_type>& result) const {
        if (node->is_end_of_word) {
            result.push_back(current);
        }
        
        for (const auto& [ch, child] : node->children) {
            current.push_back(ch);
            collect_words(child.get(), current, result);
            current.pop_back();
        }
    }
    
    /**
     * @brief Collect words with a limit
     */
    void collect_words_limited(const TrieNode* node, string_type& current,
                               std::vector<string_type>& result,
                               std::size_t max_count) const {
        if (max_count > 0 && result.size() >= max_count) {
            return;
        }
        
        if (node->is_end_of_word) {
            result.push_back(current);
            if (max_count > 0 && result.size() >= max_count) {
                return;
            }
        }
        
        for (const auto& [ch, child] : node->children) {
            current.push_back(ch);
            collect_words_limited(child.get(), current, result, max_count);
            current.pop_back();
            
            if (max_count > 0 && result.size() >= max_count) {
                return;
            }
        }
    }
    
    /**
     * @brief Collect words with their counts
     */
    void collect_words_with_count(const TrieNode* node, string_type& current,
                                  std::vector<std::pair<string_type, std::size_t>>& result) const {
        if (node->is_end_of_word) {
            result.emplace_back(current, node->word_count);
        }
        
        for (const auto& [ch, child] : node->children) {
            current.push_back(ch);
            collect_words_with_count(child.get(), current, result);
            current.pop_back();
        }
    }
    
    /**
     * @brief Count words in subtree
     */
    void count_words_in_subtree(const TrieNode* node, std::size_t& count) const {
        if (node->is_end_of_word) {
            count++;
        }
        
        for (const auto& [ch, child] : node->children) {
            count_words_in_subtree(child.get(), count);
        }
    }
    
    /**
     * @brief Helper for remove operation
     * @return true if the current node should be deleted
     */
    bool remove_helper(TrieNode* node, string_view_type word, std::size_t depth) {
        if (depth == word.length()) {
            // We've reached the end of the word
            node->word_count--;
            node->prefix_count--;
            
            if (node->word_count == 0) {
                node->is_end_of_word = false;
                m_size--;
            }
            
            return node->children.empty() && !node->is_end_of_word;
        }
        
        CharT ch = word[depth];
        auto it = node->children.find(ch);
        
        bool should_delete_child = remove_helper(it->second.get(), word, depth + 1);
        
        if (should_delete_child) {
            node->children.erase(ch);
        }
        
        node->prefix_count--;
        
        return node->children.empty() && !node->is_end_of_word;
    }
    
    /**
     * @brief Helper for pattern matching
     */
    void search_pattern_helper(const TrieNode* node, string_view_type pattern,
                               std::size_t index, string_type& current,
                               std::vector<string_type>& result) const {
        if (index == pattern.length()) {
            if (node->is_end_of_word) {
                result.push_back(current);
            }
            return;
        }
        
        CharT ch = pattern[index];
        
        if (ch == '.') {
            // Wildcard: try all children
            for (const auto& [child_ch, child] : node->children) {
                current.push_back(child_ch);
                search_pattern_helper(child.get(), pattern, index + 1, current, result);
                current.pop_back();
            }
        } else {
            // Regular character
            auto it = node->children.find(ch);
            if (it != node->children.end()) {
                current.push_back(ch);
                search_pattern_helper(it->second.get(), pattern, index + 1, current, result);
                current.pop_back();
            }
        }
    }
    
    /**
     * @brief Helper for pattern matching (boolean version)
     */
    bool matches_pattern_helper(const TrieNode* node, string_view_type pattern,
                                std::size_t index) const {
        if (index == pattern.length()) {
            return node->is_end_of_word;
        }
        
        CharT ch = pattern[index];
        
        if (ch == '.') {
            for (const auto& [child_ch, child] : node->children) {
                if (matches_pattern_helper(child.get(), pattern, index + 1)) {
                    return true;
                }
            }
            return false;
        } else {
            auto it = node->children.find(ch);
            if (it == node->children.end()) {
                return false;
            }
            return matches_pattern_helper(it->second.get(), pattern, index + 1);
        }
    }
    
    /**
     * @brief Find longest word helper
     */
    void find_longest(const TrieNode* node, string_type& current,
                      string_type& longest) const {
        if (node->is_end_of_word && current.length() > longest.length()) {
            longest = current;
        }
        
        for (const auto& [ch, child] : node->children) {
            current.push_back(ch);
            find_longest(child.get(), current, longest);
            current.pop_back();
        }
    }
    
    /**
     * @brief Helper for for_each
     */
    template <typename Func>
    void for_each_helper(const TrieNode* node, string_type& current, Func& func) const {
        if (node->is_end_of_word) {
            func(current);
        }
        
        for (const auto& [ch, child] : node->children) {
            current.push_back(ch);
            for_each_helper(child.get(), current, func);
            current.pop_back();
        }
    }
    
    /**
     * @brief Helper for fuzzy search using Levenshtein distance
     */
    void fuzzy_search_helper(const TrieNode* node, string_view_type word,
                             string_type& current_word,
                             const std::vector<std::size_t>& previous_row,
                             std::size_t max_distance,
                             std::vector<std::pair<string_type, std::size_t>>& result) const {
        std::size_t columns = word.length() + 1;
        
        if (node->is_end_of_word) {
            std::size_t distance = previous_row[columns - 1];
            if (distance <= max_distance) {
                result.emplace_back(current_word, distance);
            }
        }
        
        for (const auto& [ch, child] : node->children) {
            std::vector<std::size_t> current_row(columns);
            current_row[0] = previous_row[0] + 1;
            
            for (std::size_t i = 1; i < columns; ++i) {
                std::size_t insert_cost = current_row[i - 1] + 1;
                std::size_t delete_cost = previous_row[i] + 1;
                std::size_t replace_cost = previous_row[i - 1];
                if (word[i - 1] != ch) {
                    replace_cost++;
                }
                
                current_row[i] = std::min({insert_cost, delete_cost, replace_cost});
            }
            
            // Prune: if minimum in row exceeds max_distance, skip subtree
            std::size_t min_in_row = *std::min_element(current_row.begin(), current_row.end());
            if (min_in_row <= max_distance) {
                current_word.push_back(ch);
                fuzzy_search_helper(child.get(), word, current_word,
                                   current_row, max_distance, result);
                current_word.pop_back();
            }
        }
    }
};

// Type aliases for convenience
using StringTrie = Trie<char>;
using WStringTrie = Trie<wchar_t>;

} // namespace tree
} // namespace mylib

#endif // MYLIB_TREE_TRIE_HPP