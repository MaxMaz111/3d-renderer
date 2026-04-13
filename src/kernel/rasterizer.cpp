#include "rasterizer.h"

#include <limits>
#include <spdlog/spdlog.h>
#include <tbb/parallel_for.h>

#include "util/size.h"
#include "util/time_anchor.h"

#include "light_sample.h"

namespace renderer::kernel {

Rasterizer::Rasterizer(Width width, Height height)
    : z_buffer_(Width{width}, Height{height}),
      frame_(Width{width}, Height{height}),
      tile_mutex_manager_(width, height) {}

void Rasterizer::Clear() {
  z_buffer_.Clear();
  frame_.Clear();
}

void Rasterizer::ResetTo(Width width, Height height) {
  z_buffer_.ResetTo(width, height);
  frame_.ResetTo(width, height);
  tile_mutex_manager_.ResetTo(width, height);
}

void Rasterizer::ToggleHDR() {
  frame_.ToggleHDR();
}

const Frame& Rasterizer::Rasterize(
    std::vector<Mesh>&& meshes, const Camera& camera,
    const std::vector<DirectionalLight>& lights,
    const std::vector<ShadowMapLight>& shadow_lights) {
  util::TimeAnchor anchor("Rasterization time",
                          [](const std::string& name, double time) {
                            spdlog::info("{}: {:.2f} ms", name, time);
                          });
  tbb::parallel_for(tbb::blocked_range<size_t>(0, meshes.size()),
                    [&](const tbb::blocked_range<size_t>& range) {
                      for (size_t i = range.begin(); i < range.end(); ++i) {
                        Rasterize(std::move(meshes[i]), camera, lights,
                                  shadow_lights);
                      }
                    });
  return frame_;
}

void Rasterizer::Rasterize(Mesh&& mesh, const Camera& camera,
                           const std::vector<DirectionalLight>& lights,
                           const std::vector<ShadowMapLight>& shadow_lights) {
  tbb::parallel_for(tbb::blocked_range<size_t>(0, mesh.triangles.size(),
                                               kParallelGranularity),
                    [&](const tbb::blocked_range<size_t>& triangle_range) {
                      for (size_t j = triangle_range.begin();
                           j < triangle_range.end(); ++j) {
                        Rasterize(mesh.triangles[j], camera, lights,
                                  shadow_lights, mesh.diffuse_texture);
                      }
                    });
}

void Rasterizer::Rasterize(const Triangle& triangle, const Camera& camera,
                           const std::vector<DirectionalLight>& lights,
                           const std::vector<ShadowMapLight>& shadow_lights,
                           const Texture& diffuse_texture) {
  BBox bbox =
      triangle.GetBoundingBox(Width{frame_.Width()}, Height{frame_.Height()});
  switch (camera.CurrentRenderingMode()) {
    case Camera::RenderingMode::AllSolid: {
      for (int j = bbox.min_y; j <= bbox.max_y; ++j) {
        for (int i = bbox.min_x; i <= bbox.max_x; ++i) {
          UpdateAllSolid(Width{i}, Height{j}, triangle, lights, shadow_lights,
                         diffuse_texture);
        }
      }
      break;
    }
    case Camera::RenderingMode::AllTransparent: {
      for (int j = bbox.min_y; j <= bbox.max_y; ++j) {
        for (int i = bbox.min_x; i <= bbox.max_x; ++i) {
          UpdateAllTransparent(Width{i}, Height{j}, triangle, lights,
                               shadow_lights, diffuse_texture);
        }
      }
      break;
    }
  }
}

void Rasterizer::UpdateAllSolid(
    Width i, Height j, const Triangle& triangle,
    const std::vector<DirectionalLight>& lights,
    const std::vector<ShadowMapLight>& shadow_lights,
    const Texture& diffuse_texture) {
  Scalar x = i + 0.5;
  Scalar y = j + 0.5;
  auto z = triangle.InterpolateZ(XAxis{x}, YAxis{y});
  if (z == std::numeric_limits<Scalar>::infinity()) {
    return;
  }

  tbb::spin_mutex::scoped_lock lock(tile_mutex_manager_.GetMutexFor(i, j));
  Scalar& z_buffer_value = z_buffer_.Get(Width{i}, Height{j});
  if (z < z_buffer_value) {
    auto sample = triangle.InterpolateColor(XAxis{x}, YAxis{y}, lights,
                                            shadow_lights, diffuse_texture);
    frame_.SetColor(Width{i}, Height{j}, sample);
    z_buffer_value = z;
  }
}

void Rasterizer::UpdateAllTransparent(
    Width i, Height j, const Triangle& triangle,
    const std::vector<DirectionalLight>& lights,
    const std::vector<ShadowMapLight>& shadow_lights,
    const Texture& diffuse_texture) {
  Scalar x = i + 0.5;
  Scalar y = j + 0.5;
  auto z = triangle.InterpolateZ(XAxis{x}, YAxis{y});
  if (z == std::numeric_limits<Scalar>::infinity()) {
    return;
  }
  auto sample = triangle.InterpolateColor(XAxis{x}, YAxis{y}, lights,
                                          shadow_lights, diffuse_texture);

  tbb::spin_mutex::scoped_lock lock(tile_mutex_manager_.GetMutexFor(i, j));
  frame_.AddColor(Width{i}, Height{j}, sample);
}

}  // namespace renderer::kernel
