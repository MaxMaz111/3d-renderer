#pragma once

#include "linalg.h"
#include "vertex.h"

namespace renderer::kernel {

class Plane {
 public:
  Plane(const Vector3& normal, const Point3& point);
  Plane(const Vector3& normal, Scalar distance);

  Scalar DistanceToPoint(const Point3& point) const;
  Point3 ProjectPoint(const Point3& point) const;
  Vertex LineIntersection(const Vertex& line_start,
                          const Vertex& line_end) const;
  bool IsOnTheSameSideAsNormal(const Point3& point) const;

 private:
  Vector3 normal_;
  Scalar d_;
};

}  // namespace renderer::kernel
