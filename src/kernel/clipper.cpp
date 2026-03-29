#include "clipper.h"

namespace renderer::kernel {

void Clipper::ClipTriangleByPlane(const Triangle& triangle,
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

void Clipper::SplitVertices(const Triangle& triangle, const Plane& plane) {
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
