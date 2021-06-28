#ifndef GEO_MATH_HPP
#define GEO_MATH_HPP

#include <concepts>
#include <cmath>
#include <limits>
#include <numbers>

#include "detail/detail_math.hpp"

namespace geo {

template <std::floating_point T>
constexpr T
radiansToDegree(T radians) noexcept {
  return radians * T(180.0) / std::numbers::pi;
}

template <std::floating_point T>
constexpr T
degreeToRadians(T degree) noexcept {
  return degree / T(180.0) * std::numbers::pi;
}

template <std::integral T>
constexpr double
degreeToRadians(T degree) noexcept {
  return degreeToRadians(static_cast<double>(degree));
}

template <std::floating_point T>
constexpr T
sqrt(T x) {
  if (std::is_constant_evaluated()) {
    return x >= T() && x < std::numeric_limits<T>::infinity()
      ? detail::sqrtNewtonRaphson(x, x, T())
      : std::numeric_limits<T>::quiet_NaN();
  } else {
    return std::sqrt(x);
  }
}

template <std::integral T>
constexpr double
sqrt(T x) {
  return sqrt(static_cast<double>(x));
}

template <
  std::totally_ordered Head0,
  std::totally_ordered Head1,
  std::totally_ordered... Tail
>
constexpr auto
max(Head0 && head0, Head1 && head1, Tail && ... tail) {
  auto result = head0 < head1 ? head1 : head0;
  auto max_helper = [&result](auto && el) {
    result = (el < result) ? result : el;
  };
  (..., max_helper(std::forward<Tail>(tail)));
  return result;
}

template <
  std::totally_ordered Head0,
  std::totally_ordered Head1,
  std::totally_ordered... Tail
>
constexpr auto
min(Head0 && head0, Head1 && head1, Tail && ... tail) {
  auto result = head0 < head1 ? head0 : head1;
  auto min_helper = [&result](auto && el) {
    result = (el < result) ? el : result;
  };
  (..., min_helper(std::forward<Tail>(tail)));
  return result;
}

} // namespace geo

#endif
