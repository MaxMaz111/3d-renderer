#include "triangle.h"

#include <iostream>

#include "linalg.h"

namespace renderer {

Triangle::Triangle(const Point3& p0, const Point3& p1, const Point3& p2)
    : points_({p0, p1, p2}) {}

const std::array<Point3, 3>& Triangle::GetPoints() const {
  return points_;
}

Point3 Triangle::GetPoint(size_t index) const {
  return points_[index];
}

Triangle Triangle::GetRotatedTriangle(const Matrix3& rotation_matrix) const {
  assert((rotation_matrix.determinant() - 1) < kEpsilon);
  assert((rotation_matrix * rotation_matrix.transpose()).isIdentity(kEpsilon));
  Triangle rotated_triangle(points_[0], points_[1], points_[2]);
  for (auto& point : rotated_triangle.points_) {
    point = rotation_matrix * point;
  }
  return rotated_triangle;
}

Triangle Triangle::GetProjectedTriangle(
    const Matrix4& projection_matrix) const {
  Triangle projected_triangle(points_[0], points_[1], points_[2]);
  for (auto& point : projected_triangle.points_) {
    point = FromHomogeneous(projection_matrix * ToHomogeneous(point));
  }
  return projected_triangle;
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

Point3 Triangle::FromHomogeneous(const Point4& point) const {
  assert(abs(point.w()) > kEpsilon);
  return Point3(point.x() / point.w(), point.y() / point.w(),
                point.z() / point.w());
}

Point4 Triangle::ToHomogeneous(const Point3& point) const {
  return Point4(point.x(), point.y(), point.z(), 1);
}

}  // namespace renderer
