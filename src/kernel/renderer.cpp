#include "renderer.h"

#include <vector>

#include "directional_light.h"
#include "linalg.h"
#include "mesh.h"
#include "triangle.h"

namespace renderer::kernel {

Renderer::Renderer(Width width, Height height) : rasterizer_(width, height) {}

void Renderer::ResetTo(Width width, Height height) {
  rasterizer_.ResetTo(width, height);
}

Frame Renderer::Render(const Scene& scene) {
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

Frame Renderer::Rasterize(std::vector<Mesh>&& meshes, const Camera& camera,
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
    return {triangle};
  }
  if (inside.size() == 0) {
    return {};
  }
  std::vector<Triangle::Vertex> intersections;
  for (const auto& inside_vertex : inside) {
    for (const auto& outside_vertex : outside) {
      auto intersection = plane.LineIntersection(inside_vertex, outside_vertex);
      if (intersection.has_value()) {
        intersections.push_back(*intersection);
      }
    }
  }
  if (intersections.size() < 2) {
    return {};
  }
  if (inside.size() == 1) {
    return {Triangle({inside[0], intersections[0], intersections[1]})};
  }
  return {Triangle({inside[0], inside[1], intersections[0]}),
          Triangle({inside[1], intersections[0], intersections[1]})};
}

std::pair<std::vector<Triangle::Vertex>, std::vector<Triangle::Vertex>>
Renderer::SplitVertices(const Triangle& triangle, const Plane& plane) const {
  std::vector<Triangle::Vertex> inside;
  std::vector<Triangle::Vertex> outside;

  const auto& vertices = triangle.Vertices();

  for (int i = 0; i < 3; ++i) {
    if (plane.IsOnTheSameSideAsNormal(vertices[i].point)) {
      inside.push_back(vertices[i]);
    } else {
      outside.push_back(vertices[i]);
    }
  }
  return {inside, outside};
}

}  // namespace renderer::kernel
