#include <QApplication>

#include "application.h"
#include "except.h"

int main(int argc, char* argv[]) {
  QApplication runtime(argc, argv);
  try {
    renderer::Application app(argc, argv);
    return runtime.exec();
  } catch (...) {
    except::React();
    return 0;
  }
}

// #include <iostream>

// #include "kernel/linalg.h"
// #include "kernel/plane.h"

// using renderer::Plane;
// using renderer::Point3;
// using renderer::Scalar;
// using renderer::Vector3;

// int main() {
//   Scalar near_ = 4;
//   Scalar far_ = 10;
//   Scalar screen_width_ = 8;
//   Scalar screen_height_ = 6;
//   Plane near(Vector3{0, 0, 1}, near_);
//   Plane far(Vector3{0, 0, -1}, far_);
//   Plane left(Vector3{-near_, 0, screen_width_ / 2}, 0);
//   Plane right(Vector3{near_, 0, screen_width_ / 2}, 0);
//   Plane up(Vector3{0, -near_, screen_height_ / 2}, 0);
//   Plane down(Vector3{0, near_, screen_height_ / 2}, 0);
//   auto inside = [&](const Point3& pt) {
//     return near.IsOnTheSameSideAsNormal(pt) &&
//            far.IsOnTheSameSideAsNormal(pt) &&
//            left.IsOnTheSameSideAsNormal(pt) &&
//            right.IsOnTheSameSideAsNormal(pt) &&
//            up.IsOnTheSameSideAsNormal(pt) && down.IsOnTheSameSideAsNormal(pt);
//   };
//   Scalar x, y, z;
//   std::cin >> x >> y >> z;
//   std::cout << inside({x, y, z}) << '\n';
// }
