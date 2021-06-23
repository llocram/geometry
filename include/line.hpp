#ifndef GEO_LINE_HPP
#define GEO_LINE_HPP

#include "point.hpp"
#include "traits.hpp"

namespace geo {

/***************************** model ********************************/

template <concepts::point Point>
struct Line {
  Line() = default;
  Line(Point const & start, Point const & end)
      : start(start), end(end) {}

  Point start{};
  Point end{};
};

/***************************** adaptors ********************************/

namespace traits {

template <concepts::point Point>
struct tag<Line<Point>> {
  using type = line_tag;
};

} // namespace traits

} // namespace geo

#endif
