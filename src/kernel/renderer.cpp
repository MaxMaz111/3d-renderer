#include "renderer.h"

#include <optional>
#include <vector>

#include "../timer.h"
#include "linalg.h"
#include "triangle.h"

namespace renderer {

Frame Renderer::Render(const Scene& scene) {
  Timer render_timer("Render timer");
  std::vector<Triangle> triangles = scene.GetTriangles();
  const Camera& camera = scene.GetCamera();
  RotateTriangles(triangles, camera);
  triangles = GetClippedTriangles(triangles, camera);
  ProjectTriangles(triangles, camera);
  int width = static_cast<int>(camera.GetWidth());
  int height = static_cast<int>(camera.GetHeight());
  z_buffer_.assign(height, std::vector<Scalar>(width, 1));
  Frame frame(camera.GetWidth(), camera.GetHeight());
  for (const Triangle& triangle : triangles) {
    int min_x = std::floor(triangle.GetMinX());
    int max_x = std::ceil(triangle.GetMaxX());
    int min_y = std::floor(triangle.GetMinY());
    int max_y = std::ceil(triangle.GetMaxY());
    for (int j = std::max(0, min_y); j <= std::min(max_y, height - 1); ++j) {
      for (int i = std::max(0, min_x); i <= std::min(max_x, width - 1); ++i) {
        auto z = triangle.GetZ({i, j, 0});
        if (z == std::nullopt) {
          continue;
        }
        if (scene.Transapent()) {
          frame.BlendColor(Width{i}, Height{j}, triangle.GetColor());
        } else if (z_buffer_[j][i] > z.value()) {
          z_buffer_[j][i] = z.value();
          frame.SetColor(Width{i}, Height{j}, triangle.GetColor());
        }
      }
    }
  }

  return frame;
}

std::vector<Triangle> Renderer::GetClippedTriangles(
    const std::vector<Triangle>& triangles, const Camera& camera) const {
  const auto& planes = camera.GetPlanesForClipping();

  std::vector<Triangle> result;
  result.reserve(triangles.size() * 2);

  std::vector<Triangle> current_buffer;
  std::vector<Triangle> next_buffer;
  current_buffer.reserve(8);
  next_buffer.reserve(16);

  for (const Triangle& triangle : triangles) {
    current_buffer.clear();
    current_buffer.push_back(triangle);
    bool visible = true;
    for (const Plane& plane : planes) {
      if (current_buffer.empty()) {
        visible = false;
        break;
      }
      next_buffer.clear();
      for (const Triangle& current_triangle : current_buffer) {
        auto clipped_triangles = ClipTriangleByPlane(current_triangle, plane);
        if (!clipped_triangles.empty()) {
          if (next_buffer.size() + clipped_triangles.size() >
              next_buffer.capacity()) {
            next_buffer.reserve(next_buffer.size() + clipped_triangles.size());
          }
          next_buffer.insert(next_buffer.end(),
                             std::make_move_iterator(clipped_triangles.begin()),
                             std::make_move_iterator(clipped_triangles.end()));
        }
      }
      current_buffer.swap(next_buffer);
    }

    if (visible && !current_buffer.empty()) {
      if (result.size() + current_buffer.size() > result.capacity()) {
        result.reserve(result.size() + current_buffer.size());
      }
      result.insert(result.end(),
                    std::make_move_iterator(current_buffer.begin()),
                    std::make_move_iterator(current_buffer.end()));
    }
  }
  return result;
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
             triangle.GetNormal(), triangle.GetColor()}};
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
             triangle.GetNormal(), triangle.GetColor()},
            {inside[0], inside[1], intersect2.value(), triangle.GetNormal(),
             triangle.GetColor()}};
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

QColor Renderer::ConvertColor(const Color& color) {
  return {color.GetRed(), color.GetGreen(), color.GetBlue()};
}

}  // namespace renderer
