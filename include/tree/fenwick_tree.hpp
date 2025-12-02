/**
 * @file fenwick_tree.hpp
 * @brief Fenwick Tree (Binary Indexed Tree) data structure implementation
 * @author Jinhyeok
 * @date 2025-12-02
 * @version 1.0.0
 */

#ifndef MYLIB_TREE_FENWICK_TREE_HPP
#define MYLIB_TREE_FENWICK_TREE_HPP

#include <cstddef>
#include <vector>
#include <stdexcept>
#include <algorithm>

namespace mylib {
namespace tree {

template <typename T>
class FenwickTree {
private:
    std::vector<T> m_tree;
    std::size_t m_size;
    
public:
    explicit FenwickTree(const std::vector<T>& arr)
        : m_tree(arr.size() + 1, T()), m_size(arr.size()) {
        for (std::size_t i = 0; i < arr.size(); ++i) {
            update(i, arr[i]);
        }
    }
    
    explicit FenwickTree(std::size_t size, const T& default_value = T())
        : m_tree(size + 1, T()), m_size(size) {
        if (default_value != T()) {
            for (std::size_t i = 0; i < size; ++i) {
                update(i, default_value);
            }
        }
    }
    
    T prefix_sum(std::size_t index) const {
        if (index >= m_size) {
            throw std::out_of_range("Index out of range");
        }
        
        T sum = T();
        ++index;
        
        while (index > 0) {
            sum += m_tree[index];
            index -= lsb(index);
        }
        
        return sum;
    }
    
    T range_sum(std::size_t left, std::size_t right) const {
        if (left > right || right >= m_size) {
            throw std::out_of_range("Invalid range");
        }
        
        if (left == 0) {
            return prefix_sum(right);
        }
        
        return prefix_sum(right) - prefix_sum(left - 1);
    }
    
    T get(std::size_t index) const {
        if (index >= m_size) {
            throw std::out_of_range("Index out of range");
        }
        
        return range_sum(index, index);
    }
    
    T sum_all() const {
        return prefix_sum(m_size - 1);
    }
    
    void update(std::size_t index, const T& delta) {
        if (index >= m_size) {
            throw std::out_of_range("Index out of range");
        }
        
        ++index;
        
        while (index <= m_size) {
            m_tree[index] += delta;
            index += lsb(index);
        }
    }
    
    void set(std::size_t index, const T& value) {
        T current = get(index);
        T delta = value - current;
        update(index, delta);
    }
    
    std::size_t size() const noexcept {
        return m_size;
    }
    
    bool empty() const noexcept {
        return m_size == 0;
    }
    
    std::vector<T> to_vector() const {
        std::vector<T> result(m_size);
        for (std::size_t i = 0; i < m_size; ++i) {
            result[i] = get(i);
        }
        return result;
    }
    
    std::size_t lower_bound(const T& target) const {
        T sum = T();
        std::size_t pos = 0;
        
        std::size_t bit = 1;
        while (bit <= m_size) bit <<= 1;
        bit >>= 1;
        
        while (bit > 0) {
            if (pos + bit <= m_size && sum + m_tree[pos + bit] < target) {
                sum += m_tree[pos + bit];
                pos += bit;
            }
            bit >>= 1;
        }
        
        return pos;
    }
    
private:
    static std::size_t lsb(std::size_t x) {
        return x & (-x);
    }
};

template <typename T>
class RangeUpdateFenwickTree {
private:
    FenwickTree<T> m_tree1;
    FenwickTree<T> m_tree2;
    std::size_t m_size;
    
public:
    explicit RangeUpdateFenwickTree(const std::vector<T>& arr)
        : m_tree1(arr.size()), m_tree2(arr.size()), m_size(arr.size()) {
        for (std::size_t i = 0; i < arr.size(); ++i) {
            update(i, arr[i]);
        }
    }
    
    explicit RangeUpdateFenwickTree(std::size_t size, const T& default_value = T())
        : m_tree1(size), m_tree2(size), m_size(size) {
        if (default_value != T()) {
            for (std::size_t i = 0; i < size; ++i) {
                update(i, default_value);
            }
        }
    }
    
    void update(std::size_t index, const T& delta) {
        range_update(index, index, delta);
    }
    
    void range_update(std::size_t left, std::size_t right, const T& delta) {
        if (left > right || right >= m_size) {
            throw std::out_of_range("Invalid range");
        }
        
        m_tree1.update(left, delta);
        if (right + 1 < m_size) {
            m_tree1.update(right + 1, -delta);
        }
        
        if (left > 0) {
            m_tree2.update(left, delta * static_cast<T>(left));
        }
        if (right + 1 < m_size) {
            m_tree2.update(right + 1, -delta * static_cast<T>(right + 1));
        }
    }
    
    T get(std::size_t index) const {
        if (index >= m_size) {
            throw std::out_of_range("Index out of range");
        }
        return m_tree1.prefix_sum(index);
    }
    
