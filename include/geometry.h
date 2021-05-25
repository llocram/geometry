#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <cmath>
#include <concepts>
#include <cstdint>
#include <stdexcept>
#include <numbers>

namespace geo {

/**************************** traits & concepts *******************************/

namespace traits {

struct point_type {};
struct circle_type {};
struct line_type {};

template <typename T>
struct value_type;

template <typename T, std::size_t I>
struct access {
  static typename value_type<T>::type get(T const &);
  static void set(T &, typename value_type<T>::type);
};

template <typename T>
struct tag;

template <typename T, bool point = std::is_same_v<typename tag<T>::type, point_type>>
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

template <typename T>
concept point = geo::traits::is_point<T>::value;

template <typename T>
concept circle = geo::traits::is_circle<T>::value;

template <typename T>
concept line = geo::traits::is_line<T>::value;

template <typename T>
concept geo_object = 
  geo::traits::is_point<T>::value 
  || geo::traits::is_circle<T>::value
  || geo::traits::is_line<T>::value;

template <typename GeoObject, typename T>
concept value_type_equals =
  std::is_same_v<typename geo::traits::value_type<GeoObject>::type, T>;

template <typename GeoObject1, typename GeoObject2>
concept same_value_type =
  std::is_same_v<typename geo::traits::value_type<GeoObject1>::type,
                 typename geo::traits::value_type<GeoObject2>::type>;

} // namespace concepts

/********************* pre-defined geometric entities *************************/

template <typename T = double, typename... Mixins>
requires concepts::arithmetic<T>
struct Vector3d : public Mixins... {
  Vector3d() = default;
  Vector3d(T x, T y, T z) : x(x), y(y), z(z) {};

  T x{};
  T y{};
  T z{};
};

namespace traits {

template <typename T, typename...Mixins>
struct tag<Vector3d<T, Mixins...>> { using type = point_type; };

template <typename T, typename... Mixins>
struct value_type<Vector3d<T, Mixins...>> { using type = T; };

template <typename T, typename... Mixins>
struct access<Vector3d<T, Mixins...>, 0> {
  static T get(Vector3d<T, Mixins...> const & vec) { return vec.x; }
  static void set(Vector3d<T, Mixins...> & vec, T x) { vec.x = x; }
};

template <typename T, typename... Mixins>
struct access<Vector3d<T, Mixins...>, 1> {
  static T get(Vector3d<T, Mixins...> const & vec) { return vec.y; }
  static void set(Vector3d<T, Mixins...> & vec, T y) { vec.y = y; }
};

template <typename T, typename... Mixins>
struct access<Vector3d<T, Mixins...>, 2> {
  static T get(Vector3d<T, Mixins...> const & vec) { return vec.z; }
  static void set(Vector3d<T, Mixins...> & vec, T z) { vec.z = z; }
};
}

template <typename Point>
requires concepts::point<Point>
struct Line {
  Line() = default;
  Line(Point const & start, Point const & end) : start(start), end(end) {}

  Point start{};
  Point end{};
};

namespace traits {

template <typename Point> 
struct tag<Line<Point>> { using type = line_type; };
}

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

template <typename Point, typename T> 
struct tag<Circle<T, Point>> { using type = circle_type; };
}

/******************************* Algorithm ************************************/

namespace detail {

template <typename Point1, typename Point2>
requires concepts::point<Point1>
  && concepts::point<Point2>
  && concepts::same_value_type<Point1, Point2>
typename traits::value_type<Point1>::type
distance_impl(
    Point1 const & lhs, Point2 const & rhs,
    traits::point_type, traits::point_type) noexcept {
  return norm(lhs - rhs);
}

template <typename T>
requires concepts::arithmetic<T>
constexpr T
sqrtNewtonRaphson(T x, T curr, T prev) noexcept {
  return curr == prev
    ? curr
    : sqrtNewtonRaphson(x, T(0.5) * (curr + x / curr), curr);
}

} // namespace detail

