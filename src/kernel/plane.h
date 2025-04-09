#pragma once

#include <optional>

#include "linalg.h"

namespace renderer {

class Plane {
 public:
  Plane(const Vector3& normal, const Point3& point);
  Plane(const Vector3& normal, Scalar distance);
  Scalar DistanceToPoint(const Point3& point) const;
  Point3 ProjectPoint(const Point3& point) const;
  std::optional<Vector3> LineIntersection(const Vector3& line_origin,
                                          const Vector3& line_direction) const;
  bool IsOnTheSameSideAsNormal(const Point3& point) const;

 private:
  Vector3 normal_;
  Scalar d_;
};

}  // namespace renderer
