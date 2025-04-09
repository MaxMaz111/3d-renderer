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

const Camera& Scene::GetCamera() const {
  return cameras_.at(cur_camera_index_);
}

}  // namespace renderer
