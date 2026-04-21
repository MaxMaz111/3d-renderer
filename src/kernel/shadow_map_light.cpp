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
          Scalar x = i + kSubpixelCenterOffset;
          Scalar y = j + kSubpixelCenterOffset;
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
  const Point3 light_p = TransformToLightSpace(world_point);
  if (!std::isfinite(light_p.x()) || !std::isfinite(light_p.y()) ||
      !std::isfinite(light_p.z())) {
    return kMinIntensity;
  }

  int x = std::floor(light_p.x());
  int y = std::floor(light_p.y());
  if (!IsBounded(Width{x}, Height{y})) {
    return kMinIntensity;
  }
  const Scalar shadow = ComputeShadowPCF(light_p);

  return light_.CalculateIntensity(normal) * shadow *
         CalculateFading(XAxis{x}, YAxis{y});
}

Scalar ShadowMapLight::SampleShadow(Width x, Height y, Scalar depth) const {
  const Scalar stored_z = z_buffer_.Get(x, y);

  Scalar t = (depth - stored_z) / kDefaultBias;
  t = std::clamp(t, kMinIntensity, kMaxIntensity);

  return kMaxIntensity - t;
}

Scalar ShadowMapLight::ComputeShadowPCF(const Point3& p) const {
  Scalar shadow = kMinIntensity;
  int count = 0;

  int base_x = std::floor(p.x());
  int base_y = std::floor(p.y());

  for (int dx = -kKernel; dx <= kKernel; ++dx) {
    for (int dy = -kKernel; dy <= kKernel; ++dy) {
      const int sample_x = base_x + dx;
      const int sample_y = base_y + dy;
      if (!IsBounded(Width{sample_x}, Height{sample_y})) {
        continue;
      }

      shadow += SampleShadow(Width{sample_x}, Height{sample_y}, p.z());
      ++count;
    }
  }

  if (count == 0) {
    return kMaxIntensity;
  }

  return shadow / count;
}

Scalar ShadowMapLight::CalculateFading(XAxis x, YAxis y) const {
  Scalar x_norm = x / kDimension * kScreenToNdcScale - kMaxIntensity;
  Scalar y_norm = y / kDimension * kScreenToNdcScale - kMaxIntensity;
  return kMaxIntensity -
         std::clamp(std::sqrt(x_norm * x_norm + y_norm * y_norm), kMinIntensity,
                    kMaxIntensity);
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
  return Point4(point.x(), point.y(), point.z(), kMaxIntensity);
}

bool ShadowMapLight::IsBounded(Width x, Height y) const {
  return x >= 0 && y >= 0 && x < kDimension && y < kDimension;
}

}  // namespace renderer::kernel
