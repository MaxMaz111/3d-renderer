#include "obj_reader.h"

#include <QImage>
#include <array>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <spdlog/spdlog.h>
#include <unordered_map>
#include <vector>

#include "mesh.h"

namespace renderer::kernel {

std::unordered_map<std::string, Texture> ObjReader::texture_cache;
std::filesystem::path ObjReader::model_directory;

std::vector<Mesh> ObjReader::ReadFromFile(
    const std::filesystem::path& filepath) {
  if (!std::filesystem::exists(filepath)) {
    spdlog::warn("File {} does not exist", filepath.string());
    return {};
  }
  Assimp::Importer importer;

  model_directory = filepath.parent_path();
  spdlog::info("Setting model directory as {}", model_directory.string());

  const aiScene* scene = importer.ReadFile(
      filepath.string(),
      aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);

  if (!scene || !scene->HasMeshes()) {
    spdlog::warn("Failed to load model from file {}: {}", filepath.string(),
                 importer.GetErrorString());
    return {};
  }
  std::vector<Mesh> model(scene->mNumMeshes);
  for (size_t mesh_index = 0; mesh_index < scene->mNumMeshes; ++mesh_index) {
    model[mesh_index] = ProcessMesh(scene, scene->mMeshes[mesh_index]);
  }
  return model;
}

Mesh ObjReader::ProcessMesh(const aiScene* scene, const aiMesh* mesh) {
  Mesh current;
  ProcessMaterials(current, scene, mesh);

  current.triangles.reserve(mesh->mNumFaces);
  for (size_t i = 0; i < mesh->mNumFaces; ++i) {
    std::array<Triangle::Vertex, 3> vertices;
    const aiFace& face = mesh->mFaces[i];
    for (size_t v = 0; v < 3; ++v) {
      vertices[v] = ExtractVertex(mesh, face.mIndices[v]);
    }
    current.triangles.emplace_back(vertices);
  }
  return current;
}

Triangle::Vertex ObjReader::ExtractVertex(const aiMesh* mesh, uint32_t index) {
  const aiVector3D& vertex = mesh->mVertices[index];
  const aiVector3D& normal = mesh->mNormals[index];

  Point3 point{vertex.x, vertex.y, vertex.z};
  Vector3 normal_vec{normal.x, normal.y, normal.z};
  normal_vec = normal_vec.normalized();

  Point2 tex_coord;
  if (mesh->HasTextureCoords(0)) {
    const aiVector3D& tex = mesh->mTextureCoords[0][index];
    tex_coord = Point2{tex.x, tex.y};
  }

  return Triangle::Vertex{point, normal_vec, tex_coord};
}

void ObjReader::ProcessMaterials(Mesh& current, const aiScene* scene,
                                 const aiMesh* mesh) {
  if (!scene->HasMaterials() || mesh->mMaterialIndex >= scene->mNumMaterials) {
    return;
  }
  const aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
  current.diffuse_texture =
      LoadTextureFromMaterial(material, aiTextureType_DIFFUSE);
}

Texture ObjReader::LoadTextureFromMaterial(const aiMaterial* material,
                                           aiTextureType texture_type) {
  if (!material) {
    return Texture();
  }
  aiString ai_texture_path;
  if (material->GetTexture(texture_type, 0, &ai_texture_path) != AI_SUCCESS) {
    return Texture();
  }
  std::filesystem::path texture_path(ai_texture_path.C_Str());
  if (texture_path.empty()) {
    return Texture();
  }
  texture_path.make_preferred();
  if (texture_path.is_relative()) {
    texture_path = model_directory / texture_path;
  }
  const std::string cache_key = texture_path.lexically_normal().string();

  if (auto it = texture_cache.find(cache_key); it != texture_cache.end()) {
    spdlog::info("Texture {} loaded from cache", cache_key);
    return it->second;
  }

  QImage image(cache_key.c_str());
  if (image.isNull()) {
    spdlog::warn("Failed to load texture {}", cache_key);
    return Texture();
  }

  spdlog::info("Loaded texture from {}", cache_key);
  Texture tex(std::move(image));
  texture_cache[cache_key] = tex;
  return tex;
}

}  // namespace renderer::kernel
