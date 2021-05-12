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

template <typename T> concept arithmetic = std::integral<T> || std::floating_point<T>;

struct point_type {};
struct circle_type {};
struct line_type {};

template <typename T> struct value_type;

template <typename T, std::size_t I>
struct access {
  static typename value_type<T>::type get(T const &);
  static void set(T &, typename value_type<T>::type);
};

template <typename T> struct tag;

template <typename T, bool point = std::is_same_v<typename tag<T>::type, point_type>>
struct is_point : std::false_type {};
template <typename T> struct is_point<T, true> : std::true_type {};
template <typename T> concept point = is_point<T>::value;

template <typename T, bool circle = std::is_same_v<typename tag<T>::type, circle_type>>
struct is_circle : std::false_type {};
template <typename T> struct is_circle<T, true> : std::true_type {};
template <typename T> concept circle = is_circle<T>::value;

template <typename T, bool line = std::is_same_v<typename tag<T>::type, line_type>>
struct is_line : std::false_type {};
template <typename T> struct is_line<T, true> : std::true_type {};
template <typename T> concept line = is_line<T>::value;

template <typename T> concept geo_object = 
  is_point<T>::value || is_circle<T>::value || is_line<T>::value;

template <typename GeoObject, typename T> concept value_type_equals =
  std::is_same_v<typename value_type<GeoObject>::type, T>;

template <typename GeoObject1, typename GeoObject2> concept same_value_type =
  std::is_same_v<typename value_type<GeoObject1>::type,
                 typename value_type<GeoObject2>::type>;
}

/********************* pre-defined geometric entities *************************/

template <typename T = double, typename... Mixins>
requires traits::arithmetic<T>
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
requires traits::point<Point>
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
requires traits::point<Point> && traits::value_type_equals<Point, T>
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
requires traits::point<Point1> && traits::point<Point2>
double
distance_impl(Point1 const & lhs, Point2 const & rhs, traits::point_type, traits::point_type) {
  return norm(lhs - rhs);
}

}

template <typename T>
requires std::floating_point<T>
T
radiansToDegree(T radians) {
  return radians * T(180) / std::numbers::pi;
}

template <typename Point>
requires traits::point<Point>
typename traits::value_type<Point>::type
normSquared(Point const & point) {
  using traits::access;
  return access<Point, 0>::get(point) * access<Point, 0>::get(point)
       + access<Point, 1>::get(point) * access<Point, 1>::get(point)
       + access<Point, 2>::get(point) * access<Point, 2>::get(point);
}

template <typename Point>
requires traits::point<Point>
double
norm(Point const & point) {
  return std::sqrt(normSquared(point));
}

template <typename Point>
requires traits::point<Point>
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
requires traits::geo_object<Geo1> && traits::geo_object<Geo2>
double
distance(Geo1 const & lhs, Geo2 const & rhs) {
  return detail::distance_impl(lhs, rhs, typename traits::tag<Geo1>::type(), typename traits::tag<Geo2>::type());
}

template <typename Point>
requires traits::point<Point>
typename traits::value_type<Point>::type
dot_product(Point const & lhs, Point const & rhs) {
  using traits::access;
  return access<Point, 0>::get(lhs) * access<Point, 0>::get(rhs)
       + access<Point, 1>::get(lhs) * access<Point, 1>::get(rhs)
       + access<Point, 2>::get(lhs) * access<Point, 2>::get(rhs);
}


template <typename Point>
requires traits::point<Point>
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
  auto const lhsNorm = norm(lhs);
  auto const rhsNorm = norm(rhs);
  if (lhsNorm == zero || rhsNorm == zero) {
    throw std::invalid_argument("unable to calculate angle for zero-length vector");
  } else {
    return std::acos(dot_product(lhs, rhs) / (lhsNorm * rhsNorm));
  }
}

}


#endif
