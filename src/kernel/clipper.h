#pragma once

#include <array>
#include <cstddef>
#include <iterator>
#include <tbb/enumerable_thread_specific.h>
#include <tbb/parallel_for.h>
#include <vector>

#include "mesh.h"
#include "plane.h"

namespace renderer::kernel {

class Clipper {
  static constexpr size_t kTriangleVertexCount = 3;
  static constexpr size_t kClippedTriangleCountHint = 2;
  static constexpr size_t kMinIntersectionVertices = 2;
  static constexpr int kGrainSize = 4096;

 public:
  struct Split {
    std::array<Vertex, kTriangleVertexCount> inside;
    std::array<Vertex, kTriangleVertexCount> outside;
    size_t inside_count;
    size_t outside_count;
  };

  template <size_t N>
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
    std::array<Vertex, kClippedTriangleCountHint> intersection_vertices;
    Split split;
  };

  template <size_t N>
  static std::vector<Triangle> ClipTriangles(
      std::vector<Triangle>&& triangles, const std::array<Plane, N>& planes) {
    tbb::enumerable_thread_specific<std::vector<Triangle>> tls_output;
    tbb::parallel_for(
        tbb::blocked_range<size_t>(0, triangles.size(), kGrainSize),
        [&](const tbb::blocked_range<size_t>& range) {
          auto& output = tls_output.local();
          std::vector<Triangle> current;
          std::vector<Triangle> next;
          current.reserve(kClippedTriangleCountHint);
          next.reserve(kClippedTriangleCountHint);
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
                next.insert(
                    next.end(),
                    std::make_move_iterator(cache.clipped_triangles.begin()),
                    std::make_move_iterator(cache.clipped_triangles.end()));
              }
              current.swap(next);
            }
            std::ranges::move(current, std::back_inserter(output));
          }
        });
    size_t total = 0;
    for (const auto& local : tls_output) {
      total += std::size(local);
    }

    std::vector<Triangle> result;
    result.reserve(total);

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
