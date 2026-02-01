#pragma once

#include <filesystem>
#include <vector>

#include "triangle.h"

namespace renderer::kernel {

class ObjReader {
 public:
  static std::vector<Triangle> ReadFromFile(
      const std::filesystem::path& filepath);
};

}  // namespace renderer::kernel
