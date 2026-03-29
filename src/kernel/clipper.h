#pragma once

#include <tbb/enumerable_thread_specific.h>
#include <tbb/parallel_for.h>

#include "camera.h"
#include "mesh.h"
#include "plane.h"

namespace renderer::kernel {

class Clipper {
 public:
  struct Split {
    std::vector<Vertex> inside;
    std::vector<Vertex> outside;
  };

  template <std::size_t N>
  static std::vector<Mesh> Clip(std::vector<Mesh>&& meshes,
                                const std::array<Plane, N>& planes) {
    tbb::parallel_for(tbb::blocked_range<size_t>(0, meshes.size()),
                      [&](const tbb::blocked_range<size_t>& range) {
                        for (size_t i = range.begin(); i < range.end(); ++i) {
                          meshes[i].triangles = ClipTriangles(
                              std::move(meshes[i].triangles), planes);
                        }
                      });
    return meshes;
  }

 private:
  struct Cache {
    std::vector<Triangle> clipped_triangles;
    std::vector<Vertex> intersection_vertices;
    Split split;
  };

  template <std::size_t N>
  static std::vector<Triangle> ClipTriangles(
      std::vector<Triangle>&& triangles, const std::array<Plane, N>& planes) {
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
                          std::ranges::move(current,
                                            std::back_inserter(output));
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

  static void ClipTriangleByPlane(const Triangle& triangle, const Plane& plane);
  static void SplitVertices(const Triangle& triangle, const Plane& plane);

  static inline thread_local Cache cache;
};

}  // namespace renderer::kernel
