#include "camera.h"

#include <cassert>

#include "util/size.h"

namespace renderer::kernel {

Camera::Camera(Width width, Height height) : ViewPoint(width, height) {}

void Camera::RotateLeft() {
  rotation_matrix_ =
      AngleAxis(-kRotationSpeed, rotation_matrix_.col(1)) * rotation_matrix_;
}

void Camera::RotateRight() {
  rotation_matrix_ =
      AngleAxis(kRotationSpeed, rotation_matrix_.col(1)) * rotation_matrix_;
}

void Camera::RotateUp() {
  rotation_matrix_ =
      AngleAxis(kRotationSpeed, rotation_matrix_.col(0)) * rotation_matrix_;
}

void Camera::RotateDown() {
  rotation_matrix_ =
      AngleAxis(-kRotationSpeed, rotation_matrix_.col(0)) * rotation_matrix_;
}

void Camera::MoveLeft() {
  position_ += rotation_matrix_.col(0) * kMoveSpeed;
}

void Camera::MoveRight() {
  position_ -= rotation_matrix_.col(0) * kMoveSpeed;
}

void Camera::MoveForward() {
  position_ -= rotation_matrix_.col(2) * kMoveSpeed;
}

void Camera::MoveBackward() {
  position_ += rotation_matrix_.col(2) * kMoveSpeed;
}

void Camera::SwivelLeft() {
  rotation_matrix_ =
      AngleAxis(-kRotationSpeed, rotation_matrix_.col(2)) * rotation_matrix_;
}

void Camera::SwivelRight() {
  rotation_matrix_ =
      AngleAxis(kRotationSpeed, rotation_matrix_.col(2)) * rotation_matrix_;
}

void Camera::SwapRenderingMode() {
  if (mode_ == RenderingMode::AllSolid) {
    mode_ = RenderingMode::AllTransparent;
  } else {
    mode_ = RenderingMode::AllSolid;
  }
}

Camera::RenderingMode Camera::CurrentRenderingMode() const {
  return mode_;
}

}  // namespace renderer::kernel
