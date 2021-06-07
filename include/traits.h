#ifndef GEO_TRAITS_H
#define GEO_TRAITS_H

#include <concepts>
#include <cstdint>

namespace geo {

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
    && (dimension<point_type_t<T>>::value == 3 || dimension<point_type_t<T>>::value == 2))>
struct is_circle : std::false_type {};

template <typename T>
struct is_circle<T, true> : std::true_type {};

} // namespace traits

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

namespace traits {

template <typename T, std::size_t I>
struct access {
  static value_type_t<T>
  get(T const &);
  
  static void
  set(T &, value_type_t<T>);
};

template <typename T>
struct access_center {
  static typename T::point_type_t const &
  get(T const &);
  
  static void
  set(T &, typename T::point_type_t const &);
};

template <typename T>
struct access_radius {
  static value_type_t<T>
  get(T const &);
  
  static void
  set(T &, value_type_t<T>);
};

} // namespace traits

template <typename T, std::size_t I>
requires concepts::point<T>
static traits::value_type_t<T>
get(T const & t) {
  return traits::access<T, I>::get(t);
}

template <typename T, std::size_t I>
requires concepts::point<T>
static void
set(T & t, traits::value_type_t<T> value) {
  traits::access<T, I>::set(t, value);
}

template <typename T, std::size_t I>
requires concepts::circle<T>
static traits::value_type_t<T>
get_center(T const & t) {
  return traits::access<T, I>::get(traits::access_center<T>::get(t));
}

template <typename T, std::size_t I>
requires concepts::circle<T>
static void
set_center(T & t, traits::value_type_t<T> value) {
  traits::access<T, I>::set(traits::access_center<T>::get(t), value);
}

template <typename T>
requires concepts::circle<T>
static traits::value_type_t<T>
get_radius(T const & t) {
  return traits::access_radius<T>::get(t);
}

template <typename T>
requires concepts::circle<T>
static void
set_radius(T & t, traits::value_type_t<T> value) {
  traits::access_radius<T>::set(t, value);
}

} // namespace geo

#endif
