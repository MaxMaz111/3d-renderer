#pragma once

#include <QPixmap>

#include "frame.h"
#include "scene.h"
#include "triangle.h"
#include "z_buffer.h"

namespace renderer::kernel {

class Renderer {
 public:
  Frame Render(const Scene& scene);

 private:
  std::vector<Triangle> GetClippedTriangles(
      const std::vector<Triangle>& triangles, const Camera& camera) const;
  std::vector<Triangle> ClipTriangleByPlane(const Triangle& triangle,
                                            const Plane& plane) const;
  void RotateTriangles(std::vector<Triangle>& triangles, const Camera& camera);
  void ProjectTriangles(std::vector<Triangle>& triangles, const Camera& camera);

  static QColor ConvertColor(const Color& color);

  ZBuffer z_buffer_{};
};

}  // namespace renderer::kernel
