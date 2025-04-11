#include "triangle.h"

#include <algorithm>
#include <iostream>

#include "linalg.h"

namespace renderer {

Triangle::Triangle(const Point3& p0, const Point3& p1, const Point3& p2)
    : points_({p0, p1, p2}) {}

Triangle::Triangle(const Point3& p0, const Point3& p1, const Point3& p2,
                   const Color& color)
    : points_({p0, p1, p2}), triangle_color_(color) {}

const std::array<Point3, 3>& Triangle::GetPoints() const {
  return points_;
}

Point3 Triangle::GetPoint(size_t index) const {
  return points_[index];
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

std::optional<Scalar> Triangle::GetZ(const Point3& point) const {
  assert(point.z() == 0);
  const Point3& p0 = points_[0];
  const Point3& p1 = points_[1];
  const Point3& p2 = points_[2];
  Scalar full_area = 0.5 * ((p1.x() - p0.x()) * (p2.y() - p0.y()) -
                            (p2.x() - p0.x()) * (p1.y() - p0.y()));
  if (std::abs(full_area) < kEpsilon) {
    return std::nullopt;
  }
  Scalar alpha = 0.5 *
                 ((p1.x() - point.x()) * (p2.y() - point.y()) -
                  (p2.x() - point.x()) * (p1.y() - point.y())) /
                 full_area;
  Scalar beta = 0.5 *
                ((point.x() - p0.x()) * (p2.y() - p0.y()) -
                 (p2.x() - p0.x()) * (point.y() - p0.y())) /
                full_area;
  Scalar gamma = 1.0 - alpha - beta;
  if (alpha < -kEpsilon || beta < -kEpsilon || gamma < -kEpsilon) {
    return std::nullopt;
  }
  Scalar z = alpha * p0.z() + beta * p1.z() + gamma * p2.z();
  return z;
}

Color Triangle::GetColor() const {
  return triangle_color_;
}

void Triangle::SetColor(const Color& color) {
  triangle_color_ = color;
}

void Triangle::Print() const {
  std::cout << points_[0] << '\n';
  std::cout << points_[1] << '\n';
  std::cout << points_[2] << '\n';
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

}  // namespace renderer
