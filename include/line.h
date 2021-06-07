#ifndef GEO_LINE_H
#define GEO_LINE_H

#include "traits.h"
#include "point.h"

namespace geo {

template <typename Point>
requires concepts::point<Point>
struct Line {
  Line() = default;
  Line(Point const & start, Point const & end)
    : start(start), end(end) {}

  Point start{};
  Point end{};
};

namespace traits {

template <typename Point>
struct tag<Line<Point>> {
  using type = line_tag;
};

} // namespace traits

} // namespace geo

#endif
