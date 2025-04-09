#include "camera.h"

#include <cassert>

#include "linalg.h"

namespace renderer {

Camera::Camera()
    : near_(100),
      far_(10000),
      screen_width_(800),
      screen_height_(600),
      planes_(BuildClippingPlanes()),
      rotation_matrix_(Matrix3::Identity()) {}

Camera::Camera(Scalar near, Scalar far, Scalar screen_width,
               Scalar screen_height)
    : near_(near),
      far_(far),
      screen_width_(screen_width),
      screen_height_(screen_height),
      planes_(BuildClippingPlanes()),
      rotation_matrix_(Matrix3::Identity()) {
  assert(near > 0 && far > 0);
  assert(far > near);
  assert(screen_width > 0);
  assert(screen_height > 0);
}

void Camera::SetScreenDimensions(Width width, Height height) {
  screen_width_ = static_cast<Scalar>(width);
  screen_height_ = static_cast<Scalar>(height);
  planes_ = BuildClippingPlanes();
}

void Camera::SetNear(Scalar near) {
  assert(near > 0);
  assert(far_ > near);
  near_ = near;
  planes_ = BuildClippingPlanes();
}

void Camera::SetFar(Scalar far) {
  assert(far > 0);
  assert(far > near_);
  far_ = far;
  planes_ = BuildClippingPlanes();
}

const std::array<Plane, Camera::kNumberOfPlanes>& Camera::GetPlanesForClipping()
    const {
  return planes_;
}

Matrix4 Camera::GetProjectionMatrix() const {
  assert((far_ - near_) > kEpsilon);
  return Matrix4{{near_, 0, screen_width_ / 2, 0},
                 {0, near_, screen_height_ / 2, 0},
                 {0, 0, far_ / (far_ - near_), -far_ * near_ / (far_ - near_)},
                 {0, 0, 1, 0}};
}

Matrix3 Camera::GetRotationMatrix() const {
  return rotation_matrix_;
}

int Camera::GetWidth() const {
  return static_cast<int>(screen_width_);
}

int Camera::GetHeight() const {
  return static_cast<int>(screen_height_);
}

void Camera::RotateLeft() {
  rotation_matrix_ = AngleAxis(M_PI / 10, Vector3{0, 1, 0}) * rotation_matrix_;
}

void Camera::RotateRight() {
  rotation_matrix_ = AngleAxis(-M_PI / 10, Vector3{0, 1, 0}) * rotation_matrix_;
}

void Camera::RotateUp() {
  rotation_matrix_ = AngleAxis(M_PI / 10, Vector3{1, 0, 0}) * rotation_matrix_;
}

void Camera::RotateDown() {
  rotation_matrix_ = AngleAxis(-M_PI / 10, Vector3{1, 0, 0}) * rotation_matrix_;
}

std::array<Plane, Camera::kNumberOfPlanes> Camera::BuildClippingPlanes() {
  Plane near(Vector3{0, 0, 1}, near_);
  Plane far(Vector3{0, 0, -1}, far_);
  Plane left(Vector3{-near_, 0, screen_width_ / 2}, 0);
  Plane right(Vector3{near_, 0, screen_width_ / 2}, 0);
  Plane up(Vector3{0, -near_, screen_height_ / 2}, 0);
  Plane down(Vector3{0, near_, screen_height_ / 2}, 0);
  return {near, far, left, right, up, down};
}

}  // namespace renderer
