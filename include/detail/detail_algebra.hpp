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

[[nodiscard]] constexpr auto
distance_impl(
    concepts::point auto const & lhs, concepts::point auto const & rhs,
    traits::point_tag, traits::point_tag) noexcept
{
  return norm(lhs - rhs);
}

} // namespace detail

} // namespace geo

#endif
