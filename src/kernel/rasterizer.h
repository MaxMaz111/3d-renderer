#pragma once

#include "camera.h"
#include "frame.h"
#include "triangle.h"
#include "z_buffer.h"

namespace renderer::kernel {

class Rasterizer {
 public:
  Rasterizer(Width width, Height height);

  Frame Rasterize(std::vector<Triangle>&& triangles, const Camera& camera);
  void ResetTo(Width width, Height height);

 private:
  void Rasterize(const Triangle& triangle, const Camera& camera);
  void UpdateZBuffer(Width i, Height j, const Triangle& triangle,
                     const Camera& camera);

  static std::vector<Triangle> ConvertToRasterSpace(
      std::vector<Triangle>&& triangles, int width, int height);

  int width_, height_;
  ZBuffer z_buffer_;
  Frame frame_;
};

}  // namespace renderer::kernel
