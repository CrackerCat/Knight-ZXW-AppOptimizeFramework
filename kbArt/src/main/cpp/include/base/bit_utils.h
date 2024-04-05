//
// Created by Knight-ZXW on 2024/4/3.
//

#pragma once
#include <limits>
#include <type_traits>
#include "stl_util_identity.h"
using namespace art;
namespace art{
// For rounding integers.
// Note: Omit the `n` from T type deduction, deduce only from the `x` argument.
template<typename T>
constexpr T RoundDown(T x, typename Identity<T>::type n);

// For rounding integers.
// Note: Omit the `n` from T type deduction, deduce only from the `x` argument.
template<typename T>
constexpr T RoundDown(T x, typename Identity<T>::type n) ;

template<typename T>
constexpr T RoundDown(T x, typename Identity<T>::type n) {
  return (x & -n);
}

template<typename T>
constexpr T RoundUp(T x, std::remove_reference_t<T> n) ;

template<typename T>
constexpr T RoundUp(T x, std::remove_reference_t<T> n) {
  return RoundDown(x + n - 1, n);
}

}





