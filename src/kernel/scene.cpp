#include "scene.h"

#include "util/constants.h"
#include "util/size.h"

#include "camera.h"
#include "directional_light.h"

namespace renderer::kernel {

Scene::Scene(std::vector<Mesh>&& meshes,
             std::vector<DirectionalLight>&& directional_lights,
             std::vector<ShadowMapLight>&& shadow_map_lights)
    : camera_{Width{kDefaultWidth}, Height{kDefaultHeight}},
      meshes_{std::move(meshes)},
      directional_lights_{std::move(directional_lights)},
      shadow_map_lights_{std::move(shadow_map_lights)} {}

const std::vector<Mesh>& Scene::Meshes() const {
  return meshes_;
}

const std::vector<DirectionalLight>& Scene::DirectionalLights() const {
  return directional_lights_;
}

const std::vector<ShadowMapLight>& Scene::ShadowMapLights() const {
  return shadow_map_lights_;
}

const Camera& Scene::Camera() const {
  return camera_;
}

Camera& Scene::Camera() {
  return camera_;
}

Camera::RenderingMode Scene::CurrentRenderingMode() const {
  return Camera().CurrentRenderingMode();
}

void Scene::AddShadowLight() {
  shadow_map_lights_.emplace_back(Camera().Position(),
                                  Camera().RotationMatrix());
  auto meshes = Meshes();
  shadow_map_lights_.back().UpdateZBuffer(std::move(meshes));
}

}  // namespace renderer::kernel
