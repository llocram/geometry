#include "geometry.h"

#include <iostream>

constexpr bool
is_point_on_circle(geo::Vector3d const & point) noexcept {
  /* less than 3 % error for approximation of circular arcs with cubic beziers */
  return geo::norm(point) - 1 < 3e-2;
}

int main() {
  /* approximate circular arc segment with cubic bezier */
  constexpr std::array<geo::Vector3d, 4> ctrls {
    geo::Vector3d(1.0, 0.0, 0.0),
    geo::Vector3d(1.0, 0.558, 0.0),
    geo::Vector3d(0.558, 1.0, 0.0),
    geo::Vector3d(0.0, 1.0, 0.0)
  };

  using StaticCubicBezier = geo::Bezier<
  3,                                /* bezier degree */
    geo::Vector3d,                  /* point type */
    std::array<geo::Vector3d, 4>    /* container type */
  >;
  constexpr StaticCubicBezier sbezier(ctrls);
  constexpr geo::Vector3d spoint = geo::evaluateAt(sbezier, 0.5);
  static_assert(is_point_on_circle(spoint));

  /* use std::vector as default container. Algorithms are not constexpr anymore */
  using CubicBezier = geo::Bezier<
    3,                              /* bezier degree */
    geo::Vector3d                   /* point type */
  >;
  CubicBezier bezier(ctrls.cbegin(), ctrls.cend());
  geo::Vector3d const point = geo::evaluateAt(bezier, 0.5);
  std::cout << std::boolalpha << is_point_on_circle(point) << '\n';

  return 0;
}
