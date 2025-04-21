#include "plane.h"

namespace renderer {

Plane::Plane(const Vector3& normal, const Point3& point)
    : normal_(normal.normalized()) {
  d_ = -normal_.dot(point);
}

Plane::Plane(const Vector3& normal, Scalar distance)
    : normal_(normal.normalized()), d_(distance) {}

Scalar Plane::DistanceToPoint(const Point3& point) const {
  return normal_.dot(point) + d_;
}

Point3 Plane::ProjectPoint(const Point3& point) const {
  return point - (DistanceToPoint(point) * normal_);
}

std::optional<Vector3> Plane::LineIntersection(
    const Vector3& line_origin, const Vector3& line_direction) const {
  Scalar denominator = normal_.dot(line_direction);
  if (std::abs(denominator) <= kEpsilon) {
    return std::nullopt;
  }
  Scalar t = -(normal_.dot(line_origin) + d_) / denominator;
  return line_origin + t * line_direction;
}

bool Plane::IsOnTheSameSideAsNormal(const Point3& point) const {
  return DistanceToPoint(point) > 0;
}

}  // namespace renderer
