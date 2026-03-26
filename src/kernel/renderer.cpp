#include "renderer.h"

#include <algorithm>
#include <spdlog/spdlog.h>
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
  for (auto& light : lights) {
    light.RotateAndMove(mat, translation);
  }
  return lights;
}

std::vector<Mesh> Renderer::RotateAndMove(std::vector<Mesh>&& meshes,
                                          const Camera& camera) const {
  Matrix3 mat = camera.RotationMatrix().transpose();
  Point3 translation = -camera.Position();
  for (auto& mesh : meshes) {
    for (auto& triangle : mesh.triangles) {
      triangle.RotateAndMove(mat, translation);
    }
  }
  return meshes;
}

std::vector<Mesh> Renderer::Clip(std::vector<Mesh>&& meshes,
                                 const Camera& camera) const {
  for (auto& mesh : meshes) {
    mesh.triangles = ClipTriangles(std::move(mesh.triangles), camera);
  }
  return meshes;
}

std::vector<Mesh> Renderer::Project(std::vector<Mesh>&& meshes,
                                    const Camera& camera) const {
  Matrix4 mat = camera.ProjectionMatrix();
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
                                              const Camera& camera) const {
  const auto& planes = camera.PlanesForClipping();

  std::vector<Triangle> result;
  result.reserve(triangles.size() * 3);

  std::vector<Triangle> current_buffer;
  std::vector<Triangle> next_buffer;

  for (const Triangle& triangle : triangles) {
    current_buffer.clear();
    current_buffer.push_back(triangle);
    for (const Plane& plane : planes) {
      if (current_buffer.empty()) {
        break;
      }
      next_buffer.clear();
      for (const Triangle& current_triangle : current_buffer) {
        std::ranges::move(ClipTriangleByPlane(current_triangle, plane),
                          std::back_inserter(next_buffer));
      }
      current_buffer.swap(next_buffer);
    }
    std::ranges::move(current_buffer, std::back_inserter(result));
  }
  return result;
}

std::vector<Triangle> Renderer::ClipTriangleByPlane(const Triangle& triangle,
                                                    const Plane& plane) const {
  auto [inside, outside] = SplitVertices(triangle, plane);
  if (inside.size() == 3) {
    return {std::move(triangle)};
  }
  if (inside.size() == 0) {
    return {};
  }
  std::vector<Triangle::Vertex> intersections;
  for (const auto& inside_vertex : inside) {
    for (const auto& outside_vertex : outside) {
      intersections.push_back(
          plane.LineIntersection(inside_vertex, outside_vertex));
    }
  }
  if (intersections.size() < 2) {
    return {};
  }
  if (inside.size() == 1) {
    Triangle result({std::move(inside[0]), std::move(intersections[0]),
                     std::move(intersections[1])});
    return {std::move(result)};
  }
  return {Triangle({std::move(inside[0]), std::move(inside[1]),
                    std::move(intersections[0])}),
          Triangle({std::move(inside[1]), std::move(intersections[0]),
                    std::move(intersections[1])})};
}

Renderer::Split Renderer::SplitVertices(const Triangle& triangle,
                                        const Plane& plane) const {
  Split result;
  result.inside.reserve(3);

  const auto& vertices = triangle.Vertices();

  for (int i = 0; i < 3; ++i) {
    if (plane.IsOnTheSameSideAsNormal(vertices[i].point)) {
      result.inside.push_back(vertices[i]);
    } else {
      result.outside.push_back(vertices[i]);
    }
  }
  return result;
}

}  // namespace renderer::kernel
