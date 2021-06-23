#ifndef GEO_ALGEBRA_HPP
#define GEO_ALGEBRA_HPP

#include "detail/detail_algebra.hpp"

namespace geo {

template <concepts::point Point>
[[nodiscard]] constexpr Point
operator-(Point const & lhs, Point const & rhs) noexcept {
  return detail::substract_impl(
    lhs, rhs, std::make_index_sequence<traits::dimension_v<Point>>());
}

template <concepts::point Point>
[[nodiscard]] constexpr Point
operator+(Point const & lhs, Point const & rhs) noexcept {
  return detail::addition_impl(
    lhs, rhs, std::make_index_sequence<traits::dimension_v<Point>>());
}

template <concepts::point Point, typename Scalar>
requires concepts::value_type_equals<Point, Scalar>
[[nodiscard]] constexpr Point
operator*(Point const & point, Scalar scalar) noexcept {
  return detail::multiply_impl(
    point, scalar, std::make_index_sequence<traits::dimension_v<Point>>());
}

template <concepts::point Point, typename Scalar>
requires concepts::value_type_equals<Point, Scalar>
[[nodiscard]] constexpr Point
operator*(Scalar scalar, Point const & point) noexcept {
  return detail::multiply_impl(
    point, scalar, std::make_index_sequence<traits::dimension_v<Point>>());
}

template <concepts::point Point, typename Scalar>
requires concepts::value_type_equals<Point, Scalar>
[[nodiscard]] constexpr Point
operator/(Point const & point, Scalar scalar) noexcept(std::is_floating_point_v<Scalar>) {
  return detail::division_impl(
    point, scalar, std::make_index_sequence<traits::dimension_v<Point>>());
}

template <concepts::point Point1, concepts::point Point2>
requires concepts::same_value_type<Point1, Point2>
      && concepts::same_dimension<Point1, Point2>
constexpr traits::value_type_t<Point1>
dot_product(Point1 const & lhs, Point2 const & rhs) noexcept {
  return detail::dot_product_impl(
    lhs, rhs, std::make_index_sequence<traits::dimension<Point1>::value>());
}

template <concepts::point Point>
constexpr auto
norm(Point const & point) {
  return sqrt(dot_product(point, point));
}

template <concepts::geo_object Geo1, concepts::geo_object Geo2>
requires concepts::same_value_type<Geo1, Geo2>
constexpr auto
distance(Geo1 const & lhs, Geo2 const & rhs) noexcept {
  return detail::distance_impl(
    lhs, rhs,
    traits::tag_t<Geo1>(), traits::tag_t<Geo2>());
}

template <concepts::point Point>
requires concepts::dimension_equals<Point, 3>
[[nodiscard]] constexpr Point
vector_product(Point const & lhs, Point const & rhs) noexcept {
  return Point(
      get<Point, 1>(lhs) * get<Point, 2>(rhs) - get<Point, 2>(lhs) * get<Point, 1>(rhs),
      get<Point, 2>(lhs) * get<Point, 0>(rhs) - get<Point, 0>(lhs) * get<Point, 2>(rhs),
      get<Point, 0>(lhs) * get<Point, 1>(rhs) - get<Point, 1>(rhs) * get<Point, 0>(rhs));
}

template <concepts::point Point>
auto
angle(Point const & lhs, Point const & rhs) noexcept (std::is_floating_point_v<traits::value_type_t<Point>>){
  using value_type = traits::value_type_t<Point>;

  auto const normProduct = norm(lhs) * norm(rhs);

  if constexpr (std::is_integral_v<value_type>) {
    if (normProduct == value_type()) {
      throw std::invalid_argument("unable to calculate angle for zero-length vector");
    }
  }  
  
  return std::acos(dot_product(lhs, rhs) / normProduct);
}

} // namespace geo

#endif  
