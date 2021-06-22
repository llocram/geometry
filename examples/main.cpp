#include "geometry.h"

#include <iostream>

#define UNUSED(x) {(void)x;}

std::ostream &
operator<<(std::ostream & os, geo::Vector3d const & vec) {
  os << "[" << vec.x << ", " << vec.y << ", " << vec.z << "]";
  return os;
}

int main() {
  using StaticCubicBezier = geo::Bezier<3, geo::Vector3d, std::array<geo::Vector3d, 4>>;

  constexpr std::array<geo::Vector3d, 4> ctrls {
    geo::Vector3d(1.0, 0.0, 0.0),
    geo::Vector3d(1.0, 0.558, 0.0),
    geo::Vector3d(0.558, 1.0, 0.0),
    geo::Vector3d(0.0, 1.0, 0.0)
  };

  constexpr StaticCubicBezier bezier(ctrls);

  constexpr auto point = geo::evaluateAt(bezier, 0.5);

  std::cout << point << '\n';

  return 0;
}
