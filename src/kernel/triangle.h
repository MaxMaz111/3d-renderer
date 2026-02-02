#pragma once

#include <optional>

#include "color.h"
#include "linalg.h"

namespace renderer {

namespace detail::kernel {

class XCoordinate {
  using Scalar = renderer::kernel::Scalar;

 public:
  explicit XCoordinate(Scalar value) : value_(value) {}

  operator double() const { return value_; }

 private:
  Scalar value_;
};

class YCoordinate {
  using Scalar = renderer::kernel::Scalar;

 public:
  explicit YCoordinate(Scalar value) : value_(value) {}

  operator double() const { return value_; }

 private:
  Scalar value_;
};

}  // namespace detail::kernel

namespace kernel {
class Triangle {
 public:
  using XCoordinate = detail::kernel::XCoordinate;
  using YCoordinate = detail::kernel::XCoordinate;

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

}  // namespace kernel

}  // namespace renderer
