#include <cmath>
#include <gtest/gtest.h>

#include "kernel/camera.h"

#include "util/size.h"

namespace {

struct TestableCamera : renderer::kernel::Camera {
  using Camera::Camera;
  using Camera::PlanesForClipping;

  renderer::kernel::Scalar Near() const { return near_; }

  renderer::kernel::Scalar Far() const { return far_; }

  renderer::kernel::Scalar FovY() const { return fov_y_; }

  const renderer::kernel::Point3& PositionRef() const { return position_; }

  const renderer::kernel::Matrix3& RotationRef() const {
    return rotation_matrix_;
  }

  int WidthPx() const { return width_; }

  int HeightPx() const { return height_; }
};

using Point3 = renderer::kernel::Point3;
using Scalar = renderer::kernel::Scalar;

bool IsInsideFrustum(const TestableCamera& camera, const Point3& point) {
  const auto& planes = camera.PlanesForClipping();
  for (const auto& plane : planes) {
    if (!plane.IsOnTheSameSideAsNormal(point)) {
      return false;
    }
  }
  return true;
}

}  // namespace

TEST(Kernel, FrustumNearFarBoundaries) {
  TestableCamera c(renderer::Width{800}, renderer::Height{600});

  const Point3 inside_near{Scalar{0}, Scalar{0}, -c.Near() * Scalar{1.1f}};
  const Point3 inside_far{Scalar{0}, Scalar{0}, -c.Far() * Scalar{0.9f}};
  const Point3 outside_near{Scalar{0}, Scalar{0}, -c.Near() * Scalar{0.5f}};
  const Point3 outside_far{Scalar{0}, Scalar{0}, -(c.Far() + Scalar{1.0f})};

  EXPECT_TRUE(IsInsideFrustum(c, inside_near));
  EXPECT_TRUE(IsInsideFrustum(c, inside_far));
  EXPECT_FALSE(IsInsideFrustum(c, outside_near));
  EXPECT_FALSE(IsInsideFrustum(c, outside_far));
}

TEST(Kernel, FrustumSidePlanesBoundaries) {
  TestableCamera c(renderer::Width{800}, renderer::Height{600});
  const Scalar z = -(c.Near() + c.Far()) * 0.5;
  const Scalar depth = -z;
  const Scalar aspect =
      static_cast<Scalar>(c.WidthPx()) / static_cast<Scalar>(c.HeightPx());
  const Scalar half_height = depth * std::tan(c.FovY() * 0.5);
  const Scalar half_width = half_height * aspect;

  const Point3 inside{half_width * Scalar{0.9f}, half_height * Scalar{0.9f}, z};
  const Point3 outside_right{half_width * Scalar{1.1f}, Scalar{0}, z};
  const Point3 outside_left{-half_width * Scalar{1.1f}, Scalar{0}, z};
  const Point3 outside_top{Scalar{0}, half_height * Scalar{1.1f}, z};
  const Point3 outside_bottom{Scalar{0}, -half_height * Scalar{1.1f}, z};

  EXPECT_TRUE(IsInsideFrustum(c, inside));
  EXPECT_FALSE(IsInsideFrustum(c, outside_right));
  EXPECT_FALSE(IsInsideFrustum(c, outside_left));
  EXPECT_FALSE(IsInsideFrustum(c, outside_top));
  EXPECT_FALSE(IsInsideFrustum(c, outside_bottom));
}

TEST(Kernel, RenderingModeToggle) {
  TestableCamera c(renderer::Width{800}, renderer::Height{600});
  EXPECT_EQ(c.CurrentRenderingMode(),
            renderer::kernel::Camera::RenderingMode::AllSolid);
  c.SwapRenderingMode();
  EXPECT_EQ(c.CurrentRenderingMode(),
            renderer::kernel::Camera::RenderingMode::AllTransparent);
  c.SwapRenderingMode();
  EXPECT_EQ(c.CurrentRenderingMode(),
            renderer::kernel::Camera::RenderingMode::AllSolid);
}

TEST(Kernel, MoveOperationsAreReversible) {
  TestableCamera c(renderer::Width{800}, renderer::Height{600});
  const auto initial = c.PositionRef();

  c.MoveForward();
  c.MoveBackward();
  c.MoveLeft();
  c.MoveRight();

  EXPECT_TRUE(c.PositionRef().isApprox(initial, 1e-6f));
}

TEST(Kernel, RotationOperationsAreReversible) {
  TestableCamera c(renderer::Width{800}, renderer::Height{600});
  const auto initial = c.RotationRef();

  c.RotateLeft();
  c.RotateRight();
  c.RotateUp();
  c.RotateDown();
  c.SwivelLeft();
  c.SwivelRight();

  EXPECT_TRUE(c.RotationRef().isApprox(initial, 1e-5f));
}

TEST(Kernel, FrustumCenterStaysInsideAfterResize) {
  TestableCamera c(renderer::Width{800}, renderer::Height{600});
  c.SetDimensions(renderer::Width{1920}, renderer::Height{1080});

  const Point3 center{Scalar{0}, Scalar{0}, -(c.Near() + Scalar{0.5f})};
  EXPECT_TRUE(IsInsideFrustum(c, center));
}
