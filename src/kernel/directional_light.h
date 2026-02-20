#pragma once

#include "kernel/linalg.h"

namespace renderer::kernel {

class DirectionalLight {
 public:
  DirectionalLight();
  DirectionalLight(const Vector3& direction);

  Scalar CalculateIntensity(const Vector3& normal) const;
  void RotateAndMove(const Matrix3& rotation_matrix, const Point3& translation);

 private:
  static constexpr Scalar kMinIntensity = 0.0f;
  static constexpr Scalar kMaxIntensity = 1.0f;

  static Scalar ScaleIntensity(Scalar dot_product);

  Vector3 direction_;
};

}  // namespace renderer::kernel
