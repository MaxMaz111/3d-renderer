#include "rasterizer.h"

#include <limits>

#include "kernel/color.h"

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
  for (const Triangle& triangle : mesh.triangles) {
    Rasterize(triangle, camera, lights, mesh.diffuse_texture);
  }
}

void Rasterizer::Rasterize(const Triangle& triangle, const Camera& camera,
                           const std::vector<DirectionalLight>& lights,
                           const Texture& diffuse_texture) {
  const int min_x = std::floor(triangle.GetMinX());
  const int max_x = std::ceil(triangle.GetMaxX());
  const int min_y = std::floor(triangle.GetMinY());
  const int max_y = std::ceil(triangle.GetMaxY());
  for (int j = min_y; j <= max_y; ++j) {
    QRgb* scanline = frame_.ScanLine(Height{j});
    for (int i = min_x; i <= max_x; ++i) {
      auto z = triangle.InterpolateZ(XAxis{i}, YAxis{j});
      if (z == std::numeric_limits<Scalar>::infinity()) {
        continue;
      }
      switch (camera.CurrentRenderingMode()) {
        case Camera::RenderingMode::AllSolid: {
          Scalar& z_buffer_value = z_buffer_.Get(Width{i}, Height{j});
          if (z < z_buffer_value) {
            scanline[i] = triangle.InterpolateColor(XAxis{i}, YAxis{j}, lights,
                                                    diffuse_texture);
            z_buffer_value = z;
          }
          break;
        }
        case Camera::RenderingMode::AllTransparent: {
          Color::Blend(&scanline[i],
                       triangle.InterpolateColor(XAxis{i}, YAxis{j}, lights,
                                                 diffuse_texture),
                       kBlendFactor);
          break;
        }
      }
    }
  }
}

}  // namespace renderer::kernel
