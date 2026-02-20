#include "obj_reader.h"

#include <QDebug>
#include <array>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <vector>

#include "mesh.h"

namespace renderer::kernel {

std::vector<Mesh> ObjReader::ReadFromFile(
    const std::filesystem::path& filepath) {
  if (!std::filesystem::exists(filepath)) {
    qWarning() << "File does not exist:"
               << QString::fromStdString(filepath.string());
    return {};
  }
  Assimp::Importer importer;

  const aiScene* scene = importer.ReadFile(
      filepath.string(), aiProcess_Triangulate | aiProcess_GenSmoothNormals);

  if (!scene || !scene->HasMeshes()) {
    qWarning() << "No mesh found in file";
    return {};
  }
  std::vector<Mesh> model;
  for (uint32_t mesh_index = 0; mesh_index < scene->mNumMeshes; ++mesh_index) {
    aiMesh* mesh = scene->mMeshes[mesh_index];
    Mesh current;
    for (uint32_t i = 0; i < mesh->mNumFaces; ++i) {
      std::array<Triangle::Vertex, 3> vertices;
      for (int v = 0; v < 3; ++v) {
        uint32_t index = mesh->mFaces[i].mIndices[v];
        vertices[v] = {
            .point = {mesh->mVertices[index].x, mesh->mVertices[index].y,
                      mesh->mVertices[index].z},
            .normal = Vector3{mesh->mNormals[index].x, mesh->mNormals[index].y,
                              mesh->mNormals[index].z}
                          .normalized()};
      }
      current.triangles.emplace_back(vertices);
    }
    model.push_back(std::move(current));
  }
  return model;
}

}  // namespace renderer::kernel
