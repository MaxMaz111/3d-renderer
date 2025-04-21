#pragma once

#include <vector>

#include "../size.h"
#include "camera.h"
#include "triangle.h"

namespace renderer {

class Scene {
 public:
  Scene() = delete;
  Scene(const std::vector<Triangle>& triangles);
  Scene(const std::vector<Camera>& cameras,
        const std::vector<Triangle>& triangles);
  const std::vector<Triangle>& GetTriangles() const;
  void SetScreenDimensions(Width width, Height height);
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
  void SwapTransparency();
  const Camera& GetCamera() const;
  Camera& GetCamera();
  bool Transapent() const;

 private:
  std::vector<Camera> cameras_;
  int cur_camera_index_;
  std::vector<Triangle> triangles_;
  bool transparent_ = false;
};

}  // namespace renderer
