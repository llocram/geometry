#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <cmath>
#include <concepts>
#include <cstdint>
#include <stdexcept>
#include <numbers>
#include <utility>

namespace geo {

/**************************** traits & concepts *******************************/

namespace traits {

struct point_type {};
struct circle_type {};
struct line_type {};

template <typename T>
struct value_type;

template <typename T>
struct dimension;

template <typename T, std::size_t I>
struct access {
  static typename value_type<T>::type get(T const &);
  static void set(T &, typename value_type<T>::type);
};

template <typename T>
struct tag;

template <typename T, bool point = (std::is_same_v<typename tag<T>::type, point_type>
                                 && std::is_arithmetic_v<typename value_type<T>::type>)>
struct is_point : std::false_type {};

template <typename T>
struct is_point<T, true> : std::true_type {};

template <typename T, bool circle = std::is_same_v<typename tag<T>::type, circle_type>>
struct is_circle : std::false_type {};

template <typename T>
struct is_circle<T, true> : std::true_type {};

template <typename T, bool line = std::is_same_v<typename tag<T>::type, line_type>>
struct is_line : std::false_type {};

template <typename T>
struct is_line<T, true> : std::true_type {};

} // namespace traits

namespace concepts {

template <typename T>
concept arithmetic = std::integral<T> || std::floating_point<T>;

template <typename GeoObject1, typename GeoObject2>
concept same_dimension =
  (geo::traits::dimension<GeoObject1>::value ==
   geo::traits::dimension<GeoObject2>::value);

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
  std::is_same_v<typename geo::traits::value_type<GeoObject>::type, T>;

template <typename GeoObject1, typename GeoObject2>
concept same_value_type =
  std::is_same_v<typename geo::traits::value_type<GeoObject1>::type,
                 typename geo::traits::value_type<GeoObject2>::type>;

} // namespace concepts

/********************* pre-defined geometric entities *************************/

template <typename T, typename... Mixins>
requires concepts::arithmetic<T>
struct Vector2x : public Mixins... {
  Vector2x() = default;
  Vector2x(T x, T y) : x(x), y(y) {};

  T x{};
  T y{};
};

template <typename T, typename... Mixins>
requires concepts::arithmetic<T>
struct Vector3x : public Mixins... {
  Vector3x() = default;
  Vector3x(T x, T y, T z) : x(x), y(y), z(z) {};

  T x{};
  T y{};
  T z{};
};

using Vector2d = Vector2x<double>;
using Vector3d = Vector3x<double>;

template <typename Point>
requires concepts::point<Point>
struct Line {
  Line() = default;
  Line(Point const & start, Point const & end)
    : start(start), end(end) {}

  Point start{};
  Point end{};
};

template <typename Point, typename T = double>
requires concepts::point<Point> && concepts::value_type_equals<Point, T>
struct Circle {
  Circle() = default;
  Circle(Point const & center, T radius) : center(center), radius(radius) {
    if (radius < T()) {
      throw std::invalid_argument("negative radius");
    }
  }

