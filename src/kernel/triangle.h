#pragma once

#include <QColor>
#include <array>
#include <optional>

#include "util/alias.h"
#include "util/size.h"

#include "directional_light.h"
#include "light_sample.h"
#include "linalg.h"
#include "plane.h"
#include "texture.h"
#include "vertex.h"

namespace renderer::kernel {

using XAxis = util::Alias<Scalar, struct x_tag>;
using YAxis = util::Alias<Scalar, struct y_tag>;

class Triangle {
 public:
  struct BBox {
    int min_x;
    int max_x;
    int min_y;
    int max_y;
  };

  Triangle(Vertex&& v0, Vertex&& v1, Vertex&& v2);
  Triangle(const std::array<Vertex, 3>& vertices);

  const std::array<Vertex, 3>& Vertices() const;
  void RotateAndMove(const Matrix3& rotation_matrix, const Point3& translation);
  void Project(const Matrix4& projection_matrix);
  Scalar InterpolateZ(XAxis x, YAxis y) const;
  LightSample InterpolateColor(
      XAxis x, YAxis y, const std::vector<DirectionalLight>& lights,
      const std::vector<class ShadowMapLight>& shadow_lights,
      const Texture& texture) const;
  Scalar GetMinX() const;
  Scalar GetMaxX() const;
  Scalar GetMinY() const;
  Scalar GetMaxY() const;
  BBox GetBoundingBox(Width width, Height height) const;
  bool IsInside(const std::array<Plane, 6>& planes) const;

 private:
  std::optional<std::array<Scalar, 3>> Barycentric(XAxis x, YAxis y) const;
  std::optional<std::array<Scalar, 3>> PerspectiveCorrectBarycentric(
      XAxis x, YAxis y) const;
  Point2 InterpolateTexCoord(const std::array<Scalar, 3>& weights) const;
  Vector3 InterpolateNormal(const std::array<Scalar, 3>& weights) const;
  Point3 InterpolateWorldPoint(const std::array<Scalar, 3>& weights) const;
  Point2 InterpolateTexCoord(XAxis x, YAxis y) const;
  Vector3 InterpolateNormal(XAxis x, YAxis y) const;
  Point3 InterpolateWorldPoint(XAxis x, YAxis y) const;
  Point3 FromHomogeneous(const Point4& point) const;
  Point4 ToHomogeneous(const Point3& point) const;

  std::array<Vertex, 3> vertices_;
};

}  // namespace renderer::kernel
