#pragma once

#include <string>
#include <vector>

#include "triangle.h"

namespace renderer {

class ObjReader {
 public:
  static std::vector<Triangle> ReadFromFile(const std::string& filepath);

 private:
  static Point3 ParseVertex(const std::string& line);
  static std::vector<Triangle> ParseFace(const std::string& line,
                                         const std::vector<Point3>& vertices);
};

}  // namespace renderer
