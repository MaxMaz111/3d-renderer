#pragma once

#include <optional>

#include "color.h"
#include "linalg.h"

namespace renderer {

class Triangle {
 public:
  Triangle(const Point3& p0, const Point3& p1, const Point3& p2);
  Triangle(const Point3& p0, const Point3& p1, const Point3& p2,
           const Color& color);
  const std::array<Point3, 3>& GetPoints() const;
  Point3 GetPoint(size_t index) const;
  void RotateAndMove(const Matrix3& rotation_matrix, const Point3& translation);
  void Project(const Matrix4& projection_matrix);
  std::optional<Scalar> GetZ(const Point3& point) const;
  const Color& GetColor() const;
  void SetColor(const Color& color);
  void Print() const;
  Scalar GetMinX() const;
  Scalar GetMaxX() const;
  Scalar GetMinY() const;
  Scalar GetMaxY() const;

 private:
  Point3 FromHomogeneous(const Point4& point) const;
  Point4 ToHomogeneous(const Point3& point) const;

  std::array<Point3, 3> points_;

  Color triangle_color_ = Color::GetRandomColor();
};

}  // namespace renderer
