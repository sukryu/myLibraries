/**
 * @file dynamic_array_fwd.hpp
 * @brief Forward declarations and type traits for DynamicArray
 * @author Jinhyeok
 * @date 2025-12-06
 * @version 2.0.0
 */

#ifndef MYLIB_LINEAR_DYNAMIC_ARRAY_FWD_HPP
#define MYLIB_LINEAR_DYNAMIC_ARRAY_FWD_HPP

#include <cstddef>
#include <type_traits>

namespace mylib {
namespace linear {

// Forward declaration
template <typename T>
class DynamicArray;

// Type traits helpers
namespace detail {

/**
 * @brief Check if type is nothrow move constructible
 */
template <typename T>
inline constexpr bool is_nothrow_movable_v = 
    std::is_nothrow_move_constructible_v<T> && 
    std::is_nothrow_move_assignable_v<T>;

/**
 * @brief Check if type is trivially copyable
 */
template <typename T>
inline constexpr bool is_trivially_copyable_v = 
    std::is_trivially_copyable_v<T>;

} // namespace detail

} // namespace linear
} // namespace mylib

#endif // MYLIB_LINEAR_DYNAMIC_ARRAY_FWD_HPP