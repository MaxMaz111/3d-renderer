#include "camera.h"

#include <cassert>
#include <iostream>

#include "linalg.h"

namespace renderer {

Camera::Camera()
    : screen_width_(Width{800}),
      screen_height_(Height{600}),
      near_(500),
      far_(10000),
      rotation_matrix_(AngleAxis(M_PI, Vector3::UnitX()) *
                       AngleAxis(-M_PI / 2, Vector3::UnitY())),
      position_(15, 0, 0),
      planes_(BuildPlanesForClipping()) {
  BuildProjectionMatrix();
}

Camera::Camera(Scalar near, Scalar far, Width screen_width,
               Height screen_height)
    : screen_width_(screen_width),
      screen_height_(screen_height),
      near_(near),
      far_(far),
      rotation_matrix_(Matrix3::Identity()),
      planes_(BuildPlanesForClipping()) {
  assert(near > 0 && far > 0);
  assert(far > near);
  assert(screen_width > Width{0});
  assert(screen_height > Height{0});
}

void Camera::SetScreenDimensions(Width width, Height height) {
  screen_width_ = width;
  screen_height_ = height;
  planes_ = BuildPlanesForClipping();
  BuildProjectionMatrix();
}

void Camera::SetNear(Scalar near) {
  assert(near > 0);
  assert(far_ > near);
  near_ = near;
  planes_ = BuildPlanesForClipping();
  BuildProjectionMatrix();
}

void Camera::SetFar(Scalar far) {
  assert(far > 0);
  assert(far > near_);
  far_ = far;
  planes_ = BuildPlanesForClipping();
  BuildProjectionMatrix();
}

const std::array<Plane, Camera::kNumberOfPlanes>& Camera::GetPlanesForClipping()
    const {
  return planes_;
}

void Camera::BuildProjectionMatrix() {
  assert((far_ - near_) > kEpsilon);
  projection_matrix_ =
      Matrix4{{near_, 0, static_cast<Scalar>(screen_width_) / 2, 0},
              {0, near_, static_cast<Scalar>(screen_height_) / 2, 0},
              {0, 0, far_ / (far_ - near_), -far_ * near_ / (far_ - near_)},
              {0, 0, 1, 0}};
}

const Matrix4& Camera::GetProjectionMatrix() const {
  return projection_matrix_;
}

Matrix3 Camera::GetRotationMatrix() const {
  return rotation_matrix_;
}

Point3 Camera::GetPosition() const {
  return position_;
}

Width Camera::GetWidth() const {
  return Width{screen_width_};
}

Height Camera::GetHeight() const {
  return Height{screen_height_};
}

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
  position_ -= rotation_matrix_.col(0) * kMoveSpeed;
}

void Camera::MoveRight() {
  position_ += rotation_matrix_.col(0) * kMoveSpeed;
}

void Camera::MoveForward() {
  position_ += rotation_matrix_.col(2) * kMoveSpeed;
}

void Camera::MoveBackward() {
  position_ -= rotation_matrix_.col(2) * kMoveSpeed;
}

void Camera::SwivelLeft() {
  rotation_matrix_ =
      AngleAxis(-kRotationSpeed, rotation_matrix_.col(2)) * rotation_matrix_;
}

void Camera::SwivelRight() {
  rotation_matrix_ =
      AngleAxis(kRotationSpeed, rotation_matrix_.col(2)) * rotation_matrix_;
}

std::array<Plane, Camera::kNumberOfPlanes> Camera::BuildPlanesForClipping() {
  Plane near(Vector3{0, 0, 1}, near_);
  Plane far(Vector3{0, 0, -1}, far_);
  Plane left(Vector3{-near_, 0, static_cast<int>(screen_width_) / 2}, 0);
  Plane right(Vector3{near_, 0, static_cast<int>(screen_width_) / 2}, 0);
  Plane up(Vector3{0, -near_, static_cast<int>(screen_height_) / 2}, 0);
  Plane down(Vector3{0, near_, static_cast<int>(screen_height_) / 2}, 0);
  return {near, far, left, right, up, down};
}

}  // namespace renderer
