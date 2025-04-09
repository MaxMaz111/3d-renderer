#pragma once

#include <QPixmap>

#include "scene.h"
#include "triangle.h"

namespace renderer {

class Renderer {
 public:
  QPixmap Render(const Scene& scene);

 private:
  std::vector<Triangle> GetClippedTriangles(
      const std::vector<Triangle>& triangles, const Camera& camera) const;
  std::vector<Triangle> ClipTriangle(const Triangle& triangle,
                                     const Camera& camera) const;
  std::vector<Triangle> ClipTriangleByPlane(const Triangle& triangle,
                                            const Plane& plane) const;
  std::vector<Triangle> GetRotatedTriangles(
      const std::vector<Triangle>& triangles, const Camera& camera) const;
  std::vector<Triangle> GetProjectedTriangles(
      const std::vector<Triangle>& triangles, const Camera& camera) const;
  std::vector<Point3> ClipPolygonByPlane(const std::vector<Point3>& vertices,
                                         const Plane& plane,
                                         const Color& color) const;
  std::vector<Scalar> z_buffer_;
  QImage to_return_;
};

}  // namespace renderer
