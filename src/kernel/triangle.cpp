#include "triangle.h"

#include <algorithm>

#include "linalg.h"

namespace renderer::kernel {

Triangle::Triangle(const Point3& p0, const Point3& p1, const Point3& p2,
                   const Vector3& normal)
    : points_({p0, p1, p2}), normal_(normal) {}

const std::array<Point3, 3>& Triangle::GetPoints() const {
  return points_;
}

const Point3& Triangle::GetPoint(size_t index) const {
  return points_[index];
}

Point3& Triangle::GetPoint(size_t index) {
  return points_[index];
}

const Vector3& Triangle::GetNormal() const {
  return normal_;
}

void Triangle::RotateAndMove(const Matrix3& rotation_matrix,
                             const Point3& translation) {
  assert((rotation_matrix.determinant() - 1) < kEpsilon);
  assert((rotation_matrix * rotation_matrix.transpose()).isIdentity(kEpsilon));
  for (auto& point : points_) {
    point = rotation_matrix * (point + translation);
  }
}

void Triangle::Project(const Matrix4& projection_matrix) {
  for (auto& point : points_) {
    point = FromHomogeneous(projection_matrix * ToHomogeneous(point));
  }
}

std::optional<Scalar> Triangle::InterpolateZ(XCoordinate x,
                                             YCoordinate y) const {
  const Point3& p0 = points_[0];
  const Point3& p1 = points_[1];
  const Point3& p2 = points_[2];
  Scalar full_area = 0.5 * ((p1.x() - p0.x()) * (p2.y() - p0.y()) -
                            (p2.x() - p0.x()) * (p1.y() - p0.y()));
  if (std::abs(full_area) < kEpsilon) {
    return std::nullopt;
  }
  Scalar alpha = 0.5 *
                 ((p1.x() - x) * (p2.y() - y) - (p2.x() - x) * (p1.y() - y)) /
                 full_area;
  Scalar beta =
      0.5 *
      ((x - p0.x()) * (p2.y() - p0.y()) - (p2.x() - p0.x()) * (y - p0.y())) /
      full_area;
  Scalar gamma = 1.0 - alpha - beta;
  if (alpha < -kEpsilon || beta < -kEpsilon || gamma < -kEpsilon) {
    return std::nullopt;
  }
  Scalar z = alpha * p0.z() + beta * p1.z() + gamma * p2.z();
  return z;
}

Color Triangle::InterpolateColor(XCoordinate x, YCoordinate y) const {
  return Color().Invert();
}

Scalar Triangle::GetMinX() const {
  return std::min({points_[0].x(), points_[1].x(), points_[2].x()});
}

Scalar Triangle::GetMaxX() const {
  return std::max({points_[0].x(), points_[1].x(), points_[2].x()});
}

Scalar Triangle::GetMinY() const {
  return std::min({points_[0].y(), points_[1].y(), points_[2].y()});
}

Scalar Triangle::GetMaxY() const {
  return std::max({points_[0].y(), points_[1].y(), points_[2].y()});
}

Point3 Triangle::FromHomogeneous(const Point4& point) const {
  assert(abs(point.w()) > kEpsilon);
  return Point3(point.x() / point.w(), point.y() / point.w(),
                point.z() / point.w());
}

Point4 Triangle::ToHomogeneous(const Point3& point) const {
  return Point4(point.x(), point.y(), point.z(), 1);
}

}  // namespace renderer::kernel