  Point center{};
  T radius{};
};

namespace traits {

template <typename T, typename...Mixins>
struct tag<Vector2x<T, Mixins...>> { using type = point_type; };

template <typename T, typename... Mixins>
struct value_type<Vector2x<T, Mixins...>> {
  using type = T;
};

template <typename T, typename...Mixins>
struct dimension<Vector2x<T, Mixins...>> {
  static constexpr std::size_t value = 2;
};

template <typename T, typename... Mixins>
struct access<Vector2x<T, Mixins...>, 0> {
  static T get(Vector2x<T, Mixins...> const & vec) { return vec.x; }
  static void set(Vector2x<T, Mixins...> & vec, T x) { vec.x = x; }
};

template <typename T, typename... Mixins>
struct access<Vector2x<T, Mixins...>, 1> {
  static T get(Vector2x<T, Mixins...> const & vec) { return vec.y; }
  static void set(Vector2x<T, Mixins...> & vec, T y) { vec.y = y; }
};

template <typename T, typename...Mixins>
struct tag<Vector3x<T, Mixins...>> { using type = point_type; };

template <typename T, typename... Mixins>
struct value_type<Vector3x<T, Mixins...>> {
  using type = T;
};

template <typename T, typename...Mixins>
struct dimension<Vector3x<T, Mixins...>> {
  static constexpr std::size_t value = 3;
};

template <typename T, typename... Mixins>
struct access<Vector3x<T, Mixins...>, 0> {
  static T get(Vector3x<T, Mixins...> const & vec) { return vec.x; }
  static void set(Vector3x<T, Mixins...> & vec, T x) { vec.x = x; }
};

template <typename T, typename... Mixins>
struct access<Vector3x<T, Mixins...>, 1> {
  static T get(Vector3x<T, Mixins...> const & vec) { return vec.y; }
  static void set(Vector3x<T, Mixins...> & vec, T y) { vec.y = y; }
};

template <typename T, typename... Mixins>
struct access<Vector3x<T, Mixins...>, 2> {
  static T get(Vector3x<T, Mixins...> const & vec) { return vec.z; }
  static void set(Vector3x<T, Mixins...> & vec, T z) { vec.z = z; }
};

template <typename Point>
struct tag<Line<Point>> { using type = line_type; };

template <typename Point, typename T>
struct tag<Circle<T, Point>> { using type = circle_type; };

} // namespace traits

/******************************* algorithm ************************************/

namespace detail {

/* don't use std::sqrt, it is neither constexpr nor noexcept */
template <typename T>
requires std::floating_point<T>
constexpr T
sqrtNewtonRaphson(T x, T curr, T prev) noexcept {
  return curr == prev
    ? curr
    : sqrtNewtonRaphson(x, T(0.5) * (curr + x / curr), curr);
}

template <typename Point1, typename Point2, std::size_t ... I>
requires concepts::point<Point1>
      && concepts::point<Point2>
      && concepts::same_value_type<Point1, Point2>
      && concepts::same_dimension<Point1, Point2>
typename traits::value_type<Point1>::type
dot_product_impl(
    Point1 const & lhs, Point2 const & rhs, std::index_sequence<I...> const &) noexcept {
  using traits::access;
  return (... + (access<Point1, I>::get(lhs) * access<Point2, I>::get(rhs)));
}

} // namespace detail

constexpr auto
radiansToDegree(std::floating_point<T> auto radians) noexcept {
  return radians * T(180.0) / std::numbers::pi;
}

template <typename T>
requires std::floating_point<T>
constexpr T
degreeToRadians(T degree) noexcept {
  return degree / T(180.0) * std::numbers::pi;
}

template <typename T>
requires std::integral<T>
constexpr double
degreeToRadians(T degree) noexcept {
  return degreeToRadians(static_cast<double>(degree));
}

template <typename T>
requires std::floating_point<T>
constexpr T
sqrt(T x) noexcept {
  return x >= T() && x < std::numeric_limits<T>::infinity()
    ? detail::sqrtNewtonRaphson(x, x, T())
    : std::numeric_limits<T>::quiet_NaN();
}

template <typename T>
requires std::integral<T>
constexpr double
sqrt(T x) noexcept {
  return sqrt(static_cast<double>(x));
}

template <typename Point1, typename Point2>
requires concepts::point<Point1>
      && concepts::point<Point2>
      && concepts::same_value_type<Point1, Point2>
      && concepts::same_dimension<Point1, Point2>
typename traits::value_type<Point1>::type
dot_product(Point1 const & lhs, Point2 const & rhs) noexcept {
  return detail::dot_product_impl(
    lhs, rhs, std::make_index_sequence<traits::dimension<Point1>::value>());
}

template <typename Point>
requires concepts::point<Point>
constexpr auto
norm(Point const & point) {
  return sqrt(dot_product(point, point));
}

namespace detail {

template <typename Point1, typename Point2>
requires concepts::point<Point1>
      && concepts::point<Point2>
      && concepts::same_value_type<Point1, Point2>
      && concepts::same_dimension<Point1, Point2>
constexpr auto
distance_impl(
    Point1 const & lhs, Point2 const & rhs,
    traits::point_type, traits::point_type) noexcept {
  return norm(lhs - rhs);
}

template <typename Point, std::size_t... I>
requires concepts::point<Point>
constexpr Point
substract_impl(Point const & lhs, Point const & rhs, std::index_sequence<I...> const &) noexcept {
  using traits::access;
  Point retval;
  (..., access<Point, I>::set(retval, access<Point, I>::get(lhs) - access<Point, I>::get(rhs)));
  return retval;
}

template <typename Point, std::size_t... I>
requires concepts::point<Point>
constexpr Point
addition_impl(Point const & lhs, Point const & rhs, std::index_sequence<I...> const &) noexcept {
  using traits::access;
  Point retval;
  (..., access<Point, I>::set(retval, access<Point, I>::get(lhs) + access<Point, I>::get(rhs)));
  return retval;
}

template <typename Point, typename T, std::size_t... I>
requires concepts::point<Point>
      && concepts::value_type_equals<Point, T>
constexpr Point
multiply_impl(Point const & lhs, T scalar, std::index_sequence<I...> const &) noexcept {
  using traits::access;
  Point retval;
  (..., access<Point, I>::set(retval, access<Point, I>::get(lhs) * scalar));
  return retval;
}

template <typename Point, typename T, std::size_t... I>
requires concepts::point<Point>
      && concepts::value_type_equals<Point, T>
constexpr Point
division_impl(Point const & lhs, T scalar, std::index_sequence<I...> const &) noexcept(std::is_floating_point_v<T>) {
  if constexpr (std::is_integral_v<T>) {
    if (scalar == 0) {
      throw std::runtime_error("division by zero");
    }
  }
  using traits::access;
  Point retval;
  (..., access<Point, I>::set(retval, access<Point, I>::get(lhs) / scalar));
  return retval;
}

} // namespace detail

template <typename Point>
requires concepts::point<Point>
[[nodiscard]] constexpr Point
operator-(Point const & lhs, Point const & rhs) noexcept {
  return detail::substract_impl(
    lhs, rhs, std::make_index_sequence<traits::dimension<Point>::value>());
}

template <typename Point>
requires concepts::point<Point>
[[nodiscard]] constexpr Point
operator+(Point const & lhs, Point const & rhs) noexcept {
  return detail::addition_impl(
    lhs, rhs, std::make_index_sequence<traits::dimension<Point>::value>());
}

template <typename Point, typename T>
requires concepts::point<Point>
      && concepts::value_type_equals<Point, T>
[[nodiscard]] constexpr Point
operator*(Point const & point, T scalar) noexcept {
  return detail::multiply_impl(
    point, scalar, std::make_index_sequence<traits::dimension<Point>::value>());
}

template <typename Point, typename T>
requires concepts::point<Point>
      && concepts::value_type_equals<Point, T>
[[nodiscard]] constexpr Point
operator*(T scalar, Point const & point) noexcept {
  return detail::multiply_impl(
    point, scalar, std::make_index_sequence<traits::dimension<Point>::value>());
}

template <typename Point, typename T>
requires concepts::point<Point>
      && concepts::value_type_equals<Point, T>
[[nodiscard]] constexpr Point
operator/(Point const & point, T scalar) noexcept(std::is_floating_point_v<T>) {
  return detail::division_impl(
    point, scalar, std::make_index_sequence<traits::dimension<Point>::value>());
}

template <typename Geo1, typename Geo2>
requires concepts::geo_object<Geo1>
      && concepts::geo_object<Geo2>
      && concepts::same_value_type<Geo1, Geo2>
constexpr auto
distance(Geo1 const & lhs, Geo2 const & rhs) noexcept {
  return detail::distance_impl(
    lhs, rhs,
    typename traits::tag<Geo1>::type(), typename traits::tag<Geo2>::type());
}

template <typename Point>
requires concepts::point<Point>
[[nodiscard]] constexpr Point
vector_product(Point const & lhs, Point const & rhs) noexcept {
  using traits::access;
  return T(access<Point, 2>::get(lhs) * access<Point, 3>::get(rhs) - access<Point, 3>::get(lhs) * access<Point, 2>::get(rhs),
           access<Point, 3>::get(lhs) * access<Point, 1>::get(rhs) - access<Point, 1>::get(lhs) * access<Point, 3>::get(rhs),
           access<Point, 2>::get(lhs) * access<Point, 3>::get(rhs) - access<Point, 3>::get(rhs) * access<Point, 2>::get(rhs));
}

template <typename Point>
requires concepts::point<Point>
constexpr auto
angle(Point const & lhs, Point const & rhs) {
  constexpr auto zero = typename traits::value_type<Point>::type();
  const auto normProduct = norm(lhs) * norm(rhs);
  if (normProduct == zero) {
    throw std::invalid_argument("unable to calculate angle for zero-length vector");
  } else {
    return std::acos(dot_product<Point, Point>(lhs, rhs) / normProduct);
  }
}

} // namespace geo

#endif
