#pragma once

#include "kernel/camera.h"

#include "directional_light.h"
#include "frame.h"
#include "mesh.h"
#include "rasterizer.h"
#include "scene.h"
#include "triangle.h"

namespace renderer::kernel {

class Renderer {
  struct Split {
    std::vector<Vertex> inside;
    std::vector<Vertex> outside;
  };

  struct Cache {
    std::vector<Triangle> clipped_triangles;
    std::vector<Vertex> intersection_vertices;
    Split split;
  };

 public:
  Renderer(Width width, Height height);

  void ResetTo(Width width, Height height);
  const Frame& Render(const Scene& scene);

 private:
  std::vector<DirectionalLight> RotateAndMove(
      std::vector<DirectionalLight>&& lights, const Camera& camera) const;
  std::vector<Mesh> RotateAndMove(std::vector<Mesh>&& meshes,
                                  const Camera& camera) const;
  std::vector<Mesh> Clip(std::vector<Mesh>&& meshes, const Camera& camera);
  std::vector<Mesh> Project(std::vector<Mesh>&& meshes,
                            const Camera& camera) const;
  const Frame& Rasterize(std::vector<Mesh>&& meshes, const Camera& camera,
                         const std::vector<DirectionalLight>& lights);
  std::vector<Triangle> ClipTriangles(std::vector<Triangle>&& triangles,
                                      const Camera& camera);
  void ClipTriangleByPlane(const Triangle& triangle, const Plane& plane);
  void SplitVertices(const Triangle& triangle, const Plane& plane);

  Rasterizer rasterizer_;

  static inline thread_local Cache cache;
};

}  // namespace renderer::kernel
