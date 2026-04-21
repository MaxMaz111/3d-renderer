#include "clipper.h"

namespace renderer::kernel {

void Clipper::ClipTriangleByPlane(const Triangle& triangle,
                                  const Plane& plane) {
  SplitVertices(triangle, plane);
  auto& inside = cache.split.inside;
  auto& outside = cache.split.outside;
  const size_t inside_count = cache.split.inside_count;
  const size_t outside_count = cache.split.outside_count;
  auto& result = cache.clipped_triangles;
  result.clear();
  result.reserve(kClippedTriangleCountHint);

  if (inside_count == kTriangleVertexCount) {
    result.push_back(triangle);
    return;
  }
  if (inside_count == 0) {
    return;
  }
  size_t intersection_count = 0;
  for (size_t i = 0; i < inside_count; ++i) {
    for (size_t j = 0; j < outside_count; ++j) {
      cache.intersection_vertices[intersection_count++] =
          plane.LineIntersection(inside[i], outside[j]);
    }
  }
  if (intersection_count < kMinIntersectionVertices) {
    return;
  }
  if (inside_count == 1) {
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

void Clipper::SplitVertices(const Triangle& triangle, const Plane& plane) {
  cache.split.inside_count = 0;
  cache.split.outside_count = 0;
  const auto& vertices = triangle.Vertices();

  for (size_t i = 0; i < kTriangleVertexCount; ++i) {
    if (plane.IsOnTheSameSideAsNormal(vertices[i].point)) {
      cache.split.inside[cache.split.inside_count++] = vertices[i];
    } else {
      cache.split.outside[cache.split.outside_count++] = vertices[i];
    }
  }
}

}  // namespace renderer::kernel
