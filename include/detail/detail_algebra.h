#ifndef GEO_DETAIL_ALGEBRA_H
#define GEO_DETAIL_ALGEBRA_H

#include <cmath>
#include <numbers>
#include <stdexcept>
#include <utility>

#include "traits.h"
#include "point.h"

namespace geo {

namespace detail {

template <typename Point1, typename Point2>
requires concepts::point<Point1>
      && concepts::point<Point2>
      && concepts::same_value_type<Point1, Point2>
      && concepts::same_dimension<Point1, Point2>
constexpr auto
distance_impl(
    Point1 const & lhs, Point2 const & rhs,
    traits::point_tag, traits::point_tag) noexcept {
  return norm(lhs - rhs);
}

template <typename Point, std::size_t... I>
requires concepts::point<Point>
constexpr Point
substract_impl(Point const & lhs, Point const & rhs, std::index_sequence<I...> const &) noexcept {
  Point retval;
  (..., set<Point, I>(retval, get<Point, I>(lhs) - get<Point, I>(rhs)));
  return retval;
}

template <typename Point, std::size_t... I>
requires concepts::point<Point>
constexpr Point
addition_impl(Point const & lhs, Point const & rhs, std::index_sequence<I...> const &) noexcept {
  Point retval;
  (..., set<Point, I>(retval, get<Point, I>(lhs) + get<Point, I>(rhs)));
  return retval;
}

template <typename Point, typename T, std::size_t... I>
requires concepts::point<Point>
      && concepts::value_type_equals<Point, T>
constexpr Point
multiply_impl(Point const & lhs, T scalar, std::index_sequence<I...> const &) noexcept {
  Point retval;
  (..., set<Point, I>(retval, get<Point, I>(lhs) * scalar));
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
  Point retval;
  (..., set<Point, I>(retval, get<Point, I>(lhs) / scalar));
  return retval;
}

template <typename Point1, typename Point2, std::size_t... I>
requires concepts::point<Point1>
      && concepts::point<Point2>
      && concepts::same_value_type<Point1, Point2>
      && concepts::same_dimension<Point1, Point2>
typename traits::value_type<Point1>::type
dot_product_impl(
    Point1 const & lhs, Point2 const & rhs, std::index_sequence<I...> const &) noexcept {
  return (... + (get<Point1, I>(lhs) * get<Point2, I>(rhs)));
}

} // namespace detail

} // namespace geo

#endif
