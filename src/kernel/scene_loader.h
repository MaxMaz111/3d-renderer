#pragma once

#include <filesystem>

#include "scene.h"

namespace renderer::kernel {

class SceneLoader {
 public:
  static Scene ReadFromJson(const std::filesystem::path& filepath);

 private:
  static std::vector<Mesh> NormalizeMeshes(std::vector<Mesh>&& meshes);
  static std::vector<Mesh> TranslateMeshes(std::vector<Mesh>&& meshes,
                                           const Vector3& translation);
};

}  // namespace renderer::kernel
