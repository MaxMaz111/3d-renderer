#include "obj_reader.h"

#include <QDebug>
#include <array>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

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
      const aiFace& face = mesh->mFaces[i];
      const aiVector3D& a = mesh->mVertices[face.mIndices[0]];
      const aiVector3D& b = mesh->mVertices[face.mIndices[1]];
      const aiVector3D& c = mesh->mVertices[face.mIndices[2]];
      Point3 p0{a.x, a.y, a.z};
      Point3 p1{b.x, b.y, b.z};
      Point3 p2{c.x, c.y, c.z};
      std::array<Vector3, 3> normals;
      if (mesh->HasNormals()) {
        const aiVector3D& na = mesh->mNormals[face.mIndices[0]];
        const aiVector3D& nb = mesh->mNormals[face.mIndices[1]];
        const aiVector3D& nc = mesh->mNormals[face.mIndices[2]];
        normals = {Vector3{na.x, na.y, na.z}.normalized(),
                   Vector3{nb.x, nb.y, nb.z}.normalized(),
                   Vector3{nc.x, nc.y, nc.z}.normalized()};
      } else {
        Vector3 e1 = p1 - p0;
        Vector3 e2 = p2 - p0;
        Vector3 face_normal = e1.cross(e2).normalized();
        normals = {face_normal, face_normal, face_normal};
      }
      std::array<Color, 3> colors{Color(255, 255, 255), Color(255, 255, 255),
                                  Color(255, 255, 255)};
      triangles.emplace_back(p0, p1, p2, normals, colors);
    }
  }

  return triangles;
}

}  // namespace renderer::kernel
