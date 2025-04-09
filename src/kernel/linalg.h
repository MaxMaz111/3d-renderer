#pragma once

#include <Eigen/Dense>
#include <Eigen/Geometry>
#include <type_traits>

namespace renderer {

using Scalar = double;
static constexpr Scalar kEpsilon = 1e-9;
static_assert(std::is_same_v<Scalar, double> || std::is_same_v<Scalar, float>);
using Vector3 = Eigen::Vector3<Scalar>;
using Vector4 = Eigen::Vector4<Scalar>;
using Point3 = Vector3;
using Point4 = Vector4;
using Matrix3 = Eigen::Matrix3<Scalar>;
using Matrix4 = Eigen::Matrix4<Scalar>;
using AngleAxis = Eigen::AngleAxis<Scalar>;
using Index = Eigen::Index;
// using Plane3 = Eigen::Hyperplane<Scalar, 3>;
// ^ doesn't work due to non-consistency of normal direction

}  // namespace renderer