template <typename T>
requires std::floating_point<T>
constexpr T
radiansToDegree(T radians) noexcept {
  return radians * T(180) / std::numbers::pi;
}

template <typename T>
constexpr std::enable_if_t<std::is_floating_point<T>::value, T>
sqrt(T x) noexcept {
  return x >= T() && x < std::numeric_limits<T>::infinity()
    ? detail::sqrtNewtonRaphson(x, x, T())
    : std::numeric_limits<T>::quiet_NaN();
}

template <typename T>
constexpr std::enable_if_t<std::is_integral<T>::value, double>
sqrt(T x) noexcept {
  return x >= 0 && x < std::numeric_limits<double>::infinity()
    ? detail::sqrtNewtonRaphson(static_cast<double>(x), static_cast<double>(x), 0.0)
    : std::numeric_limits<double>::quiet_NaN();
}

template <typename Point>
requires concepts::point<Point>
constexpr typename traits::value_type<Point>::type
normSquared(Point const & point) {
  using traits::access;
  return access<Point, 0>::get(point) * access<Point, 0>::get(point)
       + access<Point, 1>::get(point) * access<Point, 1>::get(point)
       + access<Point, 2>::get(point) * access<Point, 2>::get(point);
}

template <typename Point>
requires concepts::point<Point>
constexpr traits::value_type<Point>::type
norm(Point const & point) {
  return sqrt(normSquared(point));
}

template <typename Point>
requires concepts::point<Point>
Point
operator-(Point const & lhs, Point const & rhs) {
  using traits::access;
  Point retval;
  access<Point, 0>::set(retval, access<Point, 0>::get(lhs) - access<Point, 0>::get(rhs));
  access<Point, 1>::set(retval, access<Point, 1>::get(lhs) - access<Point, 1>::get(rhs));
  access<Point, 2>::set(retval, access<Point, 2>::get(lhs) - access<Point, 2>::get(rhs));
  return retval;
}

template <typename Geo1, typename Geo2>
requires concepts::geo_object<Geo1>
  && concepts::geo_object<Geo2>
  && concepts::same_value_type<Geo1, Geo2>
double
distance(Geo1 const & lhs, Geo2 const & rhs) {
  return detail::distance_impl(
    lhs, rhs,
    typename traits::tag<Geo1>::type(), typename traits::tag<Geo2>::type());
}

template <typename Point>
requires concepts::point<Point>
typename traits::value_type<Point>::type
dot_product(Point const & lhs, Point const & rhs) {
  using traits::access;
  return access<Point, 0>::get(lhs) * access<Point, 0>::get(rhs)
       + access<Point, 1>::get(lhs) * access<Point, 1>::get(rhs)
       + access<Point, 2>::get(lhs) * access<Point, 2>::get(rhs);
}

template <typename Point>
requires concepts::point<Point>
Point
vector_product(Point const & lhs, Point const & rhs) {
  using traits::access;
  return T(access<Point, 2>::get(lhs) * access<Point, 3>::get(rhs) - access<Point, 3>::get(lhs) * access<Point, 2>::get(rhs),
           access<Point, 3>::get(lhs) * access<Point, 1>::get(rhs) - access<Point, 1>::get(lhs) * access<Point, 3>::get(rhs),
           access<Point, 2>::get(lhs) * access<Point, 3>::get(rhs) - access<Point, 3>::get(rhs) * access<Point, 2>::get(rhs));
}

template <typename Point>
double
angle(Point const & lhs, Point const & rhs) {
  constexpr auto zero = typename traits::value_type<Point>::type();
  const auto normProduct = norm(lhs) * norm(rhs);
  if (normProduct == zero) {
    throw std::invalid_argument("unable to calculate angle for zero-length vector");
  } else {
    return std::acos(dot_product(lhs, rhs) / normProduct);
  }
}

}


#endif
