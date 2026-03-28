#pragma once

#include "linalg.h"

namespace renderer::kernel {

struct Vertex {
  Point3 point;
  Vector3 normal;
  Point2 tex_coord;
  Point3 world_point;
  Scalar inv_w = 1;
};

}  // namespace renderer::kernel
