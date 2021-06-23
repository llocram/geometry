#ifndef GEO_DETAIL_MATH_HPP
#define GEO_DETAIL_MATH_HPP

#include "../traits.hpp"

namespace geo {

namespace detail {

template <std::floating_point T>
constexpr T
sqrtNewtonRaphson(T x, T curr, T prev) noexcept {
  return curr == prev
    ? curr
    : sqrtNewtonRaphson(x, T(0.5) * (curr + x / curr), curr);
}

} // namespace detail

} // namespace geo

#endif
