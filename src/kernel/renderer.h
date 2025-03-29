#pragma once

#include <QPixmap>
#include "primitives.h"
#include "scene.h"

namespace renderer {

class Renderer {
  using SentData = QPixmap;

 public:
  SentData Render(const Scene& scene);

 private:
  std::vector<Triangle> Clip() {}
};

}  // namespace renderer
