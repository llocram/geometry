#ifndef GEO_TRAITS_HPP
#define GEO_TRAITS_HPP

#include <array>
#include <concepts>
#include <cstdint>

namespace geo {

/***************************** traits ********************************/

namespace traits {

template <typename T>
struct is_array : public std::false_type {};

template <typename T, std::size_t N>
struct is_array<std::array<T, N>> : public std::true_type {};

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
struct degree;

template <typename T>
inline constexpr std::size_t degree_v = degree<T>::value;

template <typename T>
struct point_type;

template <typename T>
using point_type_t = typename point_type<T>::type;

template <typename T>
struct const_iter;

template <typename T>
using const_iter_t = typename const_iter<T>::type;

template <typename T>
struct iter;

template <typename T>
using iter_t = typename iter<T>::type;

template <typename T, bool _ = (std::is_same_v<tag_t<T>, point_tag>
                                 && std::is_arithmetic_v<value_type_t<T>>)>
struct is_point : std::false_type {};

template <typename T>
struct is_point<T, true> : std::true_type {};

template <typename T, bool _ = std::is_same_v<tag_t<T>, line_tag>>
struct is_line : std::false_type {};

template <typename T>
struct is_line<T, true> : std::true_type {};

template <typename T, bool _ =
  (std::is_same_v<tag_t<T>, circle_tag>
    && is_point<point_type_t<T>>::value
    && (dimension<point_type_t<T>>::value == 3
        || dimension<point_type_t<T>>::value == 2))>
struct is_circle : std::false_type {};

template <typename T>
struct is_circle<T, true> : std::true_type {};

template <typename T, bool _ = std::is_same_v<tag_t<T>, bezier_tag>>
struct is_bezier : std::false_type {};

template <typename T>
struct is_bezier<T, true> : std::true_type {};

} // namespace traits

/***************************** concepts ********************************/

namespace concepts {

template <class ContainerType>
concept container = requires(ContainerType a, const ContainerType b) {
  requires std::regular<ContainerType>;
  requires std::swappable<ContainerType>;
  requires std::destructible<typename ContainerType::value_type>;
  requires std::same_as<typename ContainerType::reference, typename ContainerType::value_type &>;
  requires std::same_as<typename ContainerType::const_reference, const typename ContainerType::value_type &>;
  requires std::forward_iterator<typename ContainerType::iterator>;
  requires std::forward_iterator<typename ContainerType::const_iterator>;
  requires std::signed_integral<typename ContainerType::difference_type>;
  requires std::same_as<
    typename ContainerType::difference_type,
    typename std::iterator_traits<typename ContainerType::iterator>::difference_type
  >;
  requires std::same_as<
    typename ContainerType::difference_type,
    typename std::iterator_traits<typename ContainerType::const_iterator>::difference_type
  >;
  { a.begin() } -> std::same_as<typename ContainerType::iterator>;
  { a.end() } -> std::same_as<typename ContainerType::iterator>;
  { b.begin() } -> std::same_as<typename ContainerType::const_iterator>;
  { b.end() } -> std::same_as<typename ContainerType::const_iterator>;
  { a.cbegin() } -> std::same_as<typename ContainerType::const_iterator>;
  { a.cend() } -> std::same_as<typename ContainerType::const_iterator>;
  { a.size() } -> std::same_as<typename ContainerType::size_type>;
  { a.max_size() } -> std::same_as<typename ContainerType::size_type>;
  { a.empty() } -> std::same_as<bool>;
};

template <typename T>
concept array = geo::traits::is_array<T>::value;

template <typename T>
concept not_an_array = !geo::traits::is_array<T>::value;

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
concept line = geo::traits::is_line<GeoObject>::value;

template <typename GeoObject>
concept circle = geo::traits::is_circle<GeoObject>::value;

template <std::size_t N>
concept bezier_degree = (N > 0 && N < 11);

template <typename GeoObject>
concept bezier = geo::traits::is_bezier<GeoObject>::value
              && bezier_degree<traits::degree_v<GeoObject>>;

template <typename GeoObject>
concept geo_object =
  geo::traits::is_point<GeoObject>::value
  || geo::traits::is_circle<GeoObject>::value
  || geo::traits::is_line<GeoObject>::value
  || geo::traits::is_bezier<GeoObject>::value;

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

template <concepts::point Point, std::size_t I>
struct access {
  static constexpr value_type_t<Point>
  get(Point const &);

  static constexpr void
  set(Point &, value_type_t<Point>);
};

template <concepts::circle Circle>
struct access_center {
  static constexpr typename Circle::point_type_t const &
  get(Circle const &);

  static constexpr void
  set(Circle &, typename Circle::point_type_t const &);
};

template <concepts::circle Circle>
struct access_radius {
  static value_type_t<Circle>
  get(Circle const &);

  static void
  set(Circle &, value_type_t<Circle>);
};

template <concepts::bezier Bezier>
struct access_bezier {
  static const_iter_t<Bezier>
  cbegin(Bezier const &);

  static constexpr const_iter_t<Bezier>
  cecbegin(Bezier const &);

  static iter_t<Bezier>
  begin(Bezier &);
};

} // namespace traits

/******************** convenience free functions *****************************/

template <concepts::point Point, std::size_t I>
static constexpr traits::value_type_t<Point>
get(Point const & point) {
  return traits::access<Point, I>::get(point);
}

template <concepts::point Point, std::size_t I>
static constexpr void
set(Point & point, traits::value_type_t<Point> value) {
  traits::access<Point, I>::set(point, value);
}

template <concepts::circle Circle, std::size_t I>
static traits::value_type_t<Circle>
get_center(Circle const & circle) {
  return traits::access<Circle, I>::get(traits::access_center<Circle>::get(circle));
}

template <concepts::circle Circle, std::size_t I>
static void
set_center(Circle & circle, traits::value_type_t<Circle> value) {
  traits::access<Circle, I>::set(traits::access_center<Circle>::get(circle), value);
}

template <concepts::circle Circle>
static traits::value_type_t<Circle>
get_radius(Circle const & circle) {
  return traits::access_radius<Circle>::get(circle);
}

template <concepts::circle Circle>
static void
set_radius(Circle & circle, traits::value_type_t<Circle> value) {
  traits::access_radius<Circle>::set(circle, value);
}

template <concepts::bezier Bezier>
static auto
cbegin(Bezier const & bezier) {
  return traits::access_bezier<Bezier>::cbegin(bezier);
}

template <concepts::bezier Bezier>
static constexpr auto
cecbegin(Bezier const & bezier) {
  return traits::access_bezier<Bezier>::cecbegin(bezier);
}


} // namespace geo

#endif
