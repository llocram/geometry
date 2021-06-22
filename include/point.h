#ifndef GEO_POINT_H
#define GEO_POINT_H

#include "traits.h"

namespace geo {

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
  constexpr Vector3x() = default;
  constexpr Vector3x(T x, T y, T z) : x(x), y(y), z(z) {};

  T x{};
  T y{};
  T z{};
};

using Vector2d = Vector2x<double>;
using Vector3d = Vector3x<double>;

namespace traits {

template <typename T, typename...Mixins>
struct tag<Vector2x<T, Mixins...>> {
  using type = point_tag;
};

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
  static constexpr T get(Vector2x<T, Mixins...> const & vec) { return vec.x; }
  static constexpr void set(Vector2x<T, Mixins...> & vec, T x) { vec.x = x; }
};

template <typename T, typename... Mixins>
struct access<Vector2x<T, Mixins...>, 1> {
  static constexpr T get(Vector2x<T, Mixins...> const & vec) { return vec.y; }
  static constexpr void set(Vector2x<T, Mixins...> & vec, T y) { vec.y = y; }
};

template <typename T, typename... Mixins>
struct tag<Vector3x<T, Mixins...>> {
  using type = point_tag;
};

template <typename T, typename... Mixins>
struct value_type<Vector3x<T, Mixins...>> {
  using type = T;
};

template <typename T, typename... Mixins>
struct dimension<Vector3x<T, Mixins...>> {
  static constexpr std::size_t value = 3;
};

template <typename T, typename... Mixins>
struct access<Vector3x<T, Mixins...>, 0> {
  static constexpr T get(Vector3x<T, Mixins...> const & vec) { return vec.x; }
  static constexpr void set(Vector3x<T, Mixins...> & vec, T x) { vec.x = x; }
};

template <typename T, typename... Mixins>
struct access<Vector3x<T, Mixins...>, 1> {
  static constexpr T get(Vector3x<T, Mixins...> const & vec) { return vec.y; }
  static constexpr void set(Vector3x<T, Mixins...> & vec, T y) { vec.y = y; }
};

template <typename T, typename... Mixins>
struct access<Vector3x<T, Mixins...>, 2> {
  static constexpr T get(Vector3x<T, Mixins...> const & vec) { return vec.z; }
  static constexpr void set(Vector3x<T, Mixins...> & vec, T z) { vec.z = z; }
};

} // namespace traits

} // namespace geo

#endif
