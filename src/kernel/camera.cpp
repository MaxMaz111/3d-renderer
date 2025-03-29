#include "camera.h"
#include <cassert>
#include "linalg.h"

namespace renderer {

Camera::Camera() {}

Camera::Camera(const Point3& pos, Scalar depth) : pos_(pos), depth_(depth) {
  assert(pos[0] == 0 && pos[1] == 0 && pos[2] < 0);
  assert(depth > 0);
}

void Camera::InitPlanesForClipping() {}

}  // namespace renderer
