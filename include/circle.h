#ifndef GEO_CIRCLE_H
#define GEO_CIRCLE_H

#include <stdexcept>

#include "traits.h"
#include "point.h"

namespace geo {

template <typename Point>
requires concepts::point<Point>
struct Circle {
  using value_type = traits::value_type_t<Point>;

  Circle() = default;
  Circle(Point const & center, value_type radius)
    : center(center), radius(radius) {
    if (radius < value_type()) {
      throw std::invalid_argument("negative radius");
    }
  }

  Point center{};
  value_type radius{};
};

namespace traits {

template <typename Point>
struct tag<Circle<Point>> {
  using type = circle_tag;
};

template <typename Point>
struct point_type<Circle<Point>> {
  using type = Point;
};

template <typename Point>
struct value_type<Circle<Point>> {
  using type = value_type_t<Point>;
};

template <typename Point>
struct access_center<Circle<Point>> {
  static Point const &
  get(Circle<Point> const &);
  
  static void
  set(Circle<Point> &, Point const &);
};

template <typename Point>
struct access_radius<Circle<Point>> {
  static value_type_t<Point>
  get(Circle<Point> const & circle) {
    return circle.radius;
  }
  
  static void
  set(Circle<Point> & circle, value_type_t<Point> radius) {
    circle.radius = radius;
  }
};

} // namespace traits

} // namespace geo

#endif
