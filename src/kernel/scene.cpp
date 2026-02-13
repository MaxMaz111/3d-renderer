#include "scene.h"

#include <vector>

#include "mesh.h"

namespace renderer::kernel {

Scene::Scene(std::vector<Mesh>&& meshes)
    : cameras_(4), cur_camera_index_(0), meshes_(std::move(meshes)) {}

Scene::Scene(std::vector<CameraT>&& cameras, std::vector<Mesh>&& meshes)
    : cameras_(std::move(cameras)),
      cur_camera_index_(0),
      meshes_(std::move(meshes)) {}

const std::vector<Mesh>& Scene::Meshes() const {
  return meshes_;
}

void Scene::SetAspectRatio(Scalar aspect_ratio) {
  for (auto& camera : cameras_) {
    camera.SetAspectRatio(aspect_ratio);
  }
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

void Scene::SetCurrentCamera(int camera_index) {
  assert(camera_index >= 0);
  if (static_cast<size_t>(camera_index) < cameras_.size()) {
    cur_camera_index_ = camera_index;
  }
}

void Scene::SwapRenderingMode() {
  Camera().SwapRenderingMode();
}

const Camera& Scene::Camera() const {
  return cameras_[cur_camera_index_];
}

Camera& Scene::Camera() {
  return cameras_[cur_camera_index_];
}

Camera::RenderingMode Scene::CurrentRenderingMode() const {
  return Camera().CurrentRenderingMode();
}

}  // namespace renderer::kernel
