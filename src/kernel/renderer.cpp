#include "renderer.h"

#include <optional>
#include <vector>

#include "linalg.h"
#include "triangle.h"

namespace renderer {

QPixmap Renderer::Render(const Scene& scene) {
  const std::vector<Triangle>& triangles = scene.GetTriangles();
  const Camera& camera = scene.GetCamera();
  auto rotated_triangles = GetRotatedTriangles(triangles, camera);
  auto clipped_triangles = GetClippedTriangles(rotated_triangles, camera);
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
          i % width, i / width,
          {triangle.GetColor().GetRed(), triangle.GetColor().GetGreen(),
           triangle.GetColor().GetBlue()});
      z_buffer_.at(i) = z.value();
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
    return {{intersect1.value(), intersect2.value(), inside[0]}};
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
    return {{intersect1.value(), intersect2.value(), inside[0]},
            {inside[0], inside[1], intersect2.value()}};
  }

  return {triangle};
}

std::vector<Triangle> Renderer::GetRotatedTriangles(
    const std::vector<Triangle>& triangles, const Camera& camera) const {
  std::vector<Triangle> rotated_triangles;
  rotated_triangles.reserve(triangles.size());

  Matrix3 mat = camera.GetRotationMatrix().inverse();
  Point3 translation = -camera.GetPosition();

  for (const auto& triangle : triangles) {
    rotated_triangles.push_back(
        triangle.GetRotatedAndMovedTriangle(mat, translation));
  }
  return rotated_triangles;
}

std::vector<Triangle> Renderer::GetProjectedTriangles(
    const std::vector<Triangle>& triangles, const Camera& camera) const {
  std::vector<Triangle> projected_triangles;
  projected_triangles.reserve(triangles.size());
  Matrix4 mat = camera.GetProjectionMatrix();
  for (const auto& triangle : triangles) {
    projected_triangles.push_back(triangle.GetProjectedTriangle(mat));
  }
  return projected_triangles;
}

}  // namespace renderer
