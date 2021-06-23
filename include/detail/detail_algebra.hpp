#ifndef GEO_DETAIL_ALGEBRA_HPP
#define GEO_DETAIL_ALGEBRA_HPP

#include <cmath>
#include <numbers>
#include <stdexcept>
#include <utility>

#include "../traits.hpp"
#include "../point.hpp"

namespace geo {

namespace detail {

template <concepts::point Point1, concepts::point Point2>
requires concepts::same_value_type<Point1, Point2>
      && concepts::same_dimension<Point1, Point2>
constexpr auto
distance_impl(
    Point1 const & lhs, Point2 const & rhs,
    traits::point_tag, traits::point_tag) noexcept {
  return norm(lhs - rhs);
}

template <concepts::point Point, std::size_t... I>
constexpr Point
substract_impl(Point const & lhs, Point const & rhs, std::index_sequence<I...> const &) noexcept {
  Point retval;
  (..., set<Point, I>(retval, get<Point, I>(lhs) - get<Point, I>(rhs)));
  return retval;
}

template <concepts::point Point, std::size_t... I>
constexpr Point
addition_impl(Point const & lhs, Point const & rhs, std::index_sequence<I...> const &) noexcept {
  Point retval;
  (..., set<Point, I>(retval, get<Point, I>(lhs) + get<Point, I>(rhs)));
  return retval;
}

template <concepts::point Point, typename Scalar, std::size_t... I>
requires concepts::value_type_equals<Point, Scalar>
constexpr Point
multiply_impl(Point const & lhs, Scalar scalar, std::index_sequence<I...> const &) noexcept {
  Point retval;
  (..., set<Point, I>(retval, get<Point, I>(lhs) * scalar));
  return retval;
}

template <concepts::point Point, typename Scalar, std::size_t... I>
requires concepts::value_type_equals<Point, Scalar>
constexpr Point
division_impl(Point const & lhs, Scalar scalar, std::index_sequence<I...> const &) noexcept(std::is_floating_point_v<Scalar>) {
  if constexpr (std::is_integral_v<Scalar>) {
    if (scalar == Scalar()) {
      throw std::runtime_error("division by zero");
    }
  }
  Point retval;
  (..., set<Point, I>(retval, get<Point, I>(lhs) / scalar));
  return retval;
}

template <concepts::point Point1, concepts::point Point2, std::size_t... I>
requires concepts::same_value_type<Point1, Point2>
      && concepts::same_dimension<Point1, Point2>
constexpr traits::value_type_t<Point1>
dot_product_impl(
    Point1 const & lhs, Point2 const & rhs, std::index_sequence<I...> const &) noexcept {
  return (... + (get<Point1, I>(lhs) * get<Point2, I>(rhs)));
}

} // namespace detail

} // namespace geo

#endif
