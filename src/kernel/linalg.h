#pragma once

#include <Eigen/Dense>
#include <Eigen/Geometry>
#include <type_traits>

namespace renderer {

using Scalar = double;
static_assert(std::is_same_v<Scalar, double> || std::is_same_v<Scalar, float>);
using Vector3 = std::conditional_t<std::is_same_v<Scalar, double>,
                                   Eigen::Vector3d, Eigen::Vector3f>;
using Matrix3 = std::conditional_t<std::is_same_v<Scalar, double>,
                                   Eigen::Matrix3d, Eigen::Matrix3f>;
using Point3 = Vector3;
using Index = Eigen::Index;
using Plane3 = Eigen::Hyperplane<Scalar, 3>;

static constexpr Scalar kEpsilon = 1e-9;

}  // namespace renderer
