#ifndef GEO_POINT_HPP
#define GEO_POINT_HPP

#include "traits.hpp"

namespace geo {

/***************************** model ********************************/

template <concepts::arithmetic T, typename... Mixins>
struct Vector2x : public Mixins... {
  constexpr Vector2x() = default;
  constexpr Vector2x(T x, T y)
      : x(x), y(y) {};

  T x{};
  T y{};
};

template <concepts::arithmetic T, typename... Mixins>
struct Vector3x : public Mixins... {
  constexpr Vector3x() = default;
  constexpr Vector3x(T x, T y, T z)
      : x(x), y(y), z(z) {};

  T x{};
  T y{};
  T z{};
};

using Vector2d = Vector2x<double>;
using Vector3d = Vector3x<double>;

/***************************** adaptors ********************************/

namespace traits {

template <concepts::arithmetic T, typename...Mixins>
struct tag<Vector2x<T, Mixins...>> {
  using type = point_tag;
};

template <concepts::arithmetic T, typename... Mixins>
struct value_type<Vector2x<T, Mixins...>> {
  using type = T;
};

template <concepts::arithmetic T, typename...Mixins>
struct dimension<Vector2x<T, Mixins...>> {
  static constexpr std::size_t value = 2;
};

template <concepts::arithmetic T, typename... Mixins>
struct access<Vector2x<T, Mixins...>, 0> {
  static constexpr T get(Vector2x<T, Mixins...> const & vec) { return vec.x; }
  static constexpr void set(Vector2x<T, Mixins...> & vec, T x) { vec.x = x; }
};

template <concepts::arithmetic T, typename... Mixins>
struct access<Vector2x<T, Mixins...>, 1> {
  static constexpr T get(Vector2x<T, Mixins...> const & vec) { return vec.y; }
  static constexpr void set(Vector2x<T, Mixins...> & vec, T y) { vec.y = y; }
};

template <concepts::arithmetic T, typename... Mixins>
struct tag<Vector3x<T, Mixins...>> {
  using type = point_tag;
};

template <concepts::arithmetic T, typename... Mixins>
struct value_type<Vector3x<T, Mixins...>> {
  using type = T;
};

template <concepts::arithmetic T, typename... Mixins>
struct dimension<Vector3x<T, Mixins...>> {
  static constexpr std::size_t value = 3;
};

template <concepts::arithmetic T, typename... Mixins>
struct access<Vector3x<T, Mixins...>, 0> {
  static constexpr T get(Vector3x<T, Mixins...> const & vec) { return vec.x; }
  static constexpr void set(Vector3x<T, Mixins...> & vec, T x) { vec.x = x; }
};

template <concepts::arithmetic T, typename... Mixins>
struct access<Vector3x<T, Mixins...>, 1> {
  static constexpr T get(Vector3x<T, Mixins...> const & vec) { return vec.y; }
  static constexpr void set(Vector3x<T, Mixins...> & vec, T y) { vec.y = y; }
};

template <concepts::arithmetic T, typename... Mixins>
struct access<Vector3x<T, Mixins...>, 2> {
  static constexpr T get(Vector3x<T, Mixins...> const & vec) { return vec.z; }
  static constexpr void set(Vector3x<T, Mixins...> & vec, T z) { vec.z = z; }
};

} // namespace traits

} // namespace geo

#endif
