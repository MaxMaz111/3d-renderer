#pragma once

#include <array>
#include <optional>

#include "alias.h"
#include "color.h"
#include "linalg.h"

namespace renderer::kernel {

using XCoordinate = util::Alias<Scalar, struct x_tag>;
using YCoordinate = util::Alias<Scalar, struct y_tag>;

class Triangle {
 public:
  struct Vertex {
    Point3 point;
    Vector3 normal;
    Scalar inv_w = 1.0f;
  };

  Triangle(const std::array<Vertex, 3>& vertices);

  const std::array<Vertex, 3> Vertices() const;
  Point3& GetPoint(int index);
  const Point3& GetPoint(int index) const;
  void RotateAndMove(const Matrix3& rotation_matrix, const Point3& translation);
  void Project(const Matrix4& projection_matrix);
  std::optional<std::array<Scalar, 3>> PerspectiveCorrectBarycentric(
      XCoordinate x, YCoordinate y) const;
  std::optional<Scalar> InterpolateZ(XCoordinate x, YCoordinate y) const;
  Color InterpolateColor(XCoordinate x, YCoordinate y) const;
  Scalar GetMinX() const;
  Scalar GetMaxX() const;
  Scalar GetMinY() const;
  Scalar GetMaxY() const;

 private:
  std::optional<std::array<Scalar, 3>> Barycentric(XCoordinate x,
                                                   YCoordinate y) const;
  Point3 FromHomogeneous(const Point4& point) const;
  Point4 ToHomogeneous(const Point3& point) const;

  std::array<Vertex, 3> vertices_;
};

}  // namespace renderer::kernel
