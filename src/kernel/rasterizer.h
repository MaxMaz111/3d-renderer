#pragma once

#include "camera.h"
#include "frame.h"
#include "mesh.h"
#include "z_buffer.h"

namespace renderer::kernel {

class Rasterizer {
 public:
  Rasterizer(Width width, Height height);

  void ResetTo(Width width, Height height);
  Frame Rasterize(std::vector<Mesh>&& meshes, const Camera& camera);

 private:
  void Rasterize(Mesh&& mesh, const Camera& camera);
  void Rasterize(const Triangle& triangle, const Camera& camera);
  void UpdateZBuffer(Width i, Height j, const Triangle& triangle,
                     const Camera& camera);

  static Mesh ConvertToRasterSpace(Mesh&& meshes, int width, int height);

  int width_, height_;
  ZBuffer z_buffer_;
  Frame frame_;
};

}  // namespace renderer::kernel
