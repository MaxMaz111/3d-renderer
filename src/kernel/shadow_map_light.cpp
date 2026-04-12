#include "shadow_map_light.h"

#include <cmath>
#include <limits>

#include "kernel/clipper.h"
#include "kernel/directional_light.h"
#include "kernel/mesh.h"

namespace renderer::kernel {

ShadowMapLight::ShadowMapLight(const Point3& position, const Matrix3& rotation)
    : ViewPoint(Width{kDimension}, Height{kDimension}, position, rotation),
      light_(rotation_matrix_ * Vector3{0, 0, -1}) {}

void ShadowMapLight::UpdateZBuffer(std::vector<Mesh>&& meshes) {
  meshes = ViewPoint::MoveToLocal(std::move(meshes));
  meshes = Clipper::Clip(std::move(meshes), planes_);
  meshes = ViewPoint::Project(std::move(meshes));
  for (auto& mesh : meshes) {
    for (auto& triangle : mesh.triangles) {
      BBox bbox = triangle.GetBoundingBox(Width{z_buffer_.Width()},
                                          Height{z_buffer_.Height()});
      for (int j = bbox.min_y; j <= bbox.max_y; ++j) {
        for (int i = bbox.min_x; i <= bbox.max_x; ++i) {
          Scalar x = i + 0.5f;
          Scalar y = j + 0.5f;
          auto z = triangle.InterpolateZ(XAxis{x}, YAxis{y});
          if (z == std::numeric_limits<Scalar>::infinity()) {
            continue;
          }
          Scalar& z_buffer_value = z_buffer_.Get(Width{i}, Height{j});
          if (z < z_buffer_value) {
            z_buffer_value = z;
          }
        }
      }
    }
  }
}

void ShadowMapLight::RotateAndMove(const Matrix3& rotation_matrix,
                                   const Point3& translation) {
  light_.RotateAndMove(rotation_matrix, translation);
}

Scalar ShadowMapLight::CalculateIntensity(const Vector3& normal,
                                          const Point3& world_point) const {
  Point3 light_space_point = TransformToLightSpace(world_point);

  int x = std::floor(light_space_point.x());
  int y = std::floor(light_space_point.y());
  if (x < 0 || y < 0 || x >= kDimension || y >= kDimension) {
    return 0;
  }
  const Scalar stored_z = z_buffer_.Get(Width{x}, Height{y});

  Scalar d = light_space_point.z() - stored_z;
  if (d > kDefaultBias) {
    return 0;
  }
  Scalar factor = std::min(kMaxIntensity, std::exp(-d * kDefaultShadowBackoff));
  if (factor < kDefaultTooFarThreshold) {
    return 0;
  }
  return light_.CalculateIntensity(normal) *
         CalculateFading(XAxis{x}, YAxis{y}) * factor;
}

Scalar ShadowMapLight::CalculateFading(XAxis x, YAxis y) const {
  Scalar x_norm = x / kDimension * 2 - 1;
  Scalar y_norm = y / kDimension * 2 - 1;
  return 1 - std::clamp(std::sqrt(x_norm * x_norm + y_norm * y_norm), 0.f, 1.f);
}

Point3 ShadowMapLight::TransformToLightSpace(const Point3& world_point) const {
  Point4 clip =
      projection_matrix_ *
      ToHomogeneous(rotation_matrix_.transpose() * (world_point - position_));
  if (std::abs(clip.w()) < kEpsilon) {
    return Point3(std::numeric_limits<Scalar>::infinity(),
                  std::numeric_limits<Scalar>::infinity(),
                  std::numeric_limits<Scalar>::infinity());
  }
  return Point3(clip.x() / clip.w(), clip.y() / clip.w(), clip.z() / clip.w());
}

Point4 ShadowMapLight::ToHomogeneous(const Point3& point) const {
  return Point4(point.x(), point.y(), point.z(), 1);
}

}  // namespace renderer::kernel
