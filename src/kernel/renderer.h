#pragma once

#include <QPixmap>

#include "kernel/camera.h"

#include "directional_light.h"
#include "frame.h"
#include "mesh.h"
#include "rasterizer.h"
#include "scene.h"
#include "triangle.h"

namespace renderer::kernel {

class Renderer {
 public:
  Renderer(Width width, Height height);

  void ResetTo(Width width, Height height);
  Frame Render(const Scene& scene);

 private:
  std::vector<DirectionalLight> RotateAndMove(
      std::vector<DirectionalLight>&& lights, const Camera& camera) const;
  std::vector<Mesh> RotateAndMove(std::vector<Mesh>&& meshes,
                                  const Camera& camera) const;
  std::vector<Mesh> Clip(std::vector<Mesh>&& meshes,
                         const Camera& camera) const;
  std::vector<Mesh> Project(std::vector<Mesh>&& meshes,
                            const Camera& camera) const;
  Frame Rasterize(std::vector<Mesh>&& meshes, const Camera& camera,
                  const std::vector<DirectionalLight>& lights);
  std::vector<Triangle> ClipTriangles(std::vector<Triangle>&& triangles,
                                      const Camera& camera) const;
  std::vector<Triangle> ClipTriangleByPlane(const Triangle& triangle,
                                            const Plane& plane) const;
  std::pair<std::vector<Triangle::Vertex>, std::vector<Triangle::Vertex>>
  SplitVertices(const Triangle& triangle, const Plane& plane) const;

  Rasterizer rasterizer_;
};

}  // namespace renderer::kernel
