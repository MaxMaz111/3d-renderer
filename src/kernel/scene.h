#pragma once

#include <vector>

#include "../size.h"
#include "camera.h"
#include "triangle.h"

namespace renderer {

class Scene {
 public:
  Scene() = delete;
  Scene(const std::vector<Triangle>& triangles);
  Scene(const std::vector<Camera>& cameras,
        const std::vector<Triangle>& triangles);
  const std::vector<Triangle>& GetTriangles() const;
  void SetScreenDimensions(Width width, Height height);
  const Camera& GetCamera() const;

 private:
  std::vector<Camera> cameras_;
  ssize_t cur_camera_index_;
  std::vector<Triangle> triangles_;
};

}  // namespace renderer
