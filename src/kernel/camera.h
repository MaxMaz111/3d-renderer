#pragma once

#include "../size.h"
#include "linalg.h"
#include "plane.h"

namespace renderer {

class Camera {
  static constexpr int kNumberOfPlanes = 6;
  static constexpr Scalar kMoveSpeed = 0.1;
  static constexpr Scalar kRotationSpeed = M_PI / 180;

 public:
  Camera();
  Camera(Scalar near, Scalar far, Scalar screen_width, Scalar screen_height);
  void SetScreenDimensions(Width width, Height height);
  void SetNear(Scalar near);
  void SetFar(Scalar far);
  const std::array<Plane, kNumberOfPlanes>& GetPlanesForClipping() const;
  void BuildProjectionMatrix();
  const Matrix4& GetProjectionMatrix() const;
  Matrix3 GetRotationMatrix() const;
  Point3 GetPosition() const;
  int GetWidth() const;
  int GetHeight() const;
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

 private:
  std::array<Plane, kNumberOfPlanes> BuildPlanesForClipping();

  Scalar screen_width_;
  Scalar screen_height_;
  Scalar near_;
  Scalar far_;

  Matrix3 rotation_matrix_;
  Matrix4 projection_matrix_;
  Point3 position_;

  std::array<Plane, kNumberOfPlanes> planes_;
};

;

}  // namespace renderer
