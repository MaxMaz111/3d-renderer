#pragma once

#include <filesystem>

#include "mesh.h"

namespace renderer::kernel {

class ObjReader {
 public:
  static std::vector<Mesh> ReadFromFile(const std::filesystem::path& filepath);
};

}  // namespace renderer::kernel
