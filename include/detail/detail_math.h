#ifndef GEO_DETAIL_MATH_H
#define GEO_DETAIL_MATH_H

#include "traits.h"

namespace geo {

namespace detail {

template <typename T>
requires std::floating_point<T>
constexpr T
sqrtNewtonRaphson(T x, T curr, T prev) noexcept {
  return curr == prev
    ? curr
    : sqrtNewtonRaphson(x, T(0.5) * (curr + x / curr), curr);
}

} // namespace detail

} // namespace geo

#endif
