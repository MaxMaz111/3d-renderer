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
  std::vector<Triangle> clipped_triangle = {triangle};
  for (const Plane& plane : camera.GetPlanesForClipping()) {
    std::vector<Triangle> new_clipped_triangle;
    for (const Triangle& triangle : clipped_triangle) {
      auto new_triangles = ClipTriangleByPlane(triangle, plane);
      new_clipped_triangle.insert(new_clipped_triangle.end(),
                                  new_triangles.begin(), new_triangles.end());
    }
    clipped_triangle = std::move(new_clipped_triangle);
  }
  return clipped_triangle;
}

std::vector<Triangle> Renderer::ClipTriangleByPlane(const Triangle& triangle,
                                                    const Plane& plane) const {
  std::vector<Triangle> result;

  // Get triangle points
  const auto& points = triangle.GetPoints();

  // Classify each vertex: is it on the same side as the plane normal?
  bool sides[3];
  for (int i = 0; i < 3; i++) {
    sides[i] = plane.IsOnTheSameSideAsNormal(points[i]);
  }

  // Case 1: Triangle is completely on the normal side - keep it
  if (sides[0] && sides[1] && sides[2]) {
    result.push_back(triangle);
    return result;
  }

  // Case 2: Triangle is completely on the opposite side - clip it
  if (!sides[0] && !sides[1] && !sides[2]) {
    return result;  // Empty vector
  }

  // Case 3: Triangle intersects the plane - we need to split it
  std::vector<Point3> kept_vertices;
  std::vector<Point3> intersection_points;

  // Process each edge
  for (int i = 0; i < 3; i++) {
    int j = (i + 1) % 3;

    // Keep vertices on the normal side
    if (sides[i]) {
      kept_vertices.push_back(points[i]);
    }

    // If edge crosses the plane, compute intersection point
    if (sides[i] != sides[j]) {
      Vector3 edge_dir = points[j] - points[i];
      auto intersection = plane.LineIntersection(points[i], edge_dir);
      if (intersection) {
        intersection_points.push_back(points[i] + *intersection);
      }
    }
  }

  // Should have exactly 2 intersection points for a valid case
  if (intersection_points.size() != 2) {
    return result;
  }

  // Create the new triangles based on how many vertices were kept
  if (kept_vertices.size() == 1) {
    // Create one triangle with the kept vertex and two intersection points
    Triangle new_triangle(kept_vertices[0], intersection_points[0],
                          intersection_points[1]);
    new_triangle.SetColor(triangle.GetColor());
    result.push_back(new_triangle);
  } else if (kept_vertices.size() == 2) {
    // Create two triangles from the quad
    Triangle t1(kept_vertices[0], kept_vertices[1], intersection_points[0]);
    Triangle t2(kept_vertices[0], intersection_points[0],
                intersection_points[1]);

    t1.SetColor(triangle.GetColor());
    t2.SetColor(triangle.GetColor());

    result.push_back(t1);
    result.push_back(t2);
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
