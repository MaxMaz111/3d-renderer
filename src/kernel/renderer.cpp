#include "renderer.h"

#include <algorithm>
#include <spdlog/spdlog.h>
#include <tbb/blocked_range.h>
#include <tbb/enumerable_thread_specific.h>
#include <tbb/parallel_for.h>
#include <vector>

#include "util/time_anchor.h"

#include "directional_light.h"
#include "linalg.h"
#include "mesh.h"
#include "triangle.h"

namespace renderer::kernel {

Renderer::Renderer(Width width, Height height) : rasterizer_(width, height) {}

void Renderer::ResetTo(Width width, Height height) {
  rasterizer_.ResetTo(width, height);
}

const Frame& Renderer::Render(const Scene& scene) {
  util::TimeAnchor anchor("Rendering frametime",
                          [](const std::string& name, double time) {
                            spdlog::info("{}: {:.2f} ms", name, time);
                          });
  rasterizer_.Clear();
  std::vector<Mesh> meshes = scene.Meshes();
  const Camera& camera = scene.Camera();
  std::vector<DirectionalLight> lights = scene.DirectionalLights();
  lights = RotateAndMove(std::move(lights), camera);
  meshes = RotateAndMove(std::move(meshes), camera);
  meshes = Clip(std::move(meshes), camera);
  meshes = Project(std::move(meshes), camera);
  return Rasterize(std::move(meshes), camera, lights);
}

std::vector<DirectionalLight> Renderer::RotateAndMove(
    std::vector<DirectionalLight>&& lights, const Camera& camera) const {
  Matrix3 mat = camera.RotationMatrix().transpose();
  Point3 translation = -camera.Position();
  tbb::parallel_for(tbb::blocked_range<size_t>(0, lights.size(), 4096),
                    [&](const tbb::blocked_range<size_t>& range) {
                      for (size_t i = range.begin(); i < range.end(); ++i) {
                        lights[i].RotateAndMove(mat, translation);
                      }
                    });
  return lights;
}

std::vector<Mesh> Renderer::RotateAndMove(std::vector<Mesh>&& meshes,
                                          const Camera& camera) const {
  Matrix3 mat = camera.RotationMatrix().transpose();
  Point3 translation = -camera.Position();
  for (auto& mesh : meshes) {
    tbb::parallel_for(
        tbb::blocked_range<size_t>(0, mesh.triangles.size(), 4096),
        [&](const tbb::blocked_range<size_t>& range) {
          for (size_t i = range.begin(); i < range.end(); ++i) {
            mesh.triangles[i].RotateAndMove(mat, translation);
          }
        });
  }
  return meshes;
}

std::vector<Mesh> Renderer::Clip(std::vector<Mesh>&& meshes,
                                 const Camera& camera) {
  util::TimeAnchor anchor("Clipping time",
                          [](const std::string& name, double time) {
                            spdlog::info("{}: {:.2f} ms", name, time);
                          });
  tbb::parallel_for(tbb::blocked_range<size_t>(0, meshes.size()),
                    [&](const tbb::blocked_range<size_t>& range) {
                      for (size_t i = range.begin(); i < range.end(); ++i) {
                        meshes[i].triangles =
                            ClipTriangles(std::move(meshes[i].triangles), camera);
                      }
                    });
  return meshes;
}

std::vector<Mesh> Renderer::Project(std::vector<Mesh>&& meshes,
                                    const Camera& camera) const {
  const Matrix4& mat = camera.ProjectionMatrix();
  for (auto& mesh : meshes) {
    for (auto& triangle : mesh.triangles) {
      triangle.Project(mat);
    }
  }
  return meshes;
}

const Frame& Renderer::Rasterize(std::vector<Mesh>&& meshes,
                                 const Camera& camera,
                                 const std::vector<DirectionalLight>& lights) {
  return rasterizer_.Rasterize(std::move(meshes), camera, lights);
}

std::vector<Triangle> Renderer::ClipTriangles(std::vector<Triangle>&& triangles,
                                              const Camera& camera) {

  const auto& planes = camera.PlanesForClipping();

  tbb::enumerable_thread_specific<std::vector<Triangle>> tls_output;
  tbb::parallel_for(tbb::blocked_range<size_t>(0, triangles.size(), 4096),
                    [&](const tbb::blocked_range<size_t>& range) {
                      auto& output = tls_output.local();
                      std::vector<Triangle> current;
                      std::vector<Triangle> next;
                      current.reserve(2);
                      next.reserve(2);
                      for (size_t i = range.begin(); i < range.end(); ++i) {
                        if (triangles[i].IsInside(planes)) {
                          output.push_back(std::move(triangles[i]));
                          continue;
                        }
                        current.clear();
                        current.push_back(std::move(triangles[i]));
                        for (const Plane& plane : planes) {
                          if (current.empty()) {
                            break;
                          }
                          next.clear();
                          for (const Triangle& tri : current) {
                            ClipTriangleByPlane(tri, plane);
                            std::ranges::move(cache.clipped_triangles,
                                              std::back_inserter(next));
                          }
                          current.swap(next);
                        }
                        std::ranges::move(current, std::back_inserter(output));
                      }
                    });

  std::vector<Triangle> result;
  size_t total_triangles = 0;
  for (auto& local : tls_output) {
    total_triangles += local.size();
  }
  result.reserve(total_triangles);

  for (auto& local : tls_output) {
    result.insert(result.end(), std::make_move_iterator(local.begin()),
                  std::make_move_iterator(local.end()));
  }
  return result;
}

void Renderer::ClipTriangleByPlane(const Triangle& triangle,
                                   const Plane& plane) {
  SplitVertices(triangle, plane);
  auto& inside = cache.split.inside;
  auto& outside = cache.split.outside;
  auto& result = cache.clipped_triangles;
  result.clear();
  result.reserve(2);

  if (inside.size() == 3) {
    result.push_back(std::move(triangle));
    return;
  }
  if (inside.size() == 0) {
    return;
  }
  cache.intersection_vertices.clear();
  cache.intersection_vertices.reserve(2);
  for (const auto& inside_vertex : inside) {
    for (const auto& outside_vertex : outside) {
      cache.intersection_vertices.push_back(
          plane.LineIntersection(inside_vertex, outside_vertex));
    }
  }
  if (cache.intersection_vertices.size() < 2) {
    return;
  }
  if (inside.size() == 1) {
    result.emplace_back(std::move(inside[0]),
                        std::move(cache.intersection_vertices[0]),
                        std::move(cache.intersection_vertices[1]));
    return;
  }
  result.emplace_back(std::move(inside[0]), std::move(inside[1]),
                      std::move(cache.intersection_vertices[0]));
  result.emplace_back(std::move(inside[1]),
                      std::move(cache.intersection_vertices[0]),
                      std::move(cache.intersection_vertices[1]));
}

void Renderer::SplitVertices(const Triangle& triangle, const Plane& plane) {
  cache.split.inside.reserve(3);
  cache.split.outside.reserve(3);
  cache.split.inside.clear();
  cache.split.outside.clear();
  const auto& vertices = triangle.Vertices();

  for (int i = 0; i < 3; ++i) {
    if (plane.IsOnTheSameSideAsNormal(vertices[i].point)) {
      cache.split.inside.push_back(vertices[i]);
    } else {
      cache.split.outside.push_back(vertices[i]);
    }
  }
}

}  // namespace renderer::kernel
