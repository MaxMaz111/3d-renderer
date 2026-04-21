#pragma once

#include "kernel/directional_light.h"
#include "kernel/triangle.h"

#include "linalg.h"
#include "mesh.h"
#include "view_point.h"
#include "z_buffer.h"

namespace renderer::kernel {

class ShadowMapLight : public ViewPoint {
  static constexpr int kKernel = 1;
  static constexpr int kDimension = 2048;
  static constexpr Scalar kMinIntensity = 0;
  static constexpr Scalar kMaxIntensity = 1;
  static constexpr Scalar kDefaultBias = 0.005;
  static constexpr Scalar kSubpixelCenterOffset = 0.5;
  static constexpr Scalar kScreenToNdcScale = 2;

  using BBox = Triangle::BBox;

 public:
  ShadowMapLight(const Point3& position, const Matrix3& rotation);

  void UpdateZBuffer(std::vector<Mesh>&& meshes);
  void RotateAndMove(const Matrix3& rotation_matrix, const Point3& translation);
  Scalar CalculateIntensity(const Vector3& normal,
                            const Point3& world_point) const;

 private:
  Scalar SampleShadow(Width x, Height y, Scalar depth) const;
  Scalar ComputeShadowPCF(const Point3& p) const;
  Scalar CalculateFading(XAxis x, YAxis y) const;
  Point3 TransformToLightSpace(const Point3& world_point) const;
  Point4 ToHomogeneous(const Point3& point) const;
  bool IsBounded(Width x, Height y) const;

  DirectionalLight light_;
  ZBuffer z_buffer_{Width{kDimension}, Height{kDimension}};
};

}  // namespace renderer::kernel
