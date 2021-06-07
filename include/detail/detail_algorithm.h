#ifndef DETAIL_ALGORITHM_H
#define DETAIL_ALGORITHM_H

#include <numbers>

#include "traits.h"
#include "circle.h"

namespace geo {

namespace detail {

template <typename Circle>
requires concepts::circle<Circle>
constexpr traits::value_type_t<Circle>
area(Circle const & circle, traits::circle_tag) {
  using T = traits::value_type_t<Circle>;
  return T(2.0) * std::numbers::pi_v<T> * get_radius(circle);
}

} // namespace detail

} // namespace geo

#endif
