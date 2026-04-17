#pragma once

#include "kernel/camera.h"
#include "kernel/shadow_map_light.h"

#include "directional_light.h"
#include "frame.h"
#include "mesh.h"
#include "rasterizer.h"
#include "scene.h"
#include "triangle.h"

namespace renderer::kernel {

class Renderer {
  static constexpr int kParallelGranularity = 4096;

 public:
  Renderer(Width width, Height height);

  void ResetTo(Width width, Height height);
  void ToggleHDR();
  const Frame& Render(const Scene& scene);

 private:
  std::vector<DirectionalLight> MoveToLocal(
      std::vector<DirectionalLight>&& lights, const Camera& camera) const;
  std::vector<ShadowMapLight> MoveToLocal(std::vector<ShadowMapLight>&& lights,
                                          const Camera& camera) const;
  const Frame& Rasterize(std::vector<Mesh>&& meshes, const Camera& camera,
                         const std::vector<DirectionalLight>& lights,
                         const std::vector<ShadowMapLight>& shadow_lights);

  Rasterizer rasterizer_;
};

}  // namespace renderer::kernel
