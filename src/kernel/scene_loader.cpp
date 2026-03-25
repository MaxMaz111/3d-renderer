#include "scene_loader.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonValue>
#include <algorithm>
#include <filesystem>
#include <limits>
#include <spdlog/spdlog.h>
#include <vector>

#include "directional_light.h"
#include "linalg.h"
#include "obj_reader.h"

namespace renderer::kernel {

namespace {

Vector3 ParseVector3(const QJsonValue& value, const Vector3& fallback) {
  if (!value.isArray()) {
    return fallback;
  }

  const QJsonArray array = value.toArray();
  if (array.size() != 3 || !array[0].isDouble() || !array[1].isDouble() ||
      !array[2].isDouble()) {
    return fallback;
  }

  return Vector3{static_cast<Scalar>(array[0].toDouble()),
                 static_cast<Scalar>(array[1].toDouble()),
                 static_cast<Scalar>(array[2].toDouble())};
}

void TranslateMeshes(std::vector<Mesh>& meshes, const Vector3& position) {
  for (Mesh& mesh : meshes) {
    for (Triangle& triangle : mesh.triangles) {
      for (int i = 0; i < 3; ++i) {
        triangle.GetPoint(i) += position;
      }
    }
  }
}

void NormalizeMeshes(std::vector<Mesh>& meshes) {
  Point3 min_point{std::numeric_limits<Scalar>::max(),
                   std::numeric_limits<Scalar>::max(),
                   std::numeric_limits<Scalar>::max()};
  Point3 max_point{std::numeric_limits<Scalar>::lowest(),
                   std::numeric_limits<Scalar>::lowest(),
                   std::numeric_limits<Scalar>::lowest()};

  bool has_points = false;
  for (const Mesh& mesh : meshes) {
    for (const Triangle& triangle : mesh.triangles) {
      for (int i = 0; i < 3; ++i) {
        const Point3& point = triangle.GetPoint(i);
        min_point = min_point.cwiseMin(point);
        max_point = max_point.cwiseMax(point);
        has_points = true;
      }
    }
  }

  if (!has_points) {
    return;
  }

  const Point3 center = (min_point + max_point) * static_cast<Scalar>(0.5);
  const Vector3 size = max_point - min_point;
  const Scalar max_extent = std::max({size.x(), size.y(), size.z()});
  if (max_extent <= kEpsilon) {
    return;
  }

  for (Mesh& mesh : meshes) {
    for (Triangle& triangle : mesh.triangles) {
      for (int i = 0; i < 3; ++i) {
        Point3& point = triangle.GetPoint(i);
        point = (point - center) / max_extent;
      }
    }
  }
}

std::vector<DirectionalLight> ParseLights(const QJsonObject& root) {
  std::vector<DirectionalLight> lights;

  if (!root.contains("lights") || !root["lights"].isArray()) {
    return lights;
  }

  const QJsonArray json_lights = root["lights"].toArray();
  lights.reserve(static_cast<size_t>(json_lights.size()));

  for (const QJsonValue& value : json_lights) {
    if (!value.isObject()) {
      continue;
    }
    const QJsonObject light = value.toObject();
    const Vector3 direction =
        ParseVector3(light["direction"], Vector3{0, 0, -1});
    lights.emplace_back(direction);
  }

  return lights;
}

}  // namespace

Scene SceneLoader::ReadFromJson(const std::filesystem::path& filepath) {
  if (!std::filesystem::exists(filepath)) {
    spdlog::warn("Scene file {} does not exist", filepath.string());
    return Scene(std::vector<Mesh>{});
  }

  QFile file(filepath.string().c_str());
  if (!file.open(QIODeviceBase::ReadOnly)) {
    spdlog::warn("Failed to open scene file {}", filepath.string());
    return Scene(std::vector<Mesh>{});
  }

  QJsonParseError error;
  QJsonDocument document = QJsonDocument::fromJson(file.readAll(), &error);
  if (error.error != QJsonParseError::NoError || !document.isObject()) {
    spdlog::warn("Failed to parse scene file {}: {}", filepath.string(),
                 error.errorString().toStdString());
    return Scene(std::vector<Mesh>{});
  }

  const QJsonObject root = document.object();
  const std::vector<DirectionalLight> lights = ParseLights(root);

  if (!root.contains("models") || !root["models"].isArray()) {
    spdlog::warn("Scene file {} has no 'models' array", filepath.string());
    return Scene(std::vector<Mesh>{}, std::vector<DirectionalLight>(lights));
  }

  const QJsonArray models = root["models"].toArray();
  std::vector<Mesh> result;
  result.reserve(static_cast<size_t>(models.size()));

  const std::filesystem::path scene_directory = filepath.parent_path();
  for (const QJsonValue& value : models) {
    if (!value.isObject()) {
      continue;
    }
    const QJsonObject model = value.toObject();

    if (!model.contains("path") || !model["path"].isString()) {
      continue;
    }

    std::filesystem::path model_path = model["path"].toString().toStdString();
    std::filesystem::path resolved_path = model_path;
    if (resolved_path.is_relative()) {
      resolved_path = scene_directory / resolved_path;
    }

    std::vector<Mesh> meshes = ObjReader::ReadFromFile(resolved_path);
    if (model["is_normalized"].toBool(false)) {
      NormalizeMeshes(meshes);
    }

    const Vector3 position = ParseVector3(model["position"], Point3::Zero());
    TranslateMeshes(meshes, position);

    std::ranges::move(meshes, std::back_inserter(result));
  }

  spdlog::info(
      "Loaded scene {} with {} meshes from {} model entries and {} lights",
      filepath.string(), result.size(), models.size(), lights.size());

  return Scene(std::move(result), std::vector<DirectionalLight>(lights));
}

}  // namespace renderer::kernel
