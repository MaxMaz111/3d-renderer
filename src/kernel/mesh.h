#pragma once

#include <vector>

#include "triangle.h"

namespace renderer::kernel {

struct Mesh {
  std::vector<Triangle> triangles;
};

}  // namespace renderer::kernel
