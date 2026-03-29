#pragma once

#include <vector>

#include "kernel/shadow_map_light.h"

#include "camera.h"
#include "directional_light.h"
#include "mesh.h"

namespace renderer::kernel {

class Scene {
  using CameraT = ::renderer::kernel::Camera;
  using RenderingMode = CameraT::RenderingMode;

 public:
  Scene(std::vector<Mesh>&& meshes,
        std::vector<DirectionalLight>&& directional_lights,
        std::vector<ShadowMapLight>&& shadow_map_lights);
  const std::vector<Mesh>& Meshes() const;
  const std::vector<DirectionalLight>& DirectionalLights() const;
  const std::vector<ShadowMapLight>& ShadowMapLights() const;
  const CameraT& Camera() const;
  CameraT& Camera();
  RenderingMode CurrentRenderingMode() const;
  void AddShadowLight();

 private:
  CameraT camera_;
  std::vector<Mesh> meshes_;
  std::vector<DirectionalLight> directional_lights_;
  std::vector<ShadowMapLight> shadow_map_lights_;
};

}  // namespace renderer::kernel
