#pragma once

#include <Eigen/Dense>

namespace renderer {
class Camera {
public:
    using Point = Eigen::Vector3d;

private:
    Point base_;
    Point direction_;
};
} // namespace renderer