#include "scene.h"

#include <vector>

#include "triangle.h"

namespace renderer {

Scene::Scene(const std::vector<Camera>& cameras,
             const std::vector<Triangle>& triangles)
    : cameras_(cameras), triangles_(triangles) {}

Scene::Scene(const std::vector<Triangle>& triangles)
    : cameras_(4), cur_camera_index_(0), triangles_(triangles) {}

const std::vector<Triangle>& Scene::GetTriangles() const {
  return triangles_;
}

void Scene::SetScreenDimensions(Width width, Height height) {
  for (auto& camera : cameras_) {
    camera.SetScreenDimensions(width, height);
  }
}

void Scene::RotateLeft() {
  GetCamera().RotateLeft();
}

void Scene::RotateRight() {
  GetCamera().RotateRight();
}

void Scene::RotateUp() {
  GetCamera().RotateUp();
}

void Scene::RotateDown() {
  GetCamera().RotateDown();
}

void Scene::MoveLeft() {
  GetCamera().MoveLeft();
}

void Scene::MoveRight() {
  GetCamera().MoveRight();
}

void Scene::MoveForward() {
  GetCamera().MoveForward();
}

void Scene::MoveBackward() {
  GetCamera().MoveBackward();
}

void Scene::SwivelLeft() {
  GetCamera().SwivelLeft();
}

void Scene::SwivelRight() {
  GetCamera().SwivelRight();
}

void Scene::SetCurrentCamera(int camera_index) {
  assert(camera_index >= 0);
  if (static_cast<size_t>(camera_index) < cameras_.size()) {
    cur_camera_index_ = camera_index;
  }
}

const Camera& Scene::GetCamera() const {
  return cameras_.at(cur_camera_index_);
}

Camera& Scene::GetCamera() {
  return cameras_.at(cur_camera_index_);
}

}  // namespace renderer
