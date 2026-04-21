#include "scene_config.h"

#include <fstream>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

namespace renderer::kernel {

namespace {

Vector3 ParseVec3(const nlohmann::json& value) {
  return Vector3{value.at(0).get<Scalar>(), value.at(1).get<Scalar>(),
                 value.at(2).get<Scalar>()};
}

}  // namespace

SceneConfig SceneConfig::FromJsonFile(const std::filesystem::path& filepath) {
  std::ifstream file(filepath);
  if (!file.is_open()) {
    spdlog::error("Failed to open scene file {}", filepath.string());
    return {};
  }

  nlohmann::json json_data = nlohmann::json::parse(file);

  SceneConfig config;

  for (const auto& model : json_data.at("models")) {
    ModelConfig model_config;
    model_config.path = model.at("path").get<std::string>();
    model_config.is_normalized = model.value("is_normalized", false);
    if (model.contains("position")) {
      model_config.position = ParseVec3(model.at("position"));
    }
    config.models.push_back(model_config);
  }

  for (const auto& light : json_data.at("lights")) {
    Vector3 direction{0, 0, -1};
    if (light.contains("direction")) {
      direction = ParseVec3(light.at("direction"));
    }
    config.lights.emplace_back(direction);
  }

  return config;
}

}  // namespace renderer::kernel
