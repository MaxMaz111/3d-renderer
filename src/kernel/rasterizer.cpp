#include "rasterizer.h"

namespace renderer::kernel {

Rasterizer::Rasterizer(Width width, Height height)
    : width_(width()),
      height_(height()),
      z_buffer_(Width{width_}, Height{height_}),
      frame_(Width{width_}, Height{height_}) {}

void Rasterizer::ResetTo(Width width, Height height) {
  width_ = width();
  height_ = height();
  z_buffer_.ResetTo(width, height);
}

Frame Rasterizer::Rasterize(std::vector<Mesh>&& meshes, const Camera& camera,
                            const std::vector<DirectionalLight>& lights) {
  frame_.ResetTo(Width{width_}, Height{height_});
  z_buffer_.ResetTo(Width{width_}, Height{height_});
  for (auto& mesh : meshes) {
    Rasterize(std::move(mesh), camera, lights);
  }
  return frame_;
}

void Rasterizer::Rasterize(Mesh&& mesh, const Camera& camera,
                           const std::vector<DirectionalLight>& lights) {
  mesh = ConvertToRasterSpace(std::move(mesh), width_, height_);
  for (const Triangle& triangle : mesh.triangles) {
    Rasterize(triangle, camera, lights);
  }
}

void Rasterizer::Rasterize(const Triangle& triangle, const Camera& camera,
                           const std::vector<DirectionalLight>& lights) {
  int min_x = std::floor(triangle.GetMinX());
  int max_x = std::ceil(triangle.GetMaxX());
  int min_y = std::floor(triangle.GetMinY());
  int max_y = std::ceil(triangle.GetMaxY());
  for (int j = std::max(0, min_y); j <= std::min(max_y, height_ - 1); ++j) {
    for (int i = std::max(0, min_x); i <= std::min(max_x, width_ - 1); ++i) {
      UpdateZBuffer(Width{i}, Height{j}, triangle, camera, lights);
    }
  }
}

void Rasterizer::UpdateZBuffer(Width i, Height j, const Triangle& triangle,
                               const Camera& camera,
                               const std::vector<DirectionalLight>& lights) {
  Scalar x = i() + 0.5f, y = j() + 0.5f;
  auto z = triangle.InterpolateZ(XCoordinate{x}, YCoordinate{y});
  if (!z.has_value()) {
    return;
  }
  switch (camera.CurrentRenderingMode()) {
    case Camera::RenderingMode::AllSolid: {
      Scalar& val = z_buffer_.Get(Width{i}, Height{j});
      if (val > *z) {
        val = *z;
        frame_.SetColor(
            Width{i}, Height{j},
            triangle.InterpolateColor(XCoordinate{x}, YCoordinate{y}, lights));
      }
      break;
    }
    case Camera::RenderingMode::AllTransparent: {
      frame_.BlendColor(
          Width{i}, Height{j},
          triangle.InterpolateColor(XCoordinate{x}, YCoordinate{y}, lights));
      break;
    }
    default:
      break;
  }
}

Mesh Rasterizer::ConvertToRasterSpace(Mesh&& mesh, int width, int height) {
  for (auto& triangle : mesh.triangles) {
    for (int i = 0; i < 3; ++i) {
      Vector3 v_ndc = triangle.GetPoint(i);

      Scalar x_rasterized = (v_ndc.x() + 1.0f) * 0.5f * width;
      Scalar y_rasterized = (1.0f - (v_ndc.y() + 1.0f) * 0.5f) * height;
      Scalar z_rasterized = (v_ndc.z() + 1.0f) * 0.5f;

      triangle.GetPoint(i) = Vector3{x_rasterized, y_rasterized, z_rasterized};
    }
  }
  return mesh;
}

}  // namespace renderer::kernel
