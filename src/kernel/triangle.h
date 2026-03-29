#pragma once

#include <QColor>
#include <array>
#include <optional>

#include "kernel/z_buffer.h"

#include "util/alias.h"

#include "directional_light.h"
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
    int16_t min_x;
    int16_t max_x;
    int16_t min_y;
    int16_t max_y;
  };

  Triangle(Vertex&& v0, Vertex&& v1, Vertex&& v2);
  Triangle(const std::array<Vertex, 3>& vertices);

  const std::array<Vertex, 3>& Vertices() const;
  void RotateAndMove(const Matrix3& rotation_matrix, const Point3& translation);
  void Project(const Matrix4& projection_matrix);
  Scalar InterpolateZ(XAxis x, YAxis y) const;
  QRgb InterpolateColor(XAxis x, YAxis y,
                        const std::vector<DirectionalLight>& lights,
                        const std::vector<class ShadowMapLight>& shadow_lights,
                        const Texture& texture) const;
  Scalar GetMinX() const;
  Scalar GetMaxX() const;
  Scalar GetMinY() const;
  Scalar GetMaxY() const;
  BBox GetBoundingBox(const ZBuffer& z_buffer) const;
  bool IsInside(const std::array<Plane, 6>& planes) const;

 private:
  std::optional<std::array<Scalar, 3>> Barycentric(XAxis x, YAxis y) const;
  std::optional<std::array<Scalar, 3>> PerspectiveCorrectBarycentric(
      XAxis x, YAxis y) const;
  Point2 InterpolateTexCoord(XAxis x, YAxis y) const;
  Vector3 InterpolateNormal(XAxis x, YAxis y) const;
  Point3 InterpolateWorldPoint(XAxis x, YAxis y) const;
  Point3 FromHomogeneous(const Point4& point) const;
  Point4 ToHomogeneous(const Point3& point) const;

  std::array<Vertex, 3> vertices_;
};

}  // namespace renderer::kernel
