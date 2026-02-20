#include "directional_light.h"

#include <algorithm>

namespace renderer::kernel {

DirectionalLight::DirectionalLight() : direction_(0.0f, 1.0f, 0.0f) {}

DirectionalLight::DirectionalLight(const Vector3& direction)
    : direction_(direction.normalized()) {}

Scalar DirectionalLight::CalculateIntensity(const Vector3& normal) const {
  Scalar dot_product = direction_.dot(normal.normalized());
  Scalar intensity = ScaleIntensity(dot_product);
  return std::clamp(intensity, kMinIntensity, kMaxIntensity);
}

void DirectionalLight::RotateAndMove(const Matrix3& rotation_matrix,
                                     const Point3&) {
  direction_ = rotation_matrix * direction_;
}

Scalar DirectionalLight::ScaleIntensity(Scalar dot_product) {
  return (dot_product + 1) / 2;
}

}  // namespace renderer::kernel
