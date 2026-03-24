#pragma once

#include <vector>

#include "texture.h"
#include "triangle.h"

namespace renderer::kernel {

struct Mesh {
  std::vector<Triangle> triangles;
  Texture diffuse_texture;
};

}  // namespace renderer::kernel
