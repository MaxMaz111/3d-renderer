#include "camera.h"

#include <cassert>

#include "util/constants.h"
#include "util/size.h"

namespace renderer::kernel {

const Vector3 Camera::kDefaultPosition{4, 0, 0};
const Matrix3 Camera::kDefaultRotation =
    Matrix3{AngleAxis(std::numbers::pi_v<Scalar> / 2, Vector3::UnitY())};

Camera::Camera()
    : position_(kDefaultPosition),
      rotation_matrix_(kDefaultRotation),
      near_(kDefaultNear),
      far_(kDefaultFar),
      fov_y_(kDefaultFovY),
      width_(kDefaultWidth),
      height_(kDefaultHeight),
      projection_matrix_(BuildProjectionMatrix()),
      planes_(BuildPlanesForClipping()) {}

void Camera::SetDimensions(Width width, Height height) {
  assert(aspect_ratio > kEpsilon);
  width_ = width;
  height_ = height;
  planes_ = BuildPlanesForClipping();
  projection_matrix_ = BuildProjectionMatrix();
}

void Camera::SetNear(Scalar near) {
  assert(near > kEpsilon);
  assert(far_ > near);
  near_ = near;
  planes_ = BuildPlanesForClipping();
  projection_matrix_ = BuildProjectionMatrix();
}

void Camera::SetFar(Scalar far) {
  assert(far > kEpsilon);
  assert(far > near_);
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
  if (mode_ == RenderingMode::AllSolid) {
    mode_ = RenderingMode::AllTransparent;
  } else {
    mode_ = RenderingMode::AllSolid;
  }
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
  Scalar aspect_ratio = AspectRatio(Width{width_}, Height{height_});
  Scalar t = near_ * std::tan(fov_y_ * 0.5);
  Scalar b = -t;
  Scalar r = -t * aspect_ratio;
  Scalar l = -r;

  Matrix4 projection{{{2 * near_ / (r - l), 0, (r + l) / (r - l), 0},
                      {0, 2 * near_ / (t - b), (t + b) / (t - b), 0},
                      {0, 0, -(far_ + near_) / (far_ - near_),
                       -2 * far_ * near_ / (far_ - near_)},
                      {0, 0, -1, 0}}};

  Scalar w = width_;
  Scalar h = height_;
  Matrix4 raster{{{w / 2, 0, 0, w / 2},
                  {0, -h / 2, 0, h / 2},
                  {0, 0, 0.5, 0.5},
                  {0, 0, 0, 1}}};

  return raster * projection;
}

std::array<Plane, Camera::kNumberOfPlanes> Camera::BuildPlanesForClipping()
    const {

  Plane near_plane(Vector3{0, 0, -1}, -near_);
  Plane far_plane(Vector3{0, 0, 1}, far_);

  Scalar half_height = near_ * std::tan(fov_y_ * 0.5f);
  Scalar aspect_ratio = AspectRatio(Width{width_}, Height{height_});
  Scalar half_width = half_height * aspect_ratio;

  Plane left_plane(Vector3{-near_, 0, -half_width}, 0);
  Plane right_plane(Vector3{near_, 0, -half_width}, 0);
  Plane top_plane(Vector3{0, -near_, -half_height}, 0);
  Plane bottom_plane(Vector3{0, near_, -half_height}, 0);

  return {near_plane, left_plane,   right_plane,
          top_plane,  bottom_plane, far_plane};
}

}  // namespace renderer::kernel
