#include "scene_loader.h"

#include <spdlog/spdlog.h>

#include "kernel/mesh.h"

#include "obj_reader.h"
#include "scene_config.h"

namespace renderer::kernel {

Scene SceneLoader::ReadFromJson(const std::filesystem::path& filepath) {
  SceneConfig config = SceneConfig::FromJsonFile(filepath);

  std::vector<Mesh> result;

  for (const ModelConfig& model : config.models) {
    auto meshes = ObjReader::ReadFromFile(model.path);
    if (model.is_normalized) {
      meshes = NormalizeMeshes(std::move(meshes));
    }
    meshes = TranslateMeshes(std::move(meshes), model.position);
    std::ranges::move(meshes, std::back_inserter(result));
  }

  spdlog::info(
      "Loaded scene {} with {} meshes from {} model entries and {} lights",
      filepath.string(), result.size(), config.models.size(),
      config.lights.size());
  spdlog::info("Total triangles in the scene: {}",
               std::accumulate(result.begin(), result.end(), 0,
                               [](int sum, const Mesh& mesh) {
                                 return sum + mesh.triangles.size();
                               }));

  return Scene(std::move(result), std::move(config.lights), {});
}

std::vector<Mesh> SceneLoader::NormalizeMeshes(std::vector<Mesh>&& meshes) {
  if (meshes.empty()) {
    spdlog::warn("Cannot normalize empty mesh list");
    return {};
  }
  Vector3 bbox_min = Vector3::Constant(std::numeric_limits<Scalar>::infinity());
  Vector3 bbox_max =
      Vector3::Constant(-std::numeric_limits<Scalar>::infinity());
  for (const auto& mesh : meshes) {
    for (const auto& tri : mesh.triangles) {
      const auto& vertices = tri.Vertices();
      for (const auto& v : vertices) {
        const auto& p = v.point;
        bbox_min = bbox_min.cwiseMin(p);
        bbox_max = bbox_max.cwiseMax(p);
      }
    }
  }

  Vector3 center = (bbox_min + bbox_max) / 2;
  Vector3 extents = bbox_max - bbox_min;
  Scalar max_extent = extents.maxCoeff();
  if (max_extent < kEpsilon) {
    spdlog::warn("Cannot normalize mesh with zero size");
    return {};
  }

  Scalar scale = 1 / max_extent;
  for (auto& mesh : meshes) {
    Mesh new_mesh;
    new_mesh.diffuse_texture = mesh.diffuse_texture;
    for (auto& tri : mesh.triangles) {
      auto vertices = tri.Vertices();
      for (auto& v : vertices) {
        v.point = (v.point - center) * scale;
        v.world_point = (v.world_point - center) * scale;
      }
      new_mesh.triangles.emplace_back(std::move(vertices));
    }
    mesh = std::move(new_mesh);
  }
  return std::move(meshes);
}

std::vector<Mesh> SceneLoader::TranslateMeshes(std::vector<Mesh>&& meshes,
                                               const Vector3& translation) {
  for (auto& mesh : meshes) {
    Mesh new_mesh;
    new_mesh.diffuse_texture = mesh.diffuse_texture;
    for (auto& triangle : mesh.triangles) {
      auto vertices = triangle.Vertices();
      for (auto& v : vertices) {
        v.point += translation;
        v.world_point += translation;
      }
      new_mesh.triangles.emplace_back(std::move(vertices));
    }
    mesh = std::move(new_mesh);
  }
  return std::move(meshes);
}

}  // namespace renderer::kernel
