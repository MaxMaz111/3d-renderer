#include "camera.h"

#include <cassert>

#include "constants.h"

namespace renderer::kernel {

const Vector3 Camera::kDefaultPosition{0, -30, 10};
const Matrix3 Camera::kDefaultRotation = Matrix3{AngleAxis(M_PI / 2, Vector3::UnitX())};

Camera::Camera()
    : position_(kDefaultPosition),
      rotation_matrix_(kDefaultRotation),
      near_(kDefaultNear),
      far_(kDefaultFar),
      fov_y_(kDefaultFovY),
      aspect_ratio_(AspectRatio(Width{kDefaultWidth}, Height{kDefaultHeight})),
      projection_matrix_(BuildProjectionMatrix()),
      planes_(BuildPlanesForClipping()) {}

void Camera::SetAspectRatio(Scalar aspect_ratio) {
  aspect_ratio_ = aspect_ratio;
  planes_ = BuildPlanesForClipping();
  projection_matrix_ = BuildProjectionMatrix();
}

void Camera::SetNear(Scalar near) {
  near_ = near;
  planes_ = BuildPlanesForClipping();
  projection_matrix_ = BuildProjectionMatrix();
}

void Camera::SetFar(Scalar far) {
  far_ = far;
  planes_ = BuildPlanesForClipping();
  projection_matrix_ = BuildProjectionMatrix();
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
  mode_ = mode_ == RenderingMode::AllSolid ? RenderingMode::AllTransparent
                                           : RenderingMode::AllSolid;
}

const std::array<Plane, Camera::kNumberOfPlanes>& Camera::PlanesForClipping()
    const {
  return planes_;
}

const Matrix4& Camera::ProjectionMatrix() const {
  return projection_matrix_;
}

const Matrix3& Camera::RotationMatrix() const {
  return rotation_matrix_;
}

const Point3& Camera::Position() const {
  return position_;
}

Camera::RenderingMode Camera::CurrentRenderingMode() const {
  return mode_;
}

Matrix4 Camera::BuildProjectionMatrix() const {
  assert((far_ - near_) > kEpsilon);
  Scalar t = near_ * std::tan(fov_y_ * 0.5);
  Scalar b = -t;
  Scalar r = -t * aspect_ratio_;
  Scalar l = -r;
  return Matrix4{{{2 * near_ / (r - l), 0, (r + l) / (r - l), 0},
                  {0, 2 * near_ / (t - b), (t + b) / (t - b), 0},
                  {0, 0, -(far_ + near_) / (far_ - near_),
                   -2 * far_ * near_ / (far_ - near_)},
                  {0, 0, -1, 0}}};
}

std::array<Plane, Camera::kNumberOfPlanes> Camera::BuildPlanesForClipping()
    const {

  Plane near_plane(Vector3{0, 0, -1}, -near_);
  Plane far_plane(Vector3{0, 0, 1}, far_);

  Scalar half_height = near_ * std::tan(fov_y_ * 0.5f);
  Scalar half_width = half_height * aspect_ratio_;

  Plane left_plane(Vector3{-near_, 0, -half_width}, 0);
  Plane right_plane(Vector3{near_, 0, -half_width}, 0);
  Plane top_plane(Vector3{0, -near_, -half_height}, 0);
  Plane bottom_plane(Vector3{0, near_, -half_height}, 0);

  return {near_plane,  far_plane, left_plane,
          right_plane, top_plane, bottom_plane};
}

}  // namespace renderer::kernel
