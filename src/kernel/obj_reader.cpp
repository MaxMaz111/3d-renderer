#include "obj_reader.h"

#include <QDebug>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

namespace renderer::kernel {

std::vector<Triangle> ObjReader::ReadFromFile(
    const std::filesystem::path& filepath) {
  std::vector<Triangle> triangles;
  Assimp::Importer importer;

  const aiScene* scene = importer.ReadFile(
      filepath.string(), aiProcess_Triangulate | aiProcess_GenNormals);

  if (!scene || !scene->HasMeshes()) {
    qWarning() << "No mesh found in the file";
    return triangles;
  }

  aiMesh* mesh = scene->mMeshes[0];
  for (uint32_t i = 0; i < mesh->mNumFaces; ++i) {
    const aiFace& face = mesh->mFaces[i];
    const aiVector3D& a = mesh->mVertices[face.mIndices[0]];
    const aiVector3D& b = mesh->mVertices[face.mIndices[1]];
    const aiVector3D& c = mesh->mVertices[face.mIndices[2]];
    Point3 p0{a.x, a.y, a.z};
    Point3 p1{b.x, b.y, b.z};
    Point3 p2{c.x, c.y, c.z};
    Vector3 normal;
    if (mesh->HasNormals()) {
      const aiVector3D& n = mesh->mNormals[face.mIndices[0]];
      normal = Vector3{n.x, n.y, n.z}.normalized();
    } else {
      Vector3 e1 = p1 - p0;
      Vector3 e2 = p2 - p0;
      normal = e1.cross(e2).normalized();
    }
    triangles.emplace_back(p0, p1, p2, normal);
  }

  return triangles;
}

}  // namespace renderer::kernel
