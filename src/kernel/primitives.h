#pragma once

#include <array>
#include "linalg.h"

namespace renderer {

struct Triangle {
  std::array<Point3, 3> vertices;
};

}  // namespace renderer
