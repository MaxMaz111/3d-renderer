#include "camera.h"

#include <cassert>

#include "linalg.h"

namespace renderer::kernel {

Camera::Camera()
    : screen_width_(800),
      screen_height_(600),
      near_(500),
      far_(10000),
      rotation_matrix_(AngleAxis(M_PI, Vector3::UnitX()) *
                       AngleAxis(-M_PI / 2, Vector3::UnitY())),
      projection_matrix_(BuildProjectionMatrix()),
      position_(15, 0, 0),
      planes_(BuildPlanesForClipping()) {}

Camera::Camera(Scalar near, Scalar far, WidthT screen_width,
               HeightT screen_height)
    : screen_width_(screen_width),
      screen_height_(screen_height),
      near_(near),
      far_(far),
      rotation_matrix_(Matrix3::Identity()),
      planes_(BuildPlanesForClipping()) {
  assert(near > 0 && far > 0);
  assert(far > near);
  assert(screen_width > WidthT{0});
  assert(screen_height > HeightT{0});
}

void Camera::SetScreenDimensions(WidthT width, HeightT height) {
  screen_width_ = width;
  screen_height_ = height;
  planes_ = BuildPlanesForClipping();
  projection_matrix_ = BuildProjectionMatrix();
}

void Camera::SetNear(Scalar near) {
  assert(near > 0);
  assert(far_ > near);
  near_ = near;
  planes_ = BuildPlanesForClipping();
  projection_matrix_ = BuildProjectionMatrix();
}

void Camera::SetFar(Scalar far) {
  assert(far > 0);
  assert(far > near_);
  far_ = far;
  planes_ = BuildPlanesForClipping();
  projection_matrix_ = BuildProjectionMatrix();
}

const std::array<Plane, Camera::kNumberOfPlanes>& Camera::GetPlanesForClipping()
    const {
  return planes_;
}

const Matrix4& Camera::GetProjectionMatrix() const {
  return projection_matrix_;
}

const Matrix3& Camera::GetRotationMatrix() const {
  return rotation_matrix_;
}

const Point3& Camera::GetPosition() const {
  return position_;
}

int Camera::Width() const {
  return screen_width_;
}

int Camera::Height() const {
  return screen_height_;
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

Matrix4 Camera::BuildProjectionMatrix() {
  assert((far_ - near_) > kEpsilon);
  return Matrix4{{near_, 0, static_cast<Scalar>(screen_width_) / 2, 0},
                 {0, near_, static_cast<Scalar>(screen_height_) / 2, 0},
                 {0, 0, far_ / (far_ - near_), -far_ * near_ / (far_ - near_)},
                 {0, 0, 1, 0}};
}

std::array<Plane, Camera::kNumberOfPlanes> Camera::BuildPlanesForClipping()
    const {
  Plane near(Vector3{0, 0, 1}, near_);
  Plane far(Vector3{0, 0, -1}, far_);
  Plane left(Vector3(-near_, 0, screen_width_ / 2), 0);
  Plane right(Vector3(near_, 0, screen_width_ / 2), 0);
  Plane up(Vector3(0, -near_, screen_height_ / 2), 0);
  Plane down(Vector3(0, near_, screen_height_ / 2), 0);
  return {near, far, left, right, up, down};
}

}  // namespace renderer::kernel
