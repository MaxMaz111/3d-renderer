#pragma once

#include <QImage>
#include <vector>

#include "triangle.h"

namespace renderer {
class Scene {
 public:
  QImage Render();

 private:
  const std::vector<Triangle> triangles_ = {
      {{10, 10, 10}, {10, 5, 10}, {7, 8, 13}}};
};
}  // namespace renderer
