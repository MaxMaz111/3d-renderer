#include "renderer.h"

#include <QDebug>
#include <vector>

#include "triangle.h"

namespace renderer {

QPixmap Renderer::Render(const Scene& scene) {
  const std::vector<Triangle>& triangles = scene.GetTriangles();
  const Camera& camera = scene.GetCamera();
  auto rotated_triangles = GetRotatedTriangles(triangles, camera);
  qDebug() << rotated_triangles.size();
  rotated_triangles[0].Print();
  auto clipped_triangles = GetClippedTriangles(rotated_triangles, camera);
  qDebug() << clipped_triangles.size();
  auto projected_triangles = GetProjectedTriangles(clipped_triangles, camera);
  int width = static_cast<int>(camera.GetWidth());
  int height = static_cast<int>(camera.GetHeight());
  int z_buffer_size = width * height;
  if (z_buffer_.size() != z_buffer_size) {
    z_buffer_.resize(z_buffer_size);
  }
  std::fill(z_buffer_.begin(), z_buffer_.end(), 1);
  to_return_ = QImage(width, height, QImage::Format_RGB32);
  to_return_.fill(Qt::black);
  for (int i = 0; i < z_buffer_size; ++i) {
    Scalar x = i % width;
    Scalar y = i / width;
    x += 0.5;
    y += 0.5;
    for (const auto& triangle : projected_triangles) {
      auto z = triangle.GetZ({x, y, 0});
      if (z == std::nullopt || z.value() > z_buffer_[i]) {
        continue;
      }
      to_return_.setPixelColor(
          x, y,
          {triangle.GetColor().GetRed(), triangle.GetColor().GetGreen(),
           triangle.GetColor().GetBlue()});
      z_buffer_[i] = z.value();
    }
  }
  return QPixmap::fromImage(to_return_);
}

std::vector<Triangle> Renderer::GetClippedTriangles(
    const std::vector<Triangle>& triangles, const Camera& camera) const {
  std::vector<Triangle> clipped_triangles;
  for (const Triangle& triangle : triangles) {
    auto new_triangles = ClipTriangle(triangle, camera);
    clipped_triangles.insert(clipped_triangles.end(), new_triangles.begin(),
                             new_triangles.end());
  }
  return clipped_triangles;
}

std::vector<Triangle> Renderer::ClipTriangle(const Triangle& triangle,
                                             const Camera& camera) const {
  std::vector<Point3> vertices = {triangle.GetPoints()[0],
                                  triangle.GetPoints()[1],
                                  triangle.GetPoints()[2]};

  for (const Plane& plane : camera.GetPlanesForClipping()) {
    vertices = ClipPolygonByPlane(vertices, plane, triangle.GetColor());
    if (vertices.empty()) {
      return {};
    }
  }

  std::vector<Triangle> result;
  if (vertices.size() >= 3) {
    for (size_t i = 1; i < vertices.size() - 1; ++i) {
      result.push_back(Triangle(vertices[0], vertices[i], vertices[i + 1]));
    }
  }

  return result;
}

std::vector<Point3> Renderer::ClipPolygonByPlane(
    const std::vector<Point3>& vertices, const Plane& plane,
    const Color& color) const {
  if (vertices.empty()) {
    return {};
  }

  std::vector<Point3> result;

  // Implement Sutherland-Hodgman clipping algorithm
  Point3 prev_vertex = vertices.back();  // Start with the last vertex
  bool prev_inside = plane.IsOnTheSameSideAsNormal(prev_vertex);

  for (const Point3& curr_vertex : vertices) {
    bool curr_inside = plane.IsOnTheSameSideAsNormal(curr_vertex);

    // If there's a transition between inside and outside, calculate the intersection
    if (curr_inside != prev_inside) {
      Vector3 direction = curr_vertex - prev_vertex;
      auto intersection = plane.LineIntersection(prev_vertex, direction);

      if (intersection.has_value()) {
        result.push_back(intersection.value());
      }
    }

    // If the current vertex is inside, add it to the result
    if (curr_inside) {
      result.push_back(curr_vertex);
    }

    // Update previous vertex for next iteration
    prev_vertex = curr_vertex;
    prev_inside = curr_inside;
  }

  return result;
}

std::vector<Triangle> Renderer::GetRotatedTriangles(
    const std::vector<Triangle>& triangles, const Camera& camera) const {
  std::vector<Triangle> rotated_triangles;
  rotated_triangles.reserve(triangles.size());

  const auto& mat = camera.GetRotationMatrix().inverse();

  for (const auto& triangle : triangles) {
    rotated_triangles.push_back(triangle.GetRotatedTriangle(mat));
  }
  return rotated_triangles;
}

std::vector<Triangle> Renderer::GetProjectedTriangles(
    const std::vector<Triangle>& triangles, const Camera& camera) const {
  std::vector<Triangle> projected_triangles;
  projected_triangles.reserve(triangles.size());
  const auto& mat = camera.GetProjectionMatrix();
  for (const auto& triangle : triangles) {
    projected_triangles.push_back(triangle.GetProjectedTriangle(mat));
  }
  return projected_triangles;
}

}  // namespace renderer
