#include "obj_reader.h"

#include <fstream>
#include <iostream>
#include <sstream>

namespace renderer {

std::vector<Triangle> ObjReader::ReadFromFile(const std::string& filepath) {
  std::vector<Triangle> triangles;
  std::vector<Point3> vertices;
  std::ifstream file(filepath);
  if (!file.is_open()) {
    std::cerr << "Failed to open file: " << filepath << std::endl;
    return triangles;
  }
  std::string line;
  while (std::getline(file, line)) {
    if (line.empty()) {
      continue;
    }
    std::istringstream iss(line);
    std::string token;
    iss >> token;

    if (token == "v") {
      vertices.push_back(ParseVertex(line));
    } else if (token == "f") {
      std::vector<Triangle> face_tris = ParseFace(line, vertices);
      triangles.insert(triangles.end(), face_tris.begin(), face_tris.end());
    }
  }
  return triangles;
}

Point3 ObjReader::ParseVertex(const std::string& line) {
  std::istringstream iss(line);
  std::string token;
  Scalar x, y, z;
  iss >> token;
  if (!(iss >> x >> y >> z)) {
    std::cerr << "Error parsing vertex coordinates from: " << line << std::endl;
    return Point3{0, 0, 0};
  }
  return Point3{x, y, z};
}

std::vector<Triangle> ObjReader::ParseFace(
    const std::string& line, const std::vector<Point3>& vertices) {
  std::vector<Triangle> result;
  std::istringstream iss(line);
  std::string token;
  iss >> token;
  std::vector<int> face_indices;
  while (iss >> token) {
    std::istringstream index_stream(token);
    std::string index_str;
    std::getline(index_stream, index_str, '/');
    int vertex_index = std::stoi(index_str) - 1;
    face_indices.push_back(vertex_index);
  }
  if (face_indices.size() == 3) {
    Triangle triangle(vertices[face_indices[0]], vertices[face_indices[1]],
                      vertices[face_indices[2]]);
    result.push_back(triangle);
  } else if (face_indices.size() > 3) {
    for (size_t i = 1; i < face_indices.size() - 1; ++i) {
      Triangle tri(vertices[face_indices[0]], vertices[face_indices[i]],
                   vertices[face_indices[i + 1]]);
      result.push_back(tri);
    }
  }
  return result;
}

}  // namespace renderer
