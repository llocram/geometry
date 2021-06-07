#ifndef GEO_ALGEBRA_H
#define GEO_ALGEBRA_H

#include "detail/detail_algebra.h"

namespace geo {

template <typename T>
requires std::floating_point<T>
constexpr T
radiansToDegree(T radians) noexcept {
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
sqrt(T x) {
  if (std::is_constant_evaluated()) {
    return x >= T() && x < std::numeric_limits<T>::infinity()
      ? detail::sqrtNewtonRaphson(x, x, T())
      : std::numeric_limits<T>::quiet_NaN();
  } else {
    return std::sqrt(x);
  }
}

template <typename T>
requires std::integral<T>
constexpr double
sqrt(T x) {
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

template <typename Point>
requires concepts::point<Point>
[[nodiscard]] constexpr Point
operator-(Point const & lhs, Point const & rhs) noexcept {
  return detail::substract_impl(
    lhs, rhs, std::make_index_sequence<traits::dimension_v<Point>>());
}

template <typename Point>
requires concepts::point<Point>
[[nodiscard]] constexpr Point
operator+(Point const & lhs, Point const & rhs) noexcept {
  return detail::addition_impl(
    lhs, rhs, std::make_index_sequence<traits::dimension_v<Point>>());
}

template <typename Point, typename T>
requires concepts::point<Point>
      && concepts::value_type_equals<Point, T>
[[nodiscard]] constexpr Point
operator*(Point const & point, T scalar) noexcept {
  return detail::multiply_impl(
    point, scalar, std::make_index_sequence<traits::dimension_v<Point>>());
}

template <typename Point, typename T>
requires concepts::point<Point>
      && concepts::value_type_equals<Point, T>
[[nodiscard]] constexpr Point
operator*(T scalar, Point const & point) noexcept {
  return detail::multiply_impl(
    point, scalar, std::make_index_sequence<traits::dimension_v<Point>>());
}

template <typename Point, typename T>
requires concepts::point<Point>
      && concepts::value_type_equals<Point, T>
[[nodiscard]] constexpr Point
operator/(Point const & point, T scalar) noexcept(std::is_floating_point_v<T>) {
  return detail::division_impl(
    point, scalar, std::make_index_sequence<traits::dimension_v<Point>>());
}

template <typename Geo1, typename Geo2>
requires concepts::geo_object<Geo1>
      && concepts::geo_object<Geo2>
      && concepts::same_value_type<Geo1, Geo2>
constexpr auto
distance(Geo1 const & lhs, Geo2 const & rhs) noexcept {
  return detail::distance_impl(
    lhs, rhs,
    traits::tag_t<Geo1>(), traits::tag_t<Geo2>());
}

template <typename Point>
requires concepts::point<Point>
      && concepts::dimension_equals<Point, 3>
[[nodiscard]] constexpr Point
vector_product(Point const & lhs, Point const & rhs) noexcept {
  return Point(
      get<Point, 1>(lhs) * get<Point, 2>(rhs) - get<Point, 2>(lhs) * get<Point, 1>(rhs),
      get<Point, 2>(lhs) * get<Point, 0>(rhs) - get<Point, 0>(lhs) * get<Point, 2>(rhs),
      get<Point, 0>(lhs) * get<Point, 1>(rhs) - get<Point, 1>(rhs) * get<Point, 0>(rhs));
}

template <typename Point>
requires concepts::point<Point>
constexpr auto
angle(Point const & lhs, Point const & rhs) {
  constexpr auto zero = traits::value_type_t<Point>();
  const auto normProduct = norm(lhs) * norm(rhs);
  if (normProduct == zero) {
    throw std::invalid_argument("unable to calculate angle for zero-length vector");
  } else {
    return std::acos(dot_product<Point, Point>(lhs, rhs) / normProduct);
  }
}

} // namespace geo

#endif
