#ifndef GEO_TRAITS_H
#define GEO_TRAITS_H

#include <concepts>
#include <cstdint>

namespace geo {

/***************************** traits ********************************/

namespace traits {

struct point_tag {};
struct line_tag {};
struct box_tag {};
struct circle_tag {};
struct arc_tag {};
struct bezier_tag {};

template <typename T>
struct tag;

template <typename T>
using tag_t = typename tag<T>::type;

template <typename T>
struct value_type;

template <typename T>
using value_type_t = typename value_type<T>::type;

template <typename T>
struct dimension;

template <typename T>
inline constexpr std::size_t dimension_v = dimension<T>::value;

template <typename T>
struct point_type;

template <typename T>
using point_type_t = typename point_type<T>::type;

template <typename T, bool point = (std::is_same_v<tag_t<T>, point_tag>
                                 && std::is_arithmetic_v<value_type_t<T>>)>
struct is_point : std::false_type {};

template <typename T>
struct is_point<T, true> : std::true_type {};

template <typename T, bool line = std::is_same_v<tag_t<T>, line_tag>>
struct is_line : std::false_type {};

template <typename T>
struct is_line<T, true> : std::true_type {};

template <typename T, bool circle =
  (std::is_same_v<tag_t<T>, circle_tag>
    && is_point<point_type_t<T>>::value
    && (dimension<point_type_t<T>>::value == 3
        || dimension<point_type_t<T>>::value == 2))>
struct is_circle : std::false_type {};

template <typename T>
struct is_circle<T, true> : std::true_type {};

} // namespace traits

/***************************** concepts ********************************/

namespace concepts {

template <typename T>
concept arithmetic = std::integral<T> || std::floating_point<T>;

template <typename GeoObject1, typename GeoObject2>
concept same_dimension =
  (geo::traits::dimension_v<GeoObject1> ==
   geo::traits::dimension_v<GeoObject2>);

template <typename GeoObject, std::size_t dimension>
concept dimension_equals =
  geo::traits::dimension_v<GeoObject> == dimension;

template <typename GeoObject>
concept point = geo::traits::is_point<GeoObject>::value;

template <typename GeoObject>
concept circle = geo::traits::is_circle<GeoObject>::value;

template <typename GeoObject>
concept line = geo::traits::is_line<GeoObject>::value;

template <typename GeoObject>
concept geo_object =
  geo::traits::is_point<GeoObject>::value
  || geo::traits::is_circle<GeoObject>::value
  || geo::traits::is_line<GeoObject>::value;

template <typename GeoObject, typename T>
concept value_type_equals =
  std::is_same_v<geo::traits::value_type_t<GeoObject>, T>;

template <typename GeoObject1, typename GeoObject2>
concept same_value_type =
  std::is_same_v<geo::traits::value_type_t<GeoObject1>,
                 geo::traits::value_type_t<GeoObject2>>;

} // namespace concepts

/***************************** adaptors ********************************/

namespace traits {

template <typename Point, std::size_t I>
requires concepts::point<Point>
struct access {
  static value_type_t<Point>
  get(Point const &);
  
  static void
  set(Point &, value_type_t<Point>);
};

template <typename Circle>
requires concepts::circle<Circle>
struct access_center {
  static typename Circle::point_type_t const &
  get(Circle const &);
  
  static void
  set(Circle &, typename Circle::point_type_t const &);
};

template <typename Circle>
requires concepts::circle<Circle>
struct access_radius {
  static value_type_t<Circle>
  get(Circle const &);
  
  static void
  set(Circle &, value_type_t<Circle>);
};

} // namespace traits

/******************** convenience free functions *****************************/

template <typename Point, std::size_t I>
requires concepts::point<Point>
static traits::value_type_t<Point>
get(Point const & point) {
  return traits::access<Point, I>::get(point);
}

template <typename Point, std::size_t I>
requires concepts::point<Point>
static void
set(Point & point, traits::value_type_t<Point> value) {
  traits::access<Point, I>::set(point, value);
}

template <typename Circle, std::size_t I>
requires concepts::circle<Circle>
static traits::value_type_t<Circle>
get_center(Circle const & circle) {
  return traits::access<Circle, I>::get(traits::access_center<Circle>::get(circle));
}

template <typename Circle, std::size_t I>
requires concepts::circle<Circle>
static void
set_center(Circle & circle, traits::value_type_t<Circle> value) {
  traits::access<Circle, I>::set(traits::access_center<Circle>::get(circle), value);
}

template <typename Circle>
requires concepts::circle<Circle>
static traits::value_type_t<Circle>
get_radius(Circle const & circle) {
  return traits::access_radius<Circle>::get(circle);
}

template <typename Circle>
requires concepts::circle<Circle>
static void
set_radius(Circle & circle, traits::value_type_t<Circle> value) {
  traits::access_radius<Circle>::set(circle, value);
}

} // namespace geo

#endif
