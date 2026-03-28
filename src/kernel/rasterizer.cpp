#include "rasterizer.h"

#include <limits>

#include "kernel/color.h"
#include "util/time_anchor.h"
#include <spdlog/spdlog.h>
#include <tbb/parallel_for.h>

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
  util::TimeAnchor anchor("Rasterization time", [](const std::string& name, double time) {
    spdlog::info("{}: {:.2f} ms", name, time);
  });
  for (auto& mesh : meshes) {
    Rasterize(std::move(mesh), camera, lights);
  }
  return frame_;
}

Rasterizer::BBox Rasterizer::GetBoundingBox(const Triangle& triangle) {
  BBox bbox;
  bbox.min_x = std::floor(triangle.GetMinX());
  bbox.min_x = std::max(bbox.min_x, static_cast<int16_t>(0));
  bbox.max_x = std::ceil(triangle.GetMaxX());
  bbox.max_x = std::min(bbox.max_x, static_cast<int16_t>(frame_.Width() - 1));
  bbox.min_y = std::floor(triangle.GetMinY());
  bbox.min_y = std::max(bbox.min_y, static_cast<int16_t>(0));
  bbox.max_y = std::ceil(triangle.GetMaxY());
  bbox.max_y = std::min(bbox.max_y, static_cast<int16_t>(frame_.Height() - 1));
  return bbox;
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
  BBox bbox = GetBoundingBox(triangle);
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
            scanline[i] = triangle.InterpolateColor(XAxis{x}, YAxis{y}, lights,
                                                    diffuse_texture);
            z_buffer_value = z;
          }
          break;
        }
        case Camera::RenderingMode::AllTransparent: {
          Color::Blend(&scanline[i],
                       triangle.InterpolateColor(XAxis{x}, YAxis{y}, lights,
                                                 diffuse_texture),
                       kBlendFactor);
          break;
        }
      }
    }
  }
}

}  // namespace renderer::kernel
