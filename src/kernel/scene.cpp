#include "scene.h"

#include "camera.h"
#include "directional_light.h"

namespace renderer::kernel {

Scene::Scene(std::vector<Mesh>&& meshes)
    : camera_(), meshes_(std::move(meshes)), directional_lights_{{{0, 1, 1}}} {}

Scene::Scene(CameraT&& camera, std::vector<Mesh>&& meshes)
    : camera_(std::move(camera)),
      meshes_(std::move(meshes)),
      directional_lights_{{{0, 1, 1}}} {}

const std::vector<Mesh>& Scene::Meshes() const {
  return meshes_;
}

const std::vector<DirectionalLight>& Scene::DirectionalLights() const {
  return directional_lights_;
}

void Scene::SetAspectRatio(Scalar aspect_ratio) {
  camera_.SetAspectRatio(aspect_ratio);
}

void Scene::RotateLeft() {
  Camera().RotateLeft();
}

void Scene::RotateRight() {
  Camera().RotateRight();
}

void Scene::RotateUp() {
  Camera().RotateUp();
}

void Scene::RotateDown() {
  Camera().RotateDown();
}

void Scene::MoveLeft() {
  Camera().MoveLeft();
}

void Scene::MoveRight() {
  Camera().MoveRight();
}

void Scene::MoveForward() {
  Camera().MoveForward();
}

void Scene::MoveBackward() {
  Camera().MoveBackward();
}

void Scene::SwivelLeft() {
  Camera().SwivelLeft();
}

void Scene::SwivelRight() {
  Camera().SwivelRight();
}

void Scene::SwapRenderingMode() {
  Camera().SwapRenderingMode();
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
