#pragma once

#include <filesystem>

#include "util/observer.hpp"

#include "frame.h"
#include "renderer.h"
#include "scene.h"
#include "size.h"

namespace renderer::kernel {

class Kernel {
 public:
  Kernel(const std::filesystem::path& filename);

  void Subscribe(util::Observer<Frame>* observer);
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
  void SwapRenderingMode();

 private:
  Renderer renderer_;
  Scene scene_;

  util::ObservableData<Frame> observable_;
};

}  // namespace renderer::kernel
