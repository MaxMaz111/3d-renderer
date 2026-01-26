#pragma once

#include "util/size.h"

#include "linalg.h"
#include "plane.h"

namespace renderer::kernel {

class Camera {
  using WidthT = Width;
  using HeightT = Height;

  static constexpr int kNumberOfPlanes = 6;
  static constexpr Scalar kMoveSpeed = 0.2;
  static constexpr Scalar kRotationSpeed = M_PI / 120;

 public:
  enum class RenderingMode { AllSolid, AllTransparent };

  Camera();
  Camera(Scalar near, Scalar far, Width screen_width, Height screen_height);

  void SetScreenDimensions(Width width, Height height);
  void SetNear(Scalar near);
  void SetFar(Scalar far);
  const std::array<Plane, kNumberOfPlanes>& GetPlanesForClipping() const;
  const Matrix4& GetProjectionMatrix() const;
  const Matrix3& GetRotationMatrix() const;
  const Point3& GetPosition() const;
  int Width() const;
  int Height() const;
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
  RenderingMode CurrentRenderingMode() const;

 private:
  Matrix4 BuildProjectionMatrix();
  std::array<Plane, kNumberOfPlanes> BuildPlanesForClipping() const;

  RenderingMode mode_ = RenderingMode::AllSolid;
  int screen_width_;
  int screen_height_;
  Scalar near_;
  Scalar far_;

  Matrix3 rotation_matrix_;
  Matrix4 projection_matrix_;
  Point3 position_;

  std::array<Plane, kNumberOfPlanes> planes_;
};

}  // namespace renderer::kernel
