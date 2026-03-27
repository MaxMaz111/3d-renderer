#include "scene.h"

#include "util/constants.h"
#include "util/size.h"

#include "camera.h"
#include "directional_light.h"

namespace renderer::kernel {

Scene::Scene(std::vector<Mesh>&& meshes)
    : Scene(std::move(meshes), std::vector<DirectionalLight>{}) {}

Scene::Scene(std::vector<Mesh>&& meshes,
             std::vector<DirectionalLight>&& directional_lights)
    : camera_{Width{kDefaultWidth}, Height{kDefaultHeight}},
      meshes_{std::move(meshes)},
      directional_lights_{std::move(directional_lights)} {}

Scene::Scene(CameraT&& camera, std::vector<Mesh>&& meshes)
    : camera_(std::move(camera)),
      meshes_(std::move(meshes)),
      directional_lights_{} {}

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
