#pragma once

#include <array>

#include "kernel/mesh.h"

#include "util/size.h"

#include "linalg.h"
#include "plane.h"

namespace renderer::kernel {

class ViewPoint {
  static const Point3 kDefaultPosition;
  static const Matrix3 kDefaultRotation;
  static constexpr Scalar kDefaultNear = 0.1;
  static constexpr Scalar kDefaultFar = 5;
  static constexpr Scalar kDefaultFovY = DegToRad(90);
  static constexpr int kNumberOfPlanes = 6;
  static constexpr int kParallelGranularity = 4096;

 public:
  ViewPoint(Width width, Height height);
  ViewPoint(Width width, Height height, const Point3& position,
            const Matrix3& rotation);

  const Matrix4& ProjectionMatrix() const;
  const Matrix3& RotationMatrix() const;
  const Point3& Position() const;
  void SetDimensions(Width width, Height height);
  const std::array<Plane, kNumberOfPlanes>& PlanesForClipping() const;
  std::vector<Mesh> RotateAndMove(std::vector<Mesh>&& meshes) const;
  std::vector<Mesh> Project(std::vector<Mesh>&& meshes) const;

 protected:
  Matrix4 BuildProjectionMatrix() const;
  std::array<Plane, kNumberOfPlanes> BuildPlanesForClipping() const;

  Point3 position_;
  Matrix3 rotation_matrix_;
  Scalar near_ = kDefaultNear;
  Scalar far_ = kDefaultFar;
  Scalar fov_y_ = kDefaultFovY;
  int width_;
  int height_;

  Matrix4 projection_matrix_;
  std::array<Plane, kNumberOfPlanes> planes_;
};

}  // namespace renderer::kernel
