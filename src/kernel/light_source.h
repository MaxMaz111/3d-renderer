#pragma once

#include "kernel/linalg.h"

namespace renderer::kernel {

class LightSource {
 public:
 private:
  Point3 position_;
  Vector3 direction_;
};

}  // namespace renderer
