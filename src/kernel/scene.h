#pragma once

#include <vector>

#include "camera.h"
#include "light_source.h"
#include "primitives.h"

namespace renderer {

class Scene {
 public:
  Scene() = default;
  Scene(const std::vector<Camera>& cameras,
        const std::vector<Triangle>& triangles,
        const std::vector<LightSource>& lights);

 private:
  std::vector<Camera> cameras_;
  ssize_t cur_camera_index_;
  std::vector<Triangle> triangles_;
  std::vector<LightSource> light_sources_;
};

}  // namespace renderer