    T prefix_sum(std::size_t index) const {
        if (index >= m_size) {
            throw std::out_of_range("Index out of range");
        }
        
        T sum1 = m_tree1.prefix_sum(index) * static_cast<T>(index + 1);
        T sum2 = m_tree2.prefix_sum(index);
        return sum1 - sum2;
    }
    
    T range_sum(std::size_t left, std::size_t right) const {
        if (left > right || right >= m_size) {
            throw std::out_of_range("Invalid range");
        }
        
        if (left == 0) {
            return prefix_sum(right);
        }
        return prefix_sum(right) - prefix_sum(left - 1);
    }
    
    void set(std::size_t index, const T& value) {
        T current = get(index);
        T delta = value - current;
        update(index, delta);
    }
    
    std::size_t size() const noexcept {
        return m_size;
    }
    
    std::vector<T> to_vector() const {
        std::vector<T> result(m_size);
        for (std::size_t i = 0; i < m_size; ++i) {
            result[i] = get(i);
        }
        return result;
    }
};

template <typename T>
class FenwickTree2D {
private:
    std::vector<std::vector<T>> m_tree;
    std::size_t m_rows;
    std::size_t m_cols;
    
public:
    explicit FenwickTree2D(const std::vector<std::vector<T>>& matrix)
        : m_rows(matrix.size()),
          m_cols(matrix.empty() ? 0 : matrix[0].size()) {
        
        if (m_rows == 0 || m_cols == 0) {
            throw std::invalid_argument("Matrix cannot be empty");
        }
        
        m_tree.resize(m_rows + 1, std::vector<T>(m_cols + 1, T()));
        
        for (std::size_t i = 0; i < m_rows; ++i) {
            for (std::size_t j = 0; j < m_cols; ++j) {
                update(i, j, matrix[i][j]);
            }
        }
    }
    
    FenwickTree2D(std::size_t rows, std::size_t cols, const T& default_value = T())
        : m_tree(rows + 1, std::vector<T>(cols + 1, T())),
          m_rows(rows),
          m_cols(cols) {
        
        if (default_value != T()) {
            for (std::size_t i = 0; i < rows; ++i) {
                for (std::size_t j = 0; j < cols; ++j) {
                    update(i, j, default_value);
                }
            }
        }
    }
    
    void update(std::size_t row, std::size_t col, const T& delta) {
        if (row >= m_rows || col >= m_cols) {
            throw std::out_of_range("Index out of range");
        }
        
        ++row; ++col;
        
        for (std::size_t i = row; i <= m_rows; i += lsb(i)) {
            for (std::size_t j = col; j <= m_cols; j += lsb(j)) {
                m_tree[i][j] += delta;
            }
        }
    }
    
    T prefix_sum(std::size_t row, std::size_t col) const {
        if (row >= m_rows || col >= m_cols) {
            throw std::out_of_range("Index out of range");
        }
        
        T sum = T();
        ++row; ++col;
        
        for (std::size_t i = row; i > 0; i -= lsb(i)) {
            for (std::size_t j = col; j > 0; j -= lsb(j)) {
                sum += m_tree[i][j];
            }
        }
        
        return sum;
    }
    
    T range_sum(std::size_t r1, std::size_t c1, std::size_t r2, std::size_t c2) const {
        if (r1 > r2 || c1 > c2 || r2 >= m_rows || c2 >= m_cols) {
            throw std::out_of_range("Invalid range");
        }
        
        T sum = prefix_sum(r2, c2);
        
        if (r1 > 0) sum -= prefix_sum(r1 - 1, c2);
        if (c1 > 0) sum -= prefix_sum(r2, c1 - 1);
        if (r1 > 0 && c1 > 0) sum += prefix_sum(r1 - 1, c1 - 1);
        
        return sum;
    }
    
    T get(std::size_t row, std::size_t col) const {
        return range_sum(row, col, row, col);
    }
    
    void set(std::size_t row, std::size_t col, const T& value) {
        T current = get(row, col);
        T delta = value - current;
        update(row, col, delta);
    }
    
    std::pair<std::size_t, std::size_t> size() const noexcept {
        return {m_rows, m_cols};
    }
    
    std::size_t rows() const noexcept {
        return m_rows;
    }
    
    std::size_t cols() const noexcept {
        return m_cols;
    }
    
    std::vector<std::vector<T>> to_matrix() const {
        std::vector<std::vector<T>> result(m_rows, std::vector<T>(m_cols));
        for (std::size_t i = 0; i < m_rows; ++i) {
            for (std::size_t j = 0; j < m_cols; ++j) {
                result[i][j] = get(i, j);
            }
        }
        return result;
    }
    
private:
    static std::size_t lsb(std::size_t x) {
        return x & (-x);
    }
};

} // namespace tree
} // namespace mylib

#endif // MYLIB_TREE_FENWICK_TREE_HPP