#include "renderer.h"

#include <optional>
#include <vector>

#include "linalg.h"
#include "triangle.h"

namespace renderer {

QPixmap Renderer::Render(const Scene& scene) {
  std::vector<Triangle> triangles = scene.GetTriangles();
  const Camera& camera = scene.GetCamera();
  RotateTriangles(triangles, camera);
  triangles = GetClippedTriangles(triangles, camera);
  ProjectTriangles(triangles, camera);
  int width = static_cast<int>(camera.GetWidth());
  int height = static_cast<int>(camera.GetHeight());
  z_buffer_.assign(width * height, 1);
  to_return_ = QImage(width, height, QImage::Format_RGB32);
  to_return_.fill(Qt::black);
  for (const Triangle& triangle : triangles) {
    int min_x = std::floor(triangle.GetMinX());
    int max_x = std::ceil(triangle.GetMaxX());
    int min_y = std::floor(triangle.GetMinY());
    int max_y = std::ceil(triangle.GetMaxY());
    for (int i = std::max(0, min_x); i <= std::min(max_x, width - 1); ++i) {
      for (int j = std::max(0, min_y); j <= std::min(max_y, height - 1); ++j) {
        auto z = triangle.GetZ({i, j, 0});
        if (z == std::nullopt || z.value() > z_buffer_[j * width + i]) {
          continue;
        }
        z_buffer_[j * width + i] = z.value();
        to_return_.setPixelColor(
            i, j,
            {triangle.GetColor().GetRed(), triangle.GetColor().GetGreen(),
             triangle.GetColor().GetBlue()});
        z_buffer_.at(j * width + i) = z.value();
      }
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
  std::array<bool, 3> is_inside;
  int is_inside_cnt = 0;
  for (int i = 0; i < 3; ++i) {
    is_inside[i] = plane.IsOnTheSameSideAsNormal(triangle.GetPoints()[i]);
    is_inside_cnt += is_inside[i];
  }
  if (is_inside_cnt == 0) {
    return {};
  } else if (is_inside_cnt == 3) {
    return {triangle};
  }
  std::vector<Point3> inside, outside;
  for (int i = 0; i < 3; ++i) {
    if (is_inside[i]) {
      inside.push_back(triangle.GetPoints()[i]);
    } else {
      outside.push_back(triangle.GetPoints()[i]);
    }
  }
  if (is_inside_cnt == 1) {
    auto intersect1 = plane.LineIntersection(inside[0], outside[0] - inside[0]);
    auto intersect2 = plane.LineIntersection(inside[0], outside[1] - inside[0]);
    assert(intersect1.has_value() && intersect2.has_value());
    if (intersect1 == std::nullopt) {
      intersect1 = outside[0];
    }
    if (intersect2 == std::nullopt) {
      intersect2 = outside[1];
    }
    return {{intersect1.value(), intersect2.value(), inside[0],
             triangle.GetColor()}};
  } else if (is_inside_cnt == 2) {
    auto intersect1 =
        plane.LineIntersection(outside[0], inside[0] - outside[0]);
    auto intersect2 =
        plane.LineIntersection(outside[0], inside[1] - outside[0]);
    assert(intersect1.has_value() && intersect2.has_value());
    if (intersect1 == std::nullopt) {
      intersect1 = inside[0];
    }
    if (intersect2 == std::nullopt) {
      intersect2 = inside[1];
    }
    return {{intersect1.value(), intersect2.value(), inside[0],
             triangle.GetColor()},
            {inside[0], inside[1], intersect2.value(), triangle.GetColor()}};
  }

  return {triangle};
}

void Renderer::RotateTriangles(std::vector<Triangle>& triangles,
                               const Camera& camera) {
  Matrix3 mat = camera.GetRotationMatrix().inverse();
  Point3 translation = -camera.GetPosition();
  for (auto& triangle : triangles) {
    triangle.RotateAndMove(mat, translation);
  }
}

void Renderer::ProjectTriangles(std::vector<Triangle>& triangles,
                                const Camera& camera) {
  Matrix4 mat = camera.GetProjectionMatrix();
  for (auto& triangle : triangles) {
    triangle.Project(mat);
  }
}

}  // namespace renderer
