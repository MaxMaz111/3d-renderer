#include "scene.h"

#include <vector>

#include "triangle.h"

namespace renderer {

Scene::Scene(const std::vector<Camera>& cameras,
             const std::vector<Triangle>& triangles)
    : cameras_(cameras), triangles_(triangles) {}

Scene::Scene(const std::vector<Triangle>& triangles)
    : triangles_(triangles), cameras_(1), cur_camera_index_(0) {}

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

const Camera& Scene::GetCamera() const {
  return cameras_.at(cur_camera_index_);
}

Camera& Scene::GetCamera() {
  return cameras_.at(cur_camera_index_);
}

}  // namespace renderer
