#pragma once

#include "../constants.h"
#include "linalg.h"

namespace renderer {

class Camera {
 public:
  Camera();

  Camera(const Point3& pos, Scalar depth);

 private:
  void InitPlanesForClipping();

  Point3 pos_ = {0, 0, -800};
  const Point3 left_down_{-constants::kWindowWidth / 2,
                          -constants::kWindowHeight / 2, 0};
  const Point3 left_up_{-constants::kWindowWidth / 2,
                        constants::kWindowHeight / 2, 0};
  const Point3 right_up_{constants::kWindowWidth / 2,
                         constants::kWindowHeight / 2, 0};
  const Point3 right_down_{constants::kWindowWidth / 2,
                           -constants::kWindowHeight / 2, 0};
  Scalar depth_ = 10000;

  std::vector<Plane3> planes_for_clipping_;
};

}  // namespace renderer
