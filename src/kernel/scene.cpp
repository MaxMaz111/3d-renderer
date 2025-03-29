#include "scene.h"

#include <vector>

namespace renderer {

Scene::Scene(const std::vector<Camera>& cameras,
             const std::vector<Triangle>& triangles,
             const std::vector<LightSource>& light_sources)
    : cameras_(cameras), triangles_(triangles), light_sources_(light_sources) {}

std::vector<Triangle> Scene::Clip() const {
  std::vector<Triangle> clipped_triangles;
  for (const Triangle& triangle : triangles_) {
    auto new_triangles = ClipTriangle(triangle);
  }
  return clipped_triangles;
}

std::vector<Triangle> Scene::ClipTriangle(const Triangle& triangle) const {
  std::vector<Triangle> clipped_triangle;

  return clipped_triangle;
}

}  // namespace renderer
