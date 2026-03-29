#include "shadow_map_light.h"

#include "kernel/clipper.h"
#include "kernel/directional_light.h"
#include "kernel/mesh.h"

namespace renderer::kernel {

const Point3 ShadowMapLight::kDefaultPosition{5, 0, 0};
const Matrix3 ShadowMapLight::kDefaultRotation =
    Matrix3{AngleAxis(std::numbers::pi_v<Scalar> / 2, Vector3::UnitY())};
const Vector3 ShadowMapLight::kDefaultDirection =
    kDefaultRotation * Vector3{0, 0, -1};

ShadowMapLight::ShadowMapLight()
    : light_(rotation_matrix_ * Vector3{0, 0, -1}),
      projection_matrix_(BuildProjectionMatrix()),
      planes_(BuildPlanesForClipping()) {}

ShadowMapLight::ShadowMapLight(const Point3& position, const Matrix3& rotation)
    : position_(position),
      rotation_matrix_(rotation),
      light_(rotation_matrix_ * Vector3{0, 0, -1}),
      projection_matrix_(BuildProjectionMatrix()),
      planes_(BuildPlanesForClipping()) {}

void ShadowMapLight::UpdateZBuffer(std::vector<Mesh>&& meshes) {
  meshes = RotateAndMove(std::move(meshes));
  meshes = Clipper::Clip(std::move(meshes), planes_);
  meshes = Project(std::move(meshes));
  for (auto& mesh : meshes) {
    for (auto& triangle : mesh.triangles) {
      BBox bbox = triangle.GetBoundingBox(z_buffer_);
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
  if (light_space_point.z() - stored_z < kDefaultBias) {
    Scalar x_norm = light_space_point.x() / kDimension * 2 - 1;
    Scalar y_norm = light_space_point.y() / kDimension * 2 - 1;
    Scalar factor = 1 - std::clamp(std::sqrt(x_norm * x_norm + y_norm * y_norm), 0.f, 1.f);
    return light_.CalculateIntensity(normal) * factor;
  }
  return 0;
}

std::vector<Mesh> ShadowMapLight::RotateAndMove(
    std::vector<Mesh>&& meshes) const {
  const Matrix3& mat = rotation_matrix_.transpose();
  const Point3& translation = -position_;
  for (auto& mesh : meshes) {
    for (auto& triangle : mesh.triangles) {
      triangle.RotateAndMove(mat, translation);
    }
  }
  return meshes;
}

std::vector<Mesh> ShadowMapLight::Project(std::vector<Mesh>&& meshes) const {
  const Matrix4& mat = projection_matrix_;
  for (auto& mesh : meshes) {
    for (auto& triangle : mesh.triangles) {
      triangle.Project(mat);
    }
  }
  return meshes;
}

Point3 ShadowMapLight::TransformToLightSpace(const Point3& world_point) const {
  Point4 clip =
      projection_matrix_ *
      ToHomogeneous(rotation_matrix_.transpose() * (world_point - position_));
  if (std::abs(clip.w()) < kEpsilon) {
    return Point3{std::numeric_limits<Scalar>::infinity(),
                  std::numeric_limits<Scalar>::infinity(),
                  std::numeric_limits<Scalar>::infinity()};
  }
  return Point3(clip.x() / clip.w(), clip.y() / clip.w(), clip.z() / clip.w());
}

Point4 ShadowMapLight::ToHomogeneous(const Point3& point) const {
  return Point4(point.x(), point.y(), point.z(), 1);
}

Matrix4 ShadowMapLight::BuildProjectionMatrix() const {
  Scalar aspect_ratio = 1;
  Scalar t = near_ * std::tan(fov_ * 0.5);
  Scalar b = -t;
  Scalar r = -t * aspect_ratio;
  Scalar l = -r;

  Matrix4 projection{{{2 * near_ / (r - l), 0, (r + l) / (r - l), 0},
                      {0, 2 * near_ / (t - b), (t + b) / (t - b), 0},
                      {0, 0, -(far_ + near_) / (far_ - near_),
                       -2 * far_ * near_ / (far_ - near_)},
                      {0, 0, -1, 0}}};
  Scalar w = kDimension;
  Scalar h = kDimension;
  Matrix4 raster{{{w / 2, 0, 0, w / 2},
                  {0, -h / 2, 0, h / 2},
                  {0, 0, 0.5, 0.5},
                  {0, 0, 0, 1}}};

  return raster * projection;
}

std::array<Plane, ShadowMapLight::kNumberOfPlanes>
ShadowMapLight::BuildPlanesForClipping() const {

  Plane near_plane(Vector3{0, 0, -1}, -near_);
  Plane far_plane(Vector3{0, 0, 1}, far_);

  Scalar half_height = near_ * std::tan(fov_ * 0.5f);
  Scalar aspect_ratio = 1;
  Scalar half_width = half_height * aspect_ratio;

  Plane left_plane(Vector3{-near_, 0, -half_width}, 0);
  Plane right_plane(Vector3{near_, 0, -half_width}, 0);
  Plane top_plane(Vector3{0, -near_, -half_height}, 0);
  Plane bottom_plane(Vector3{0, near_, -half_height}, 0);

  return {near_plane, left_plane,   right_plane,
          top_plane,  bottom_plane, far_plane};
}

}  // namespace renderer::kernel
