#include "geometry.h"

#include <iostream>

using namespace geo;

int main() {
  Vector3d vec1(0.0, 1.0, 0.0);
  Vector3d vec2(1.0, 0.0, 0.0);

  Line<Vector3d<double>> line;
  Circle circle(Vector3d(1.0, 2.0, 3.0), 2.0);

  std::cout << radiansToDegree(angle(vec1, vec2)) << std::endl;
  std::cout << distance(vec1, vec2) << std::endl;

  return 0;
}
