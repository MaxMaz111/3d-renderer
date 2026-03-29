#include "rasterizer.h"

#include <limits>
#include <tbb/parallel_for.h>

#include "color.h"

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
    const std::vector<DirectionalLight>& lights,
    const std::vector<ShadowMapLight>& shadow_lights) {
  for (auto& mesh : meshes) {
    Rasterize(std::move(mesh), camera, lights, shadow_lights);
  }
  return frame_;
}

void Rasterizer::Rasterize(Mesh&& mesh, const Camera& camera,
                           const std::vector<DirectionalLight>& lights,
                           const std::vector<ShadowMapLight>& shadow_lights) {
  for (const Triangle& triangle : mesh.triangles) {
    Rasterize(triangle, camera, lights, shadow_lights, mesh.diffuse_texture);
  }
}

void Rasterizer::Rasterize(const Triangle& triangle, const Camera& camera,
                           const std::vector<DirectionalLight>& lights,
                           const std::vector<ShadowMapLight>& shadow_lights,
                           const Texture& diffuse_texture) {
  BBox bbox = triangle.GetBoundingBox(z_buffer_);
  for (int16_t j = bbox.min_y; j <= bbox.max_y; ++j) {
    QRgb* scanline = frame_.ScanLine(Height{j});
    for (int16_t i = bbox.min_x; i <= bbox.max_x; ++i) {
      Scalar x = i + 0.5;
      Scalar y = j + 0.5;
      auto z = triangle.InterpolateZ(XAxis{x}, YAxis{y});
      if (z == std::numeric_limits<Scalar>::infinity()) {
        continue;
      }
      switch (camera.CurrentRenderingMode()) {
        case Camera::RenderingMode::AllSolid: {
          Scalar& z_buffer_value = z_buffer_.Get(Width{i}, Height{j});
          if (z < z_buffer_value) {
            scanline[i] = triangle.InterpolateColor(
                XAxis{x}, YAxis{y}, lights, shadow_lights, diffuse_texture);
            z_buffer_value = z;
          }
          break;
        }
        case Camera::RenderingMode::AllTransparent: {
          Color::Blend(
              &scanline[i],
              triangle.InterpolateColor(XAxis{x}, YAxis{y}, lights,
                                        shadow_lights, diffuse_texture),
              kBlendFactor);
          break;
        }
      }
    }
  }
}

}  // namespace renderer::kernel
