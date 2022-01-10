#ifndef GEO_CIRCLE_HPP
#define GEO_CIRCLE_HPP

#include <stdexcept>

#include "traits.hpp"
#include "point.hpp"

namespace geo {

/***************************** model ********************************/

template <concepts::point Point>
struct Circle
{
  using value_type = traits::value_type_t<Point>;

  Circle() = default;
  Circle(Point const & center, value_type radius)
      : center(center), radius(radius)
  {
    if (radius < value_type{}) {
      throw std::invalid_argument("negative radius");
    }
  }

  Point center{};
  value_type radius{};
};

/***************************** adaptors ********************************/

namespace traits {

template <concepts::point Point>
struct tag<Circle<Point>>
{
  using type = circle_tag;
};

template <concepts::point Point>
struct point_type<Circle<Point>>
{
  using type = Point;
};

template <concepts::point Point>
struct value_type<Circle<Point>>
{
  using type = value_type_t<Point>;
};

template <concepts::point Point>
struct access_center<Circle<Point>>
{
  [[nodiscard]] static constexpr Point const &
  get(Circle<Point> const &);
  
  static constexpr void
  set(Circle<Point> &, Point const &);
};

template <concepts::point Point>
struct access_radius<Circle<Point>>
{
  [[nodiscard]] static constexpr value_type_t<Point>
  get(Circle<Point> const & circle)
  {
    return circle.radius;
  }
  
  static constexpr void
  set(Circle<Point> & circle, value_type_t<Point> radius)
  {
    if (radius < value_type_t<Point>{}) {
      throw std::invalid_argument("negative radius");
    }
    circle.radius = radius;
  }
};

} // namespace traits

} // namespace geo

#endif
