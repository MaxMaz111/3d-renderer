#include "renderer.h"

#include <spdlog/spdlog.h>
#include <tbb/blocked_range.h>
#include <tbb/enumerable_thread_specific.h>
#include <tbb/parallel_for.h>
#include <vector>

#include "util/time_anchor.h"

#include "clipper.h"
#include "directional_light.h"
#include "linalg.h"
#include "mesh.h"
#include "shadow_map_light.h"
#include "triangle.h"

namespace renderer::kernel {

Renderer::Renderer(Width width, Height height) : rasterizer_(width, height) {}

void Renderer::ResetTo(Width width, Height height) {
  rasterizer_.ResetTo(width, height);
}

void Renderer::ToggleHDR() {
  rasterizer_.ToggleHDR();
}

const Frame& Renderer::Render(const Scene& scene) {
  util::TimeAnchor anchor("Rendering frametime",
                          [](const std::string& name, double time) {
                            spdlog::info("{}: {:.2f} ms", name, time);
                          });

  rasterizer_.Clear();

  const auto& camera = scene.Camera();
  auto meshes = scene.Meshes();
  auto lights = scene.DirectionalLights();
  auto shadow_lights = scene.ShadowMapLights();
  shadow_lights = MoveToLocal(std::move(shadow_lights), camera);
  lights = MoveToLocal(std::move(lights), camera);
  meshes = camera.MoveToLocal(std::move(meshes));
  meshes = Clipper::Clip(std::move(meshes), camera.PlanesForClipping());
  meshes = camera.Project(std::move(meshes));
  return Rasterize(std::move(meshes), camera, lights, shadow_lights);
}

std::vector<DirectionalLight> Renderer::MoveToLocal(
    std::vector<DirectionalLight>&& lights, const Camera& camera) const {
  const Matrix3 mat = camera.RotationMatrix().transpose();
  const Point3 translation = -camera.Position();
  tbb::parallel_for(tbb::blocked_range<size_t>(0, lights.size(), 4096),
                    [&](const tbb::blocked_range<size_t>& range) {
                      for (size_t i = range.begin(); i < range.end(); ++i) {
                        lights[i].RotateAndMove(mat, translation);
                      }
                    });
  return lights;
}

std::vector<ShadowMapLight> Renderer::MoveToLocal(
    std::vector<ShadowMapLight>&& lights, const Camera& camera) const {
  const Matrix3 mat = camera.RotationMatrix().transpose();
  const Point3 translation = -camera.Position();
  tbb::parallel_for(tbb::blocked_range<size_t>(0, lights.size(), 4096),
                    [&](const tbb::blocked_range<size_t>& range) {
                      for (size_t i = range.begin(); i < range.end(); ++i) {
                        lights[i].RotateAndMove(mat, translation);
                      }
                    });
  return lights;
}

const Frame& Renderer::Rasterize(
    std::vector<Mesh>&& meshes, const Camera& camera,
    const std::vector<DirectionalLight>& lights,
    const std::vector<ShadowMapLight>& shadow_lights) {
  return rasterizer_.Rasterize(std::move(meshes), camera, lights,
                               shadow_lights);
}

}  // namespace renderer::kernel
