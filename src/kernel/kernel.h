#pragma once

#include "../observer.hpp"
#include "../size.h"
#include "kernel/frame.h"
#include "renderer.h"
#include "scene.h"

namespace renderer {

class Kernel {
 public:
  Kernel() = delete;
  Kernel(const std::string& filename);
  void Subscribe(Observer<Frame>* observer);
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

 private:
  Renderer renderer_;
  Scene scene_;

  ObservableData<Frame> observable_;
};

}  // namespace renderer
