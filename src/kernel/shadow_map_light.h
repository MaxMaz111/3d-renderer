#pragma once

#include "kernel/directional_light.h"
#include "kernel/triangle.h"

#include "linalg.h"
#include "mesh.h"
#include "view_point.h"
#include "z_buffer.h"

namespace renderer::kernel {

class ShadowMapLight : public ViewPoint {
  static constexpr int kDimension = 4096;
  static constexpr Scalar kMinIntensity = 0;
  static constexpr Scalar kMaxIntensity = 1;
  static constexpr Scalar kDefaultShadowBackoff = 50;
  static constexpr Scalar kDefaultBias = 0.05;
  static constexpr Scalar kDefaultTooFarThreshold = 0.1;

  using BBox = Triangle::BBox;

 public:
  ShadowMapLight(const Point3& position, const Matrix3& rotation);

  void UpdateZBuffer(std::vector<Mesh>&& meshes);
  void RotateAndMove(const Matrix3& rotation_matrix, const Point3& translation);
  Scalar CalculateIntensity(const Vector3& normal,
                            const Point3& world_point) const;

 private:
  Scalar CalculateFading(XAxis x, YAxis y) const;
  Point3 TransformToLightSpace(const Point3& world_point) const;
  Point4 ToHomogeneous(const Point3& point) const;

  DirectionalLight light_;
  ZBuffer z_buffer_{Width{kDimension}, Height{kDimension}};
};

}  // namespace renderer::kernel
