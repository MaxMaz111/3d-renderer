#pragma once

#include "kernel/directional_light.h"

#include "linalg.h"
#include "mesh.h"
#include "plane.h"
#include "z_buffer.h"

namespace renderer::kernel {

class ShadowMapLight {
  static const Point3 kDefaultPosition;
  static const Matrix3 kDefaultRotation;
  static const Vector3 kDefaultDirection;

  static constexpr int kDimension = 4096;
  static constexpr Scalar kDefaultNear = 0.1;
  static constexpr Scalar kDefaultFar = 1000;
  static constexpr int kNumberOfPlanes = 6;
  static constexpr Scalar kDefaultFov = DegToRad(60);
  static constexpr Scalar kMinIntensity = 0;
  static constexpr Scalar kMaxIntensity = 1;
  static constexpr Scalar kDefaultBias = 1e-3;

  using BBox = Triangle::BBox;

 public:
  ShadowMapLight();
  ShadowMapLight(const Point3& position, const Matrix3& rotation);

  void UpdateZBuffer(std::vector<Mesh>&& meshes);
  void RotateAndMove(const Matrix3& rotation_matrix, const Point3& translation);
  Scalar CalculateIntensity(const Vector3& normal,
                            const Point3& world_point) const;

 private:
  std::vector<Mesh> RotateAndMove(std::vector<Mesh>&& meshes) const;
  std::vector<Mesh> Project(std::vector<Mesh>&& meshes) const;
  Point3 TransformToLightSpace(const Point3& world_point) const;
  Point4 ToHomogeneous(const Point3& point) const;
  Matrix4 BuildProjectionMatrix() const;
  std::array<Plane, kNumberOfPlanes> BuildPlanesForClipping() const;

  Point3 position_ = kDefaultPosition;
  Matrix3 rotation_matrix_ = kDefaultRotation;
  Scalar near_ = kDefaultNear;
  Scalar far_ = kDefaultFar;
  Scalar fov_ = kDefaultFov;

  DirectionalLight light_;

  ZBuffer z_buffer_{Width{kDimension}, Height{kDimension}};

  Matrix4 projection_matrix_;
  std::array<Plane, kNumberOfPlanes> planes_;
};

}  // namespace renderer::kernel
