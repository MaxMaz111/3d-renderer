#pragma once

#include "util/size.h"

#include "linalg.h"
#include "plane.h"

namespace renderer::kernel {

class Camera {
  using WidthT = Width;
  using HeightT = Height;

  static const Vector3 kDefaultPosition;
  static const Matrix3 kDefaultRotation;

  static constexpr Scalar kDefaultNear = 0.1;
  static constexpr Scalar kDefaultFar = 1000;
  static constexpr int kNumberOfPlanes = 6;
  static constexpr Scalar kMoveSpeed = 0.2;
  static constexpr Scalar kRotationSpeed = M_PI / 120;
  static constexpr Scalar kDefaultFovY = DegToRad(60);

 public:
  enum class RenderingMode { AllSolid, AllTransparent };

  Camera();

  void SetAspectRatio(Scalar aspect_ratio);
  void SetNear(Scalar near);
  void SetFar(Scalar far);
  void RotateLeft();
  void RotateRight();
  void RotateUp();
  void RotateDown();
  void MoveLeft();
  void MoveRight();
  void MoveForward();
  void MoveBackward();
  void SwivelLeft();
  void SwivelRight();
  void SwapRenderingMode();
  const std::array<Plane, kNumberOfPlanes>& PlanesForClipping() const;
  const Matrix4& ProjectionMatrix() const;
  const Matrix3& RotationMatrix() const;
  const Point3& Position() const;
  RenderingMode CurrentRenderingMode() const;

 private:
  Matrix4 BuildProjectionMatrix() const;
  std::array<Plane, kNumberOfPlanes> BuildPlanesForClipping() const;

  RenderingMode mode_ = RenderingMode::AllSolid;
  Point3 position_;
  Matrix3 rotation_matrix_;
  Scalar near_;
  Scalar far_;
  Scalar fov_y_;
  Scalar aspect_ratio_;

  Matrix4 projection_matrix_;
  std::array<Plane, kNumberOfPlanes> planes_;
};

}  // namespace renderer::kernel
