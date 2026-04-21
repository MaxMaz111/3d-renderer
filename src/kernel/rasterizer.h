#pragma once

#include "camera.h"
#include "directional_light.h"
#include "frame.h"
#include "mesh.h"
#include "tile_mutex_manager.h"
#include "z_buffer.h"

namespace renderer::kernel {

class Rasterizer {
  using BBox = Triangle::BBox;
  static constexpr Scalar kBlendFactor = 0.2;
  static constexpr Scalar kSubpixelCenterOffset = 0.5;
  static constexpr int kParallelGranularity = 4096;

 public:
  Rasterizer(Width width, Height height);

  void Clear();
  void ResetTo(Width width, Height height);
  void ToggleHDR();
  const Frame& Rasterize(std::vector<Mesh>&& meshes, const Camera& camera,
                         const std::vector<DirectionalLight>& lights,
                         const std::vector<ShadowMapLight>& shadow_lights);

 private:
  void Rasterize(Mesh&& mesh, const Camera& camera,
                 const std::vector<DirectionalLight>& lights,
                 const std::vector<ShadowMapLight>& shadow_lights);
  void Rasterize(const Triangle& triangle, const Camera& camera,
                 const std::vector<DirectionalLight>& lights,
                 const std::vector<ShadowMapLight>& shadow_lights,
                 const Texture& diffuse_texture);
  void UpdateAllSolid(Width i, Height j, const Triangle& triangle,
                      const std::vector<DirectionalLight>& lights,
                      const std::vector<ShadowMapLight>& shadow_lights,
                      const Texture& diffuse_texture);
  void UpdateAllTransparent(Width i, Height j, const Triangle& triangle,
                            const std::vector<DirectionalLight>& lights,
                            const std::vector<ShadowMapLight>& shadow_lights,
                            const Texture& diffuse_texture);

  ZBuffer z_buffer_;
  Frame frame_;
  TileMutexManager tile_mutex_manager_;
};

}  // namespace renderer::kernel
