#pragma once

#include "kernel/frame.h"

#include "observer.hpp"
#include "renderer.h"
#include "scene.h"
#include "size.h"

namespace renderer {

class Kernel {
 public:
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

  void SwapTransparency();

 private:
  Renderer renderer_;
  Scene scene_;

  ObservableData<Frame> observable_;
};

}  // namespace renderer
