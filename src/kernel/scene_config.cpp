#include "scene_config.h"

#include <fstream>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

namespace renderer::kernel {

SceneConfig SceneConfig::FromJsonFile(const std::filesystem::path& filepath) {
  std::ifstream file(filepath);
  if (!file.is_open()) {
    spdlog::error("Failed to open scene file {}", filepath.string());
    return {};
  }
  nlohmann::json json_data;
  file >> json_data;
  SceneConfig config;
  for (const auto& model : json_data["models"]) {
    ModelConfig model_config;
    model_config.path = model["path"].get<std::string>();
    const auto& position = model["position"];
    model_config.position =
        Vector3{position[0].get<Scalar>(), position[1].get<Scalar>(),
                position[2].get<Scalar>()};
    model_config.is_normalized = model["is_normalized"].get<bool>();
    config.models.push_back(model_config);
  }

  for (const auto& light : json_data["lights"]) {
    const auto& direction = light["direction"];
    config.lights.push_back(Vector3{direction[0].get<Scalar>(),
                                    direction[1].get<Scalar>(),
                                    direction[2].get<Scalar>()});
  }
  return config;
}

}  // namespace renderer::kernel
