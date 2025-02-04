#pragma once

#include <Eigen/Dense>

#pragma once

namespace renderer {
class Triangle {
 public:
  using Point = Eigen::Vector3d;
  Triangle(Point p1, Point p2, Point p3);

 private:
  Point p1_, p2_, p3_;
};
}  // namespace renderer
