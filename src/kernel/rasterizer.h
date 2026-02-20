#pragma once

#include "camera.h"
#include "directional_light.h"
#include "frame.h"
#include "mesh.h"
#include "z_buffer.h"

namespace renderer::kernel {

class Rasterizer {
 public:
  Rasterizer(Width width, Height height);

  void ResetTo(Width width, Height height);
  Frame Rasterize(std::vector<Mesh>&& meshes, const Camera& camera,
                  const std::vector<DirectionalLight>& lights);

 private:
  void Rasterize(Mesh&& mesh, const Camera& camera,
                 const std::vector<DirectionalLight>& lights);
  void Rasterize(const Triangle& triangle, const Camera& camera,
                 const std::vector<DirectionalLight>& lights);
  void UpdateZBuffer(Width i, Height j, const Triangle& triangle,
                     const Camera& camera,
                     const std::vector<DirectionalLight>& lights);

  static Mesh ConvertToRasterSpace(Mesh&& meshes, int width, int height);

  int width_, height_;
  ZBuffer z_buffer_;
  Frame frame_;
};

}  // namespace renderer::kernel
