#pragma once

#include <assimp/material.h>
#include <assimp/scene.h>
#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>

#include "mesh.h"

namespace renderer::kernel {

class ObjReader {
 public:
  static std::vector<Mesh> ReadFromFile(const std::filesystem::path& filepath);

 private:
  static Mesh ProcessMesh(const aiScene* scene, const aiMesh* mesh);
  static Vertex ExtractVertex(const aiMesh* mesh, uint32_t index);
  static void ProcessMaterials(Mesh& current, const aiScene* scene,
                               const aiMesh* mesh);
  static Texture LoadTextureFromMaterial(const aiMaterial* material,
                                         aiTextureType texture_type);

  static std::filesystem::path model_directory;
  static std::unordered_map<std::string, Texture> texture_cache;
};

}  // namespace renderer::kernel
