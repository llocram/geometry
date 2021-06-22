#include "../../include/geometry.h"

using CubicBezier = geo::Bezier<3, geo::Vector3d, std::array<geo::Vector3d, 4>>;

/* validate that the algorithms is SSE-vectorized */
geo::Vector3d
evaluate(CubicBezier const & bezier, double t) {
  return geo::evaluateAt(bezier, t);
}
