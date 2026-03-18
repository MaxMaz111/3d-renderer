#include "scene.h"

#include "camera.h"
#include "directional_light.h"

namespace renderer::kernel {

Scene::Scene(std::vector<Mesh>&& meshes)
    : camera_{}, meshes_{std::move(meshes)}, directional_lights_{1} {}

Scene::Scene(CameraT&& camera, std::vector<Mesh>&& meshes)
    : camera_{std::move(camera)},
      meshes_{std::move(meshes)},
      directional_lights_{1} {}

const std::vector<Mesh>& Scene::Meshes() const {
  return meshes_;
}

const std::vector<DirectionalLight>& Scene::DirectionalLights() const {
  return directional_lights_;
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

}  // namespace renderer::kernel
