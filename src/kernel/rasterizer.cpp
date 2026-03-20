#include "rasterizer.h"

namespace renderer::kernel {

Rasterizer::Rasterizer(Width width, Height height)
    : z_buffer_(Width{width}, Height{height}),
      frame_(Width{width}, Height{height}) {}

void Rasterizer::Clear() {
  z_buffer_.Clear();
  frame_.Clear();
}

void Rasterizer::ResetTo(Width width, Height height) {
  z_buffer_.ResetTo(width, height);
  frame_.ResetTo(width, height);
}

const Frame& Rasterizer::Rasterize(
    std::vector<Mesh>&& meshes, const Camera& camera,
    const std::vector<DirectionalLight>& lights) {
  for (auto& mesh : meshes) {
    Rasterize(std::move(mesh), camera, lights);
  }
  return frame_;
}

void Rasterizer::Rasterize(Mesh&& mesh, const Camera& camera,
                           const std::vector<DirectionalLight>& lights) {
  mesh = ConvertToRasterSpace(std::move(mesh));
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
  for (int j = std::max(0, min_y); j <= std::min(max_y, frame_.Height() - 1);
       ++j) {
    for (int i = std::max(0, min_x); i <= std::min(max_x, frame_.Width() - 1);
         ++i) {
      UpdateZBuffer(Width{i}, Height{j}, triangle, camera, lights);
    }
  }
}

void Rasterizer::UpdateZBuffer(Width i, Height j, const Triangle& triangle,
                               const Camera& camera,
                               const std::vector<DirectionalLight>& lights) {
  Scalar x = i + 0.5, y = j + 0.5;
  auto z = triangle.InterpolateZ(XCoordinate{x}, YCoordinate{y});
  if (!z.has_value()) {
    return;
  }
  auto color =
      triangle.InterpolateColor(XCoordinate{x}, YCoordinate{y}, lights);
  switch (camera.CurrentRenderingMode()) {
    case Camera::RenderingMode::AllSolid: {
      Scalar& val = z_buffer_.Get(i, j);
      if (val > *z) {
        val = *z;
        frame_.SetColor(i, j, color);
      }
      break;
    }
    case Camera::RenderingMode::AllTransparent: {
      frame_.BlendColor(i, j, color);
      break;
    }
    default:
      break;
  }
}

Mesh Rasterizer::ConvertToRasterSpace(Mesh&& mesh) const {
  for (auto& triangle : mesh.triangles) {
    for (int i = 0; i < 3; ++i) {
      Vector3& v = triangle.GetPoint(i);

      v.x() = (v.x() + 1) * 0.5 * frame_.Width();
      v.y() = (1 - (v.y() + 1) * 0.5) * frame_.Height();
      v.z() = (v.z() + 1) * 0.5;
    }
  }
  return mesh;
}

}  // namespace renderer::kernel
