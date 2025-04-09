#pragma once

#include <optional>

#include "color.h"
#include "linalg.h"

namespace renderer {

class Triangle {
 public:
  Triangle(const Point3& p0, const Point3& p1, const Point3& p2);
  const std::array<Point3, 3>& GetPoints() const;
  Point3 GetPoint(size_t index) const;
  Triangle GetRotatedTriangle(const Matrix3& rotation_matrix) const;
  Triangle GetProjectedTriangle(const Matrix4& projection_matrix) const;
  std::optional<Scalar> GetZ(const Point3& point) const;
  Color GetColor() const;
  void SetColor(const Color& color);
  void Print() const;
  void AddVector();

 private:
  Point3 FromHomogeneous(const Point4& point) const;
  Point4 ToHomogeneous(const Point3& point) const;

  std::array<Point3, 3> points_;

  Color triangle_color_ = Color::GetRandomColor();
};

}  // namespace renderer
