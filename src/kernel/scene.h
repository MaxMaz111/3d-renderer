#pragma once

#include <vector>

#include "camera.h"
#include "mesh.h"

namespace renderer::kernel {

class Scene {
  using CameraT = Camera;

 public:
  Scene(std::vector<Mesh>&& meshes);
  Scene(std::vector<Camera>&& cameras, std::vector<Mesh>&& meshes);

  const std::vector<Mesh>& Meshes() const;
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
  void SetCurrentCamera(int camera_index);
  void SwapRenderingMode();
  const CameraT& Camera() const;
  CameraT& Camera();
  Camera::RenderingMode CurrentRenderingMode() const;

 private:
  std::vector<CameraT> cameras_;
  int cur_camera_index_;
  std::vector<Mesh> meshes_;
};

}  // namespace renderer::kernel
