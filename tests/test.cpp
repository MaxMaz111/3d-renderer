#include <camera.h>
#include <gtest/gtest.h>

namespace {

struct TestableCamera : renderer::kernel::Camera {
  using Camera::PlanesForClipping;
};

}  // namespace

TEST(Kernel, TestNearFar) {
  TestableCamera c;
  const auto& planes = c.PlanesForClipping();

  std::vector<renderer::kernel::Point3> inside_points = {
      {0, 0, -1}, {0, 0, -2}, {0, 0, -999}};
  std::vector<renderer::kernel::Point3> outside_points = {
      {0, 0, 0}, {0, 0, 1}, {10000, 10000, 10000}, {0, 0, -1001}};

  for (const auto& point : inside_points) {
    EXPECT_TRUE(planes[0].IsOnTheSameSideAsNormal(point) &&
                planes[1].IsOnTheSameSideAsNormal(point));
  }

  for (const auto& point : outside_points) {
    EXPECT_FALSE(planes[0].IsOnTheSameSideAsNormal(point) &&
                 planes[1].IsOnTheSameSideAsNormal(point));
  }
}

TEST(Kernel, TestInsideOutsideSimple) {
  TestableCamera c;
  const auto& planes = c.PlanesForClipping();

  std::vector<renderer::kernel::Point3> inside_points = {
      {0, 0, -1}, {0, 0, -2}, {0, 0, -999}};
  std::vector<renderer::kernel::Point3> outside_points = {
      {0, 0, 0}, {0, 0, 1}, {10000, 10000, 10000}, {0, 0, -1001}};

  auto is_inside_frustum = [planes](const renderer::kernel::Point3& p) {
    for (const auto& plane : planes) {
      if (!plane.IsOnTheSameSideAsNormal(p)) {
        return false;
      }
    }
    return true;
  };

  auto is_outside_frustum = [planes](const renderer::kernel::Point3& p) {
    for (const auto& plane : planes) {
      if (!plane.IsOnTheSameSideAsNormal(p)) {
        return true;
      }
    }
    return false;
  };

  for (const auto& point : inside_points) {
    EXPECT_TRUE(is_inside_frustum(point));
    EXPECT_FALSE(is_outside_frustum(point));
  }

  for (const auto& point : outside_points) {
    EXPECT_TRUE(is_outside_frustum(point));
    EXPECT_FALSE(is_inside_frustum(point));
  }
}

TEST(Kernel, TestInsideOutsideAdvanced) {
  TestableCamera c;
  const auto& planes = c.PlanesForClipping();

  std::vector<renderer::kernel::Point3> inside_points = {
      {0, 0, -1},          {0, 0, -50},        {0, 0, -500},
      {-0.05, 0.03, -0.2}, {0.05, -0.02, -10}, {-0.5, 0.3, -500},
      {0.5, -0.3, -999}};

  std::vector<renderer::kernel::Point3> outside_points = {
      {0, 0, 0},      {0, 0, 1001}, {100, 0, -50},
      {-100, 0, -50}, {0, 50, -50}, {0, -50, -50}};

  auto is_inside_frustum = [planes](const renderer::kernel::Point3& p) {
    for (const auto& plane : planes) {
      if (!plane.IsOnTheSameSideAsNormal(p)) {
        return false;
      }
    }
    return true;
  };

  auto is_outside_frustum = [planes](const renderer::kernel::Point3& p) {
    for (const auto& plane : planes) {
      if (!plane.IsOnTheSameSideAsNormal(p)) {
        return true;
      }
    }
    return false;
  };

  for (const auto& point : inside_points) {
    EXPECT_TRUE(is_inside_frustum(point));
    EXPECT_FALSE(is_outside_frustum(point));
  }

  for (const auto& point : outside_points) {
    EXPECT_TRUE(is_outside_frustum(point));
    EXPECT_FALSE(is_inside_frustum(point));
  }
}
