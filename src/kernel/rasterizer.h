#pragma once

#include "camera.h"
#include "directional_light.h"
#include "frame.h"
#include "mesh.h"
#include "z_buffer.h"

namespace renderer::kernel {

class Rasterizer {
  static constexpr Scalar kBlendFactor = 0.2f;

 public:
  Rasterizer(Width width, Height height);

  void Clear();
  void ResetTo(Width width, Height height);
  const Frame& Rasterize(std::vector<Mesh>&& meshes, const Camera& camera,
                         const std::vector<DirectionalLight>& lights);

 private:
  void Rasterize(Mesh&& mesh, const Camera& camera,
                 const std::vector<DirectionalLight>& lights);
  void Rasterize(const Triangle& triangle, const Camera& camera,
                 const std::vector<DirectionalLight>& lights,
                 const Texture& diffuse_texture);
  void UpdateZBuffer(Width x, Height y);

  ZBuffer z_buffer_;
  Frame frame_;
};

}  // namespace renderer::kernel
