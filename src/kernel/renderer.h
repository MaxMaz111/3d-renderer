#pragma once

#include <QPixmap>

#include "frame.h"
#include "scene.h"
#include "triangle.h"

namespace renderer {

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

  std::vector<std::vector<Scalar>> z_buffer_;
};

}  // namespace renderer
