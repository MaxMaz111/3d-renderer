#pragma once

#include <QPixmap>

#include "frame.h"
#include "rasterizer.h"
#include "scene.h"
#include "triangle.h"

namespace renderer::kernel {

class Renderer {
 public:
  Renderer(Width width, Height height);

  Frame Render(const Scene& scene);
  void ResetTo(Width width, Height height);

 private:
  std::vector<Triangle> Clip(std::vector<Triangle>&& triangles,
                             const Camera& camera) const;
  std::vector<Triangle> ClipTriangleByPlane(const Triangle& triangle,
                                            const Plane& plane) const;
  std::vector<Triangle> Rotate(std::vector<Triangle>&& triangles,
                               const Camera& camera) const;
  std::vector<Triangle> Project(std::vector<Triangle>&& triangles,
                                const Camera& camera) const;
  Frame Rasterize(std::vector<Triangle>&& triangles, const Camera& camera);

  Rasterizer rasterizer_;
};

}  // namespace renderer::kernel
