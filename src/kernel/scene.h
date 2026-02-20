#pragma once

#include <vector>

#include "camera.h"
#include "directional_light.h"
#include "mesh.h"

namespace renderer::kernel {

class Scene {
  using CameraT = Camera;

 public:
  Scene(std::vector<Mesh>&& meshes);
  Scene(CameraT&& camera, std::vector<Mesh>&& meshes);

  const std::vector<Mesh>& Meshes() const;
  const std::vector<DirectionalLight>& DirectionalLights() const;
  void SetAspectRatio(Scalar aspect_ratio);
  void RotateLeft();
  void RotateRight();
  void RotateUp();
  void RotateDown();
  void MoveLeft();
  void MoveRight();
  void MoveForward();
  void MoveBackward();
  void SwivelLeft();
  void SwivelRight();
  void SwapRenderingMode();
  const CameraT& Camera() const;
  CameraT& Camera();
  Camera::RenderingMode CurrentRenderingMode() const;

 private:
  CameraT camera_;
  std::vector<Mesh> meshes_;
  std::vector<DirectionalLight> directional_lights_;
};

}  // namespace renderer::kernel
