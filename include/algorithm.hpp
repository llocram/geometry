#ifndef GEO_ALGORITHM_HPP
#define GEO_ALGORITHM_HPP

#include "detail/detail_algorithm.hpp"

namespace geo {

template <concepts::geo_object Geo>
constexpr auto
area(Geo const & geo_object) noexcept {
  return detail::area(geo_object, traits::tag_t<Geo>());
}

} // namespace geo

#endif
