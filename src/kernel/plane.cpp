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

Vertex Plane::LineIntersection(const Vertex& line_start,
                               const Vertex& line_end) const {
  Vector3 line_direction = line_end.point - line_start.point;
  Scalar denominator = normal_.dot(line_direction);
  Scalar t = -(normal_.dot(line_start.point) + d_) / denominator;
  Vector3 interpolated_normal =
      (line_start.normal + t * (line_end.normal - line_start.normal))
          .normalized();
  Point2 interpolated_tex_coord =
      line_start.tex_coord + t * (line_end.tex_coord - line_start.tex_coord);
  Vertex vertex{.point = line_start.point + t * line_direction,
                .normal = interpolated_normal,
                .tex_coord = interpolated_tex_coord};
  return vertex;
}

bool Plane::IsOnTheSameSideAsNormal(const Point3& point) const {
  return DistanceToPoint(point) > 0;
}

}  // namespace renderer::kernel
