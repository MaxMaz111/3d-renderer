#pragma once

#include <filesystem>
#include <vector>

#include "directional_light.h"
#include "linalg.h"

namespace renderer::kernel {

struct ModelConfig {
  std::filesystem::path path;
  Vector3 position = Point3::Zero();
  bool is_normalized = false;
};

struct SceneConfig {
  std::vector<ModelConfig> models;
  std::vector<DirectionalLight> lights;

  static SceneConfig FromJsonFile(const std::filesystem::path& filepath);
};

}  // namespace renderer::kernel
