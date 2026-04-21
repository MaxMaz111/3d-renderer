#include "view_point.h"

#include <cassert>
#include <cmath>
#include <numbers>
#include <tbb/parallel_for.h>

#include "kernel/mesh.h"

#include "util/constants.h"
#include "util/size.h"

namespace renderer::kernel {

const Point3 ViewPoint::kDefaultPosition{-1, 0, 0};
const Matrix3 ViewPoint::kDefaultRotation =
    Matrix3{AngleAxis(-std::numbers::pi_v<Scalar> / 2, Vector3::UnitY())};

ViewPoint::ViewPoint(Width width, Height height)
    : ViewPoint(width, height, kDefaultPosition, kDefaultRotation) {}

ViewPoint::ViewPoint(Width width, Height height, const Point3& position,
                     const Matrix3& rotation)
    : position_(position),
      rotation_matrix_(rotation),
      width_(width),
      height_(height),
      projection_matrix_(BuildProjectionMatrix()),
      planes_(BuildPlanesForClipping()) {}

const Matrix4& ViewPoint::ProjectionMatrix() const {
  return projection_matrix_;
}

const Matrix3& ViewPoint::RotationMatrix() const {
  return rotation_matrix_;
}

const Point3& ViewPoint::Position() const {
  return position_;
}

void ViewPoint::SetDimensions(Width width, Height height) {
  assert(AspectRatio(width, height) > kEpsilon);
  width_ = width;
  height_ = height;
  projection_matrix_ = BuildProjectionMatrix();
  planes_ = BuildPlanesForClipping();
}

const std::array<Plane, ViewPoint::kNumberOfPlanes>&
ViewPoint::PlanesForClipping() const {
  return planes_;
}

std::vector<Mesh> ViewPoint::MoveToLocal(std::vector<Mesh>&& meshes) const {
  const Matrix3 mat = rotation_matrix_.transpose();
  const Point3 translation = -position_;
  tbb::parallel_for(
      tbb::blocked_range<size_t>(0, meshes.size()),
      [&](const tbb::blocked_range<size_t>& range) {
        for (size_t i = range.begin(); i < range.end(); ++i) {
          tbb::parallel_for(
              tbb::blocked_range<size_t>(0, meshes[i].triangles.size(),
                                         kParallelGranularity),
              [&](const tbb::blocked_range<size_t>& range) {
                for (size_t j = range.begin(); j < range.end(); ++j) {
                  meshes[i].triangles[j].RotateAndMove(mat, translation);
                }
              });
        }
      });
  return meshes;
}

std::vector<Mesh> ViewPoint::Project(std::vector<Mesh>&& meshes) const {
  tbb::parallel_for(
      tbb::blocked_range<size_t>(0, meshes.size()),
      [&](const tbb::blocked_range<size_t>& range) {
        for (size_t i = range.begin(); i < range.end(); ++i) {
          tbb::parallel_for(
              tbb::blocked_range<size_t>(0, meshes[i].triangles.size(),
                                         kParallelGranularity),
              [&](const tbb::blocked_range<size_t>& range) {
                for (size_t j = range.begin(); j < range.end(); ++j) {
                  meshes[i].triangles[j].Project(projection_matrix_);
                }
              });
        }
      });
  return meshes;
}

Matrix4 ViewPoint::BuildProjectionMatrix() const {
  assert((far_ - near_) > kEpsilon);

  Scalar width = width_;
  Scalar height = height_;
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

  Matrix4 raster{{{width / 2, 0, 0, width / 2},
                  {0, -height / 2, 0, height / 2},
                  {0, 0, 0.5, 0.5},
                  {0, 0, 0, 1}}};

  return raster * projection;
}

std::array<Plane, ViewPoint::kNumberOfPlanes>
ViewPoint::BuildPlanesForClipping() const {
  Scalar aspect_ratio = AspectRatio(Width{width_}, Height{height_});

  Plane near_plane(Vector3{0, 0, -1}, -near_);
  Plane far_plane(Vector3{0, 0, 1}, far_);

  Scalar half_height = near_ * std::tan(fov_y_ * 0.5);
  Scalar half_width = half_height * aspect_ratio;

  Plane left_plane(Vector3{-near_, 0, -half_width}, 0);
  Plane right_plane(Vector3{near_, 0, -half_width}, 0);
  Plane top_plane(Vector3{0, -near_, -half_height}, 0);
  Plane bottom_plane(Vector3{0, near_, -half_height}, 0);

  return {near_plane,  far_plane, left_plane,
          right_plane, top_plane, bottom_plane};
}

}  // namespace renderer::kernel
