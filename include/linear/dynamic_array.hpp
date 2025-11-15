/**
 * @file dynamic_array.hpp
 * @brief Dynamic array implementation with automatic memory management
 * @author Jinhyeok
 * @date 2025-11-15
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

#ifndef MYLIB_LINEAR_DYNAMIC_ARRAY_HPP
#define MYLIB_LINEAR_DYNAMIC_ARRAY_HPP

#include <cstddef>
#include <stdexcept>
#include <utility>
#include <initializer_list>

namespace mylib {
namespace linear {

/**
 * @class DynamicArray 
 * @berif A dynamic array container that automatically grows as needed
 * 
 * This class provides a dynamic array implementation similar to 
 * std::vector, with automatic memory management and capacity growth..
 *
 * @tparam T the type of elements stored in the array
 */
template <typename T>
class DynamicArray {
public:
    // Type aliases
    using value_type = T;
    using size_type = std::size_t;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using const_pointer = const T*;

    /**
     * @berif Constructor with initial capacity
     * @param initial_capacity Initial capacity to reserve
     */
    explicit DynamicArray(size_type initial_capacity);

    /**
    * @berif Constructor with size and default value 
    * @param count Number of elements
    * @param value Value to initialize elements with 
    */
    DynamicArray(size_type count, const T& value);

    /**
    * @berif Initialize list constructor 
    * @param init Initializer list
    */
    DynamicArray(std::initializer_list<T> init);

    /**
    * @berif Copy constructor 
    * @param other Array to copy from 
    */
    DynamicArray(const DynamicArray& other);

    /**
    * @berif Move constructor
    * @param other Array move to from 
    */
    DynamicArray(DynamicArray&& other) noexcept;

    /**
    * @berif Destructor
    */
    ~DynamicArray();

    /**
    * @berif Copy assignment operator
    * @param other Array to copy from
    * @return Reference to this array
    */
    DynamicArray& operator=(const DynamicArray& other);

    /**
    * @berif Move assignment operator
    * @param other Array to move from
    * @return Reference to this array
    */
    DynamicArray& operator=(const DynamicArray&& other);

    // Element Access
    /**
    * @berif Access element at index with bounds checking
    * @param index Position of element
    * @return Reference to elements
    * @throws std::out_of_range if index is out of bounds
    */
    reference at(size_type index);
    const_pointer at(size_type index) const;

    /**
    * @berif Access element at index without bounds checking
    * @param index Position of element
    * @return Reference of element 
    */
    reference operator[](size_type index);
    const_reference operator[](size_type index) const;

    /**
    * @berif Access the first element
    * @return Reference to first element
    */
    reference front();
    const_reference front() const;

    /**
    * @berif Access the last element
    * @return Reference to last element
    */
    reference back();
    const_reference back() const;

    /**
    * @berif Direct access to underlying array
    * @return Pointer to underlying array
    */
    pointer data() noexcept;
    const_pointer data() const noexcept;

    // Capacity
    /**
    * @berif Check if array is empty
    * @return true if empty, false otherwise 
    */
    bool empty() const noexcept;

    /**
    * @berif Get number of elements
    * @return Number of elements
    */
    size_type size() const noexcept;

    /**
    * @berif Get current capacity
    * @return Currrent capacity
    */
    size_type capacity() const noexcept;

    /**
    * @berif Reverse capacity for at least new_capacity elements
    * @param new_capacity Minimum capacity to reverse 
    */
    void reverse(size_type new_capacity);

    /**
    * @berif Reduce capacity to fit size
    */
    void shrink_to_fit();

    //Modifiers
    /**
    * @berif Clear all elements
    */
    void clear() noexcept;

    /**
    * @berif Add element to end
    * @param value Value to end
    */
    void push_back(const T& value);

    /**
    * @berif Add element to end (move version)
    * @param value Value to move
    */
    void push_back(const T&& value);

    /**
    * @berif Remove last element
    */
    void pop_back();

    /**
     * @berif Resize array to contain count elements
     * @param Count new size
     */
    void resize(size_type count);

    /**
    * @berif Resize array with default value
    * @param count New size
    * @param value Value for new element
    */
    void resize(size_type count, const T& value);

    /**
    * @berif Swap contents with another array
    * @param other Array to swap with
    */
    void swap(DynamicArray& other) noexcept;

private:
    pointer m_data; ///< Pointer to array m_data
    size_type m_size; ///< Current number of elements
    size_type m_capacity; ///< Current capacity

    static constexpr size_type DEFAULT_CAPACITY = 16;
    static constexpr double GROWTH_FACTOR = 2.0;

    /**
    * @berif Grow capacity when needed
    */
    void grow();

    /**
    * @berif Reallocate with new capacity
    * @param new_capacity New capacity
    */
    void reallocate(size_type new_capacity);

};

} // end of linear
} // end of mylib
#endif // MYLIB_LINEAR_DYNAMIC_ARRAY_HPP
