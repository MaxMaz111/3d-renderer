#include "obj_reader.h"

#include <QDebug>
#include <array>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "linalg.h"

namespace renderer::kernel {

std::vector<Triangle> ObjReader::ReadFromFile(
    const std::filesystem::path& filepath) {
  std::vector<Triangle> triangles;
  Assimp::Importer importer;

  const aiScene* scene = importer.ReadFile(
      filepath.string(), aiProcess_Triangulate | aiProcess_GenSmoothNormals);

  if (!scene || !scene->HasMeshes()) {
    qWarning() << "No mesh found in the file";
    return triangles;
  }

  for (uint32_t mesh_index = 0; mesh_index < scene->mNumMeshes; ++mesh_index) {
    aiMesh* mesh = scene->mMeshes[mesh_index];
    for (uint32_t i = 0; i < mesh->mNumFaces; ++i) {
      std::array<Point3, 3> points;
      std::array<Vector3, 3> normals;
      for (int vertex = 0; vertex < 3; ++vertex) {
        uint32_t vertex_index = mesh->mFaces[i].mIndices[vertex];
        points[vertex] = Point3(mesh->mVertices[vertex_index].x,
                                mesh->mVertices[vertex_index].y,
                                mesh->mVertices[vertex_index].z);
        normals[vertex] = Vector3(mesh->mNormals[vertex_index].x,
                                  mesh->mNormals[vertex_index].y,
                                  mesh->mNormals[vertex_index].z)
                              .normalized();
      }
      triangles.emplace_back(points, normals);
    }
  }

  return triangles;
}

}  // namespace renderer::kernel
