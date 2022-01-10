#ifndef GEO_ALGEBRA_HPP
#define GEO_ALGEBRA_HPP

#include "detail/detail_algebra.hpp"
#include "math.hpp"
#include "point.hpp"

namespace geo {

template <concepts::point Point>
[[nodiscard]] constexpr Point
operator-(Point const & lhs, Point const & rhs) noexcept
{
  return [&]<std::size_t... Is>(std::index_sequence<Is...>)
  {
    Point retval;
    (..., set<Is>(retval, get<Is>(lhs) - get<Is>(rhs)));
    return retval;
  }(std::make_index_sequence<traits::dimension_v<Point>>{});
}

template <concepts::point Point>
[[nodiscard]] constexpr Point
operator+(Point const & lhs, Point const & rhs) noexcept
{
  return [&]<std::size_t... Is>(std::index_sequence<Is...>)
  {
    Point retval;
    (..., set<Is>(retval, get<Is>(lhs) + get<Is>(rhs)));
    return retval;
  }(std::make_index_sequence<traits::dimension_v<Point>>{});
}

template <concepts::point Point, concepts::arithmetic Scalar>
requires concepts::value_type_equals<Point, Scalar>
[[nodiscard]] constexpr Point
operator*(Point const & point, Scalar scalar) noexcept
{
  return [&]<std::size_t... Is>(std::index_sequence<Is...>)
  {
    Point retval;
    (..., set<Is>(retval, get<Is>(point) * scalar));
    return retval;
  }(std::make_index_sequence<traits::dimension_v<Point>>{});
}

template <concepts::point Point, concepts::arithmetic Scalar>
requires concepts::value_type_equals<Point, Scalar>
[[nodiscard]] constexpr Point
operator*(Scalar scalar, Point const & point) noexcept
{
  return operator*(point, scalar);
}

template <concepts::point Point, concepts::arithmetic Scalar>
requires concepts::value_type_equals<Point, Scalar>
[[nodiscard]] constexpr Point
operator/(Point const & point, Scalar scalar) noexcept(std::floating_point<Scalar>)
{
  return [&]<std::size_t... Is>(std::index_sequence<Is...>)
  {
    if constexpr (std::integral<Scalar>) {
      if (scalar == Scalar{}) {
        throw std::runtime_error("division by zero");
      }
    }
    Point retval;
    (..., set<Is>(retval, get<Is>(point) / scalar));
    return retval;
  }(std::make_index_sequence<traits::dimension_v<Point>>{});
}

template <concepts::point Point>
[[nodiscard]] constexpr traits::value_type_t<Point>
dot_product(Point const & lhs, Point const & rhs) noexcept
{
  return [&]<std::size_t... Is>(std::index_sequence<Is...>)
  {
    return (... + (get<Is>(lhs) * get<Is>(rhs)));
  }(std::make_index_sequence<traits::dimension_v<Point>>{});
}

[[nodiscard]] constexpr auto
norm(concepts::point auto const & point)
{
  return sqrt(dot_product(point, point));
}

template <concepts::geo_object Geo1, concepts::geo_object Geo2>
requires concepts::same_value_type<Geo1, Geo2>
[[nodiscard]] constexpr auto
distance(Geo1 const & lhs, Geo2 const & rhs) noexcept
{
  return detail::distance_impl(
    lhs, rhs,
    traits::tag_t<Geo1>{}, traits::tag_t<Geo2>{}
  );
}

template <concepts::point Point>
requires concepts::dimension_equals<Point, 3>
[[nodiscard]] constexpr Point
vector_product(Point const & lhs, Point const & rhs) noexcept
{
  return Point{
    get<1>(lhs) * get<2>(rhs) - get<2>(lhs) * get<1>(rhs),
    get<2>(lhs) * get<0>(rhs) - get<0>(lhs) * get<2>(rhs),
    get<0>(lhs) * get<1>(rhs) - get<1>(rhs) * get<0>(rhs)
  };
}

template <concepts::point Point>
[[nodiscard]] auto
angle(Point const & lhs, Point const & rhs) noexcept (std::floating_point<traits::value_type_t<Point>>)
{
  using value_type = traits::value_type_t<Point>;

  auto const norm_product = norm(lhs) * norm(rhs);

  if constexpr (std::is_integral_v<value_type>) {
    if (norm_product == value_type{}) {
      throw std::invalid_argument("unable to calculate angle for zero-length vector");
    }
  }

  return std::acos(dot_product(lhs, rhs) / norm_product);
}

} // namespace geo

#endif
