#include "triangle.h"

#include <QColor>
#include <algorithm>
#include <array>
#include <cmath>

#include "util/constants.h"

#include "color.h"
#include "directional_light.h"
#include "linalg.h"

namespace renderer::kernel {

Triangle::Triangle(const std::array<Vertex, 3>& vertices)
    : vertices_(vertices) {}

const std::array<Triangle::Vertex, 3> Triangle::Vertices() const {
  return vertices_;
}

Point3& Triangle::GetPoint(int index) {
  return vertices_[index].point;
}

const Point3& Triangle::GetPoint(int index) const {
  return vertices_[index].point;
}

void Triangle::RotateAndMove(const Matrix3& rotation_matrix,
                             const Point3& translation) {
  assert(std::abs(rotation_matrix.determinant() - 1) < kEpsilon);
  assert((rotation_matrix * rotation_matrix.transpose()).isIdentity(kEpsilon));
  for (auto& vertex : vertices_) {
    vertex.point = rotation_matrix * (vertex.point + translation);
    vertex.normal = (rotation_matrix * vertex.normal).normalized();
  }
}

void Triangle::Project(const Matrix4& projection_matrix) {
  for (auto& vertex : vertices_) {
    Point4 clip = projection_matrix * ToHomogeneous(vertex.point);
    assert(std::abs(clip.w()) > kEpsilon);
    vertex.inv_w = 1.0f / clip.w();
    vertex.point = Point3(clip.x() * vertex.inv_w, clip.y() * vertex.inv_w,
                          clip.z() * vertex.inv_w);
  }
}

std::optional<std::array<Scalar, 3>> Triangle::PerspectiveCorrectBarycentric(
    XCoordinate x, YCoordinate y) const {
  auto bary = Barycentric(x, y);
  if (!bary.has_value()) {
    return std::nullopt;
  }

  const auto [alpha, beta, gamma] = *bary;
  const Scalar w0 = vertices_[0].inv_w;
  const Scalar w1 = vertices_[1].inv_w;
  const Scalar w2 = vertices_[2].inv_w;
  const Scalar denom = alpha * w0 + beta * w1 + gamma * w2;

  if (std::abs(denom) < kEpsilon) {
    return std::nullopt;
  }

  return std::array<Scalar, 3>{alpha * w0 / denom, beta * w1 / denom,
                               gamma * w2 / denom};
}

std::optional<Scalar> Triangle::InterpolateZ(XCoordinate x,
                                             YCoordinate y) const {
  auto weights = Barycentric(x, y);
  if (!weights.has_value()) {
    return std::nullopt;
  }

  const auto [alpha, beta, gamma] = *weights;
  const Point3& p0 = vertices_[0].point;
  const Point3& p1 = vertices_[1].point;
  const Point3& p2 = vertices_[2].point;
  return alpha * p0.z() + beta * p1.z() + gamma * p2.z();
}

QRgb Triangle::InterpolateColor(
    XCoordinate x, YCoordinate y,
    const std::vector<DirectionalLight>& lights) const {
  auto weights = PerspectiveCorrectBarycentric(x, y);
  if (!weights.has_value()) {
    return kBlackColor;
  }

  const auto [alpha, beta, gamma] = *weights;
  const Vector3& n0 = vertices_[0].normal;
  const Vector3& n1 = vertices_[1].normal;
  const Vector3& n2 = vertices_[2].normal;
  Vector3 normal = alpha * n0 + beta * n1 + gamma * n2;
  if (normal.norm() > kEpsilon) {
    normal.normalize();
  }
  return lights.empty()
             ? kWhiteColor
             : Color::ScaleColor(kWhiteColor,
                                 lights[0].CalculateIntensity(normal));
}

Scalar Triangle::GetMinX() const {
  return std::min(
      {vertices_[0].point.x(), vertices_[1].point.x(), vertices_[2].point.x()});
}

Scalar Triangle::GetMaxX() const {
  return std::max(
      {vertices_[0].point.x(), vertices_[1].point.x(), vertices_[2].point.x()});
}

Scalar Triangle::GetMinY() const {
  return std::min(
      {vertices_[0].point.y(), vertices_[1].point.y(), vertices_[2].point.y()});
}

Scalar Triangle::GetMaxY() const {
  return std::max(
      {vertices_[0].point.y(), vertices_[1].point.y(), vertices_[2].point.y()});
}

std::optional<std::array<Scalar, 3>> Triangle::Barycentric(
    XCoordinate x, YCoordinate y) const {
  const Point3& p0 = vertices_[0].point;
  const Point3& p1 = vertices_[1].point;
  const Point3& p2 = vertices_[2].point;

  Scalar full_area = 0.5f * ((p1.x() - p0.x()) * (p2.y() - p0.y()) -
                             (p2.x() - p0.x()) * (p1.y() - p0.y()));
  if (std::abs(full_area) < kEpsilon) {
    return std::nullopt;
  }

  Scalar alpha =
      0.5f *
      ((p1.x() - x()) * (p2.y() - y()) - (p2.x() - x()) * (p1.y() - y())) /
      full_area;
  Scalar beta = 0.5f *
                ((x() - p0.x()) * (p2.y() - p0.y()) -
                 (p2.x() - p0.x()) * (y() - p0.y())) /
                full_area;
  Scalar gamma = 1.0f - alpha - beta;

  if (alpha < -kEpsilon || beta < -kEpsilon || gamma < -kEpsilon) {
    return std::nullopt;
  }

  return std::array<Scalar, 3>{alpha, beta, gamma};
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
