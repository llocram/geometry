#include "ut.hpp"
#include "geometry.h"

using namespace boost::ut;
using namespace geo;

int main() {

  "Vector3d norm"_test = [] {
    Vector3d vec(1.0, 1.0, 0.0);
    expect(norm(vec) == 1.41_d);
  };

  return 0;
}
