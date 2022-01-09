#ifndef GEO_DETAIL_ALGEBRA_HPP
#define GEO_DETAIL_ALGEBRA_HPP

#include <cmath>
#include <numbers>
#include <stdexcept>
#include <utility>

#include "../traits.hpp"
#include "../point.hpp"

namespace geo {

namespace detail {

template <concepts::point Point>
constexpr auto
distance_impl(
    Point const & lhs, Point const & rhs,
    traits::point_tag, traits::point_tag) noexcept {
  return norm(lhs - rhs);
}

} // namespace detail

} // namespace geo

#endif
