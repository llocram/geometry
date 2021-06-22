#ifndef DETAIL_ALGORITHM_H
#define DETAIL_ALGORITHM_H

#include <numbers>
#include <type_traits>

#include "../traits.h"
#include "../circle.h"

namespace geo {

namespace detail {

template <typename Circle>
requires concepts::circle<Circle>
constexpr auto
area(Circle const & circle, traits::circle_tag) noexcept {
  using Scalar = traits::value_type_t<Circle>;
  if constexpr (std::is_floating_point_v<Scalar>) {
    return std::numbers::pi_v<Scalar> * get_radius(circle) * get_radius(circle);
  } else {
    return std::numbers::pi_v<double> * get_radius(circle) * get_radius(circle);
  }
}

} // namespace detail

} // namespace geo

#endif
