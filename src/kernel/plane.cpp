#include "plane.h"

namespace renderer::kernel {

Plane::Plane(const Vector3& normal, const Point3& point)
    : normal_(normal.normalized()), d_(-normal_.dot(point)) {}

Plane::Plane(const Vector3& normal, Scalar distance)
    : normal_(normal.normalized()), d_(distance) {}

Scalar Plane::DistanceToPoint(const Point3& point) const {
  return normal_.dot(point) + d_;
}

Point3 Plane::ProjectPoint(const Point3& point) const {
  return point - (DistanceToPoint(point) * normal_);
}

std::optional<Triangle::Vertex> Plane::LineIntersection(
    const Triangle::Vertex& line_start,
    const Triangle::Vertex& line_end) const {
  Vector3 line_direction = line_end.point - line_start.point;
  Scalar denominator = normal_.dot(line_direction);
  if (std::abs(denominator) <= kEpsilon) {
    return std::nullopt;
  }
  Scalar t = -(normal_.dot(line_start.point) + d_) / denominator;
  if (t < -kEpsilon || t > 1.0 + kEpsilon) {
    return std::nullopt;
  }
  Vector3 interpolated_normal =
      (line_start.normal + t * (line_end.normal - line_start.normal))
          .normalized();
  Triangle::Vertex vertex{
      .point = line_start.point + t * line_direction,
      .normal = interpolated_normal,
  };
  return vertex;
}

bool Plane::IsOnTheSameSideAsNormal(const Point3& point) const {
  return DistanceToPoint(point) > 0;
}

}  // namespace renderer::kernel
