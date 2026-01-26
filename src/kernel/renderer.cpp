#include "renderer.h"

#include <optional>
#include <vector>

#include "util/time_anchor.h"

#include "linalg.h"
#include "triangle.h"

namespace renderer::kernel {

Frame Renderer::Render(const Scene& scene) {
  util::TimeAnchor render_timer("Render timer");
  std::vector<Triangle> triangles = scene.GetTriangles();
  const Camera& camera = scene.Camera();
  triangles = Rotate(std::move(triangles), camera);
  triangles = Clip(std::move(triangles), camera);
  triangles = Project(std::move(triangles), camera);
  return Rasterize(std::move(triangles), camera);
}

std::vector<Triangle> Renderer::Clip(std::vector<Triangle>&& triangles,
                                     const Camera& camera) const {
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

std::vector<Triangle> Renderer::Rotate(std::vector<Triangle>&& triangles,
                                       const Camera& camera) const {
  Matrix3 mat = camera.GetRotationMatrix().inverse();
  Point3 translation = -camera.GetPosition();
  for (auto& triangle : triangles) {
    triangle.RotateAndMove(mat, translation);
  }
  return triangles;
}

std::vector<Triangle> Renderer::Project(std::vector<Triangle>&& triangles,
                                        const Camera& camera) const {
  Matrix4 mat = camera.GetProjectionMatrix();
  for (auto& triangle : triangles) {
    triangle.Project(mat);
  }
  return triangles;
}

Frame Renderer::Rasterize(std::vector<Triangle>&& triangles,
                          const Camera& camera) {
  const int width = camera.Width();
  const int height = camera.Height();
  z_buffer_.ResetTo(Width{width}, Height{height});
  Frame frame(Width{width}, Height{height});
  for (const Triangle& triangle : triangles) {
    int min_x = std::floor(triangle.GetMinX());
    int max_x = std::ceil(triangle.GetMaxX());
    int min_y = std::floor(triangle.GetMinY());
    int max_y = std::ceil(triangle.GetMaxY());
    for (int j = std::max(0, min_y); j <= std::min(max_y, height - 1); ++j) {
      for (int i = std::max(0, min_x); i <= std::min(max_x, width - 1); ++i) {
        auto z = triangle.GetZ(Point3(i, j, 0));
        if (!z.has_value()) {
          continue;
        }
        switch (camera.CurrentRenderingMode()) {
          case Camera::RenderingMode::AllSolid: {
            Scalar& val = z_buffer_.Get(Width{i}, Height{j});
            if (val > *z) {
              val = *z;
              frame.SetColor(Width{i}, Height{j}, triangle.GetColor());
            }
            break;
          }
          case Camera::RenderingMode::AllTransparent: {
            frame.BlendColor(Width{i}, Height{j}, triangle.GetColor());
            break;
          }
          default:
            break;
        }
      }
    }
  }

  return frame;
}

QColor Renderer::ConvertColor(const Color& color) {
  return {color.GetRed(), color.GetGreen(), color.GetBlue()};
}

}  // namespace renderer::kernel
