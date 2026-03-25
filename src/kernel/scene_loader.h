#pragma once

#include <filesystem>

#include "scene.h"

namespace renderer::kernel {

class SceneLoader {
 public:
  static Scene ReadFromJson(const std::filesystem::path& filepath);
};

}  // namespace renderer::kernel
