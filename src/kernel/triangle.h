#pragma once

#include <optional>

#include "alias.h"
#include "color.h"
#include "linalg.h"

namespace renderer::kernel {

class Triangle {
 public:
  using XCoordinate = util::Alias<Scalar, struct x_tag>;
  using YCoordinate = util::Alias<Scalar, struct y_tag>;

  Triangle(const Point3& p0, const Point3& p1, const Point3& p2,
           const Vector3& normal);

  const std::array<Point3, 3>& GetPoints() const;
  const Point3& GetPoint(size_t index) const;
  Point3& GetPoint(size_t index);
  const Vector3& GetNormal() const;
  void RotateAndMove(const Matrix3& rotation_matrix, const Point3& translation);
  void Project(const Matrix4& projection_matrix);
  std::optional<Scalar> InterpolateZ(XCoordinate x, YCoordinate y) const;
  Color InterpolateColor(XCoordinate x, YCoordinate y) const;
  Scalar GetMinX() const;
  Scalar GetMaxX() const;
  Scalar GetMinY() const;
  Scalar GetMaxY() const;

 private:
  Point3 FromHomogeneous(const Point4& point) const;
  Point4 ToHomogeneous(const Point3& point) const;

  std::array<Point3, 3> points_;
  Vector3 normal_;
};

}  // namespace renderer::kernel
