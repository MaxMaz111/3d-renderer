#include "obj_reader.h"

#include <fstream>
#include <iostream>
#include <sstream>

namespace renderer {

std::vector<Triangle> ObjReader::ReadFromFile(const std::string& filepath) {
  std::vector<Triangle> triangles;
  std::vector<Point3> vertices;
  std::vector<Vector3> normals;
  std::ifstream file(filepath);
  if (!file.is_open()) {
    std::cerr << "Error: Failed to open file: " << filepath << std::endl;
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
    } else if (token == "vn") {
      normals.push_back(ParseNormal(line));
    } else if (token == "f") {
      std::vector<Triangle> face_triangles = ParseFace(line, vertices, normals);
      triangles.insert(triangles.end(), face_triangles.begin(),
                       face_triangles.end());
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
    std::cerr << "Error: Could not parse vertex coordinates from: " << line
              << std::endl;
    return Point3{0, 0, 0};
  }
  return Point3{x, y, z};
}

Vector3 ObjReader::ParseNormal(const std::string& line) {
  std::istringstream iss(line);
  std::string token;
  Scalar x, y, z;
  iss >> token;
  if (!(iss >> x >> y >> z)) {
    std::cerr << "Error: Could not parse normal vector from: " << line
              << std::endl;
    return Vector3{0, 0, 0};
  }
  return Vector3{x, y, z};
}

std::vector<Triangle> ObjReader::ParseFace(
    const std::string& line, const std::vector<Point3>& vertices,
    const std::vector<Vector3>& normals) {
  std::vector<Triangle> result;
  std::istringstream iss(line);
  std::string token;
  iss >> token;

  std::vector<int> face_vertex_indices;
  std::vector<int> face_normal_indices;
  bool has_normals = false;

  while (iss >> token) {
    std::istringstream index_stream(token);
    std::string vertex_index_str;
    std::string texture_index_str;
    std::string normal_index_str;

    std::getline(index_stream, vertex_index_str, '/');
    try {
      int vertex_index = std::stoi(vertex_index_str) - 1;
      face_vertex_indices.push_back(vertex_index);
    } catch (const std::exception& e) {
      std::cerr << "Error: Failed to parse face vertex index from token: "
                << token << " in line: " << line << std::endl;
      continue;
    }

    if (index_stream.peek() == '/') {
      index_stream.ignore();
    } else {
      std::getline(index_stream, texture_index_str, '/');
    }

    if (std::getline(index_stream, normal_index_str)) {
      try {
        int normal_index = std::stoi(normal_index_str) - 1;
        face_normal_indices.push_back(normal_index);
        has_normals = true;
      } catch (const std::exception& e) {
        std::cerr << "Error: Failed to parse face normal index from token: "
                  << token << " in line: " << line << std::endl;
      }
    }
  }

  for (int idx : face_vertex_indices) {
    if (idx < 0 || static_cast<size_t>(idx) >= vertices.size()) {
      std::cerr << "Error: Face vertex index " << idx + 1
                << " out of range in line: " << line << std::endl;
      return result;
    }
  }

  if (has_normals) {
    for (int idx : face_normal_indices) {
      if (idx < 0 || static_cast<size_t>(idx) >= normals.size()) {
        std::cerr << "Error: Face normal index " << idx + 1
                  << " out of range in line: " << line << std::endl;
        has_normals = false;
        break;
      }
    }
  }

  Color default_color = Color::GetRandomColor();

  if (face_vertex_indices.size() == 3) {
    if (has_normals && face_normal_indices.size() == 3) {
      Vector3 normal = normals[face_normal_indices[0]];
      result.emplace_back(
          vertices[face_vertex_indices[0]], vertices[face_vertex_indices[1]],
          vertices[face_vertex_indices[2]], normal, default_color);
    } else {
      Vector3 edge1 =
          vertices[face_vertex_indices[1]] - vertices[face_vertex_indices[0]];
      Vector3 edge2 =
          vertices[face_vertex_indices[2]] - vertices[face_vertex_indices[0]];
      Vector3 normal = edge1.cross(edge2).normalized();

      result.emplace_back(
          vertices[face_vertex_indices[0]], vertices[face_vertex_indices[1]],
          vertices[face_vertex_indices[2]], normal, default_color);
    }
  } else if (face_vertex_indices.size() > 3) {
    for (size_t i = 1; i < face_vertex_indices.size() - 1; ++i) {
      if (has_normals &&
          face_normal_indices.size() == face_vertex_indices.size()) {
        Vector3 normal = normals[face_normal_indices[0]];
        result.emplace_back(
            vertices[face_vertex_indices[0]], vertices[face_vertex_indices[i]],
            vertices[face_vertex_indices[i + 1]], normal, default_color);
      } else {
        Vector3 edge1 =
            vertices[face_vertex_indices[i]] - vertices[face_vertex_indices[0]];
        Vector3 edge2 = vertices[face_vertex_indices[i + 1]] -
                        vertices[face_vertex_indices[0]];
        Vector3 normal = edge1.cross(edge2).normalized();

        result.emplace_back(
            vertices[face_vertex_indices[0]], vertices[face_vertex_indices[i]],
            vertices[face_vertex_indices[i + 1]], normal, default_color);
      }
    }
  }

  return result;
}

}  // namespace renderer
