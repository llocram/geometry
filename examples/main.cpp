#include "geometry.h"

#include <iostream>

using namespace geo;

#define UNUSED(x) {(void)x;}

int main() {
  Vector3d vec1(0.0, 1.0, 0.0);
  Vector3d vec2(1.0, 0.0, 0.0);

  Line<Vector3d> line;
  Circle circle(Vector3d(1.0, 2.0, 3.0), 2.0);

  std::cout << radiansToDegree(angle(vec1, vec2)) << std::endl;
  std::cout << distance(vec1, vec2) << std::endl;

  const auto vec3 = vec2 - vec1;
  const auto vec4 = vec3 + vec1;
  const auto vec5 = vec4 * 42.0;
  const auto vec6 = vec5 / 2.0;

  const auto vec7 = vector_product(vec3, vec4);
  
  UNUSED(vec6)
  UNUSED(vec7)

  return 0;
}
