#ifndef GEO_DETAIL_MATH_HPP
#define GEO_DETAIL_MATH_HPP

#include <concepts>

namespace geo {

namespace detail {

template <std::floating_point T>
[[nodiscard]] constexpr T
sqrt_newton_raphson(T x, T curr, T prev) noexcept
{
  return curr == prev
    ? curr
    : sqrt_newton_raphson(x, T{0.5} * (curr + x / curr), curr);
}

} // namespace detail

} // namespace geo

#endif
