#include "geometry.h"
#include "ut.hpp"

#include <tuple>

using namespace boost::ut;

int main() {
  "radians to degree (double)"_test = [](const auto & args){
    const auto & [radians, degree] = args;
    constexpr auto epsilon = 2 * std::numeric_limits<double>::epsilon();
    expect(geo::radiansToDegree(radians) - degree < epsilon);
  } | std::vector<std::pair<double, double>>{
    {0.0, 0.0},
    {std::numbers::pi / 2.0, 90.0},
    {std::numbers::pi, 180.0},
    {std::numbers::pi * 2.0, 360.0},
    {std::numbers::pi * 3.0, 540.0},
  };

  "radians to degree (float)"_test = [](const auto & args){
    const auto & [radians, degree] = args;
    constexpr auto epsilon = 2 * std::numeric_limits<float>::epsilon();
    expect(geo::radiansToDegree(radians) - degree < epsilon);
  } | std::vector<std::pair<float, float>>{
    {0.0f, 0.0f},
    {std::numbers::pi_v<float> / 2.0f, 90.0f},
    {std::numbers::pi_v<float>, 180.0f},
    {std::numbers::pi_v<float> * 2.0f, 360.0f},
    {std::numbers::pi_v<float> * 3.0f, 540.0f},
  };

  "degree to radians (double)"_test = [](const auto & args){
    const auto & [degree, radians] = args;
    constexpr auto epsilon = 2 * std::numeric_limits<double>::epsilon();
    expect(geo::degreeToRadians(degree) - radians < epsilon);
  } | std::vector<std::pair<double, double>>{
    {0.0, 0.0},
    {90.0, std::numbers::pi / 2.0},
    {180.0, std::numbers::pi},
    {360.0, std::numbers::pi * 2.0},
    {540.0, std::numbers::pi * 3.0},
  };

  "degree to radians (float)"_test = [](const auto & args){
    const auto & [degree, radians] = args;
    constexpr auto epsilon = 2 * std::numeric_limits<float>::epsilon();
    expect(geo::degreeToRadians(degree) - radians < epsilon);
  } | std::vector<std::pair<float, float>>{
    {0.0f, 0.0f},
    {90.0f, std::numbers::pi_v<float> / 2.0f},
    {180.0f, std::numbers::pi_v<float>},
    {360.0f, std::numbers::pi_v<float> * 2.0f},
    {540.0f, std::numbers::pi_v<float> * 3.0f},
  };

  "degree to radians (int)"_test = [](const auto & args){
    const auto & [degree, radians] = args;
    constexpr auto epsilon = 2 * std::numeric_limits<double>::epsilon();
    expect(geo::degreeToRadians(degree) - radians < epsilon);
  } | std::vector<std::pair<int, double>>{
    {0, 0.0},
    {90, std::numbers::pi / 2.0},
    {180, std::numbers::pi},
    {360, std::numbers::pi * 2.0},
    {540, std::numbers::pi * 3.0},
  };

  "sqrt"_test = [](const auto & number){
    constexpr auto epsilon = 2 * std::numeric_limits<double>::epsilon();
    expect(geo::sqrt(number) - std::sqrt(number) < epsilon);
  } | std::vector<double>{
    0.0,
    1.0,
    13.45,
    2343.424,
    2346872.245324
  };

  "dot_product Vector3d"_test = [](const auto & args){
    const auto & [lhs, rhs, result] = args;
    constexpr auto epsilon = 10 * std::numeric_limits<double>::epsilon();
    expect(geo::dot_product(lhs, rhs) - result < epsilon);
  } | std::vector<std::tuple<geo::Vector3d, geo::Vector3d, double>>{
    {geo::Vector3d(0.0, 0.0, 0.0), geo::Vector3d(0.0, 0.0, 0.0), 0.0},
    {geo::Vector3d(1.0, 0.0, 0.0), geo::Vector3d(0.0, 1.0, 1.0), 0.0},
    {geo::Vector3d(1.0, 2.0, 3.0), geo::Vector3d(4.0, 5.0, 6.0), 32.0}
  };

  "norm Vector3d"_test = [](const auto & args){
    const auto & [data, result] = args;
    constexpr auto epsilon = 10 * std::numeric_limits<double>::epsilon();
    expect(geo::norm(data) - result < epsilon);
  } | std::vector<std::pair<geo::Vector3d, double>>{
    {geo::Vector3d(0.0, 0.0, 0.0), 0.0},
    {geo::Vector3d(1.0, 0.0, 0.0), geo::sqrt(1)},
    {geo::Vector3d(0.0, 1.0, 0.0), geo::sqrt(1)},
    {geo::Vector3d(0.0, 0.0, 1.0), geo::sqrt(1)},
    {geo::Vector3d(1.0, 1.0, 0.0), geo::sqrt(2)},
    {geo::Vector3d(1.0, 2.0, 3.0), geo::sqrt(14)}
  };

  "operator- Vector3d"_test = [](const auto & args){
    const auto & [lhs, rhs, result] = args;
    constexpr auto epsilon = 10 * std::numeric_limits<double>::epsilon();
    expect((lhs - rhs).x - result.x < epsilon and
           (lhs - rhs).y - result.y < epsilon and
           (lhs - rhs).z - result.z < epsilon);
  } | std::vector<std::tuple<geo::Vector3d, geo::Vector3d, geo::Vector3d>>{
    {geo::Vector3d(0.0, 0.0, 0.0), geo::Vector3d(0.0, 0.0, 0.0), geo::Vector3d(0.0, 0.0, 0.0)},
    {geo::Vector3d(1.1, 2.2, 3.3), geo::Vector3d(4.4, 5.5, 6.6), geo::Vector3d(-3.3, -3.3, -3.3)}
  };

  "distance Vector3d"_test = [](const auto & args){
    const auto & [lhs, rhs, result] = args;
    constexpr auto epsilon = 10 * std::numeric_limits<double>::epsilon();
    expect(geo::distance(lhs, rhs) - result < epsilon);
  } | std::vector<std::tuple<geo::Vector3d, geo::Vector3d, double>>{
    {geo::Vector3d(0.0, 0.0, 0.0), geo::Vector3d(0.0, 0.0, 0.0), 0.0},
    {geo::Vector3d(1.0, 0.0, 0.0), geo::Vector3d(0.0, 1.0, 1.0), geo::sqrt(3)},
    {geo::Vector3d(1.0, 2.0, 3.0), geo::Vector3d(4.0, 5.0, 6.0), geo::sqrt(27)}
  };

  "operator+ Vector3d"_test = [](const auto & args){
    const auto & [lhs, rhs, result] = args;
    constexpr auto epsilon = 10 * std::numeric_limits<double>::epsilon();
    expect(geo::distance(lhs + rhs, result) < epsilon);
  } | std::vector<std::tuple<geo::Vector3d, geo::Vector3d, geo::Vector3d>>{
    {geo::Vector3d(0.0, 0.0, 0.0), geo::Vector3d(0.0, 0.0, 0.0), geo::Vector3d(0.0, 0.0, 0.0)},
    {geo::Vector3d(1.1, 2.2, 3.3), geo::Vector3d(4.4, 5.5, 6.6), geo::Vector3d(5.5, 7.7, 9.9)},
    {geo::Vector3d(-1.1, -2.2, -3.3), geo::Vector3d(4.4, 5.5, 6.6), geo::Vector3d(3.3, 3.3, 3.3)}
  };

  "operator* Vector3d"_test = [](const auto & args){
    const auto & [vec, scalar, result] = args;
    constexpr auto epsilon = 10 * std::numeric_limits<double>::epsilon();
    expect(geo::distance(vec * scalar, result) < epsilon and
           geo::distance(scalar * vec, result) < epsilon);
  } | std::vector<std::tuple<geo::Vector3d, double, geo::Vector3d>>{
    {geo::Vector3d(0.0, 0.0, 0.0), 0.0, geo::Vector3d(0.0, 0.0, 0.0)},
    {geo::Vector3d(1.1, 2.2, 3.3), 0.0, geo::Vector3d(0.0, 0.0, 0.0)},
    {geo::Vector3d(1.1, 2.2, 3.3), 1.0, geo::Vector3d(1.1, 2.2, 3.3)},
    {geo::Vector3d(1.1, 2.2, 3.3), -1.0, geo::Vector3d(-1.1, -2.2, -3.3)},
    {geo::Vector3d(1.1, 2.2, 3.3), 2.0, geo::Vector3d(2.2, 4.4, 6.6)}
  };

 "operator/ Vector3d"_test = [](const auto & args){
    const auto & [vec, scalar, result] = args;
    constexpr auto epsilon = 10 * std::numeric_limits<double>::epsilon();
    expect(geo::distance(vec / scalar, result) < epsilon);
  } | std::vector<std::tuple<geo::Vector3d, double, geo::Vector3d>>{
    {geo::Vector3d(1.1, 2.2, 3.3), 1.0, geo::Vector3d(1.1, 2.2, 3.3)},
    {geo::Vector3d(1.1, 2.2, 3.3), -1.0, geo::Vector3d(-1.1, -2.2, -3.3)},
    {geo::Vector3d(2.2, 4.4, 6.6), 2.0, geo::Vector3d(1.1, 2.2, 3.3)}
  };

  "area() Circle"_test = [](const auto & args){
    const auto & [circle, result] = args;
    constexpr auto epsilon = 10 * std::numeric_limits<double>::epsilon();
    expect(geo::area(circle) - result < epsilon);
  } | std::vector<std::pair<geo::Circle<geo::Vector3d>, double>>{
    {geo::Circle<geo::Vector3d>(geo::Vector3d(0.0, 0.0, 0.0), 1.0), 1.0 * std::numbers::pi},
    {geo::Circle<geo::Vector3d>(geo::Vector3d(1.0, 2.0, 3.0), 4.0), 16.0 * std::numbers::pi}
  };

  return 0;
}
