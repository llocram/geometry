#ifndef GEO_ALGORITHM_H
#define GEO_ALGORITHM_H

#include "detail/detail_algorithm.h"

namespace geo {

template <typename Geo>
requires concepts::geo_object<Geo>
constexpr traits::value_type_t<Geo>
area(Geo const & geo_object) {
  return detail::area(geo_object, traits::tag_t<Geo>());
}

} // namespace geo

#endif
