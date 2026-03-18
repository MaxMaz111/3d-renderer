#pragma once

#include <filesystem>

#include "util/observer.hpp"
#include "util/size.h"

#include "frame.h"
#include "renderer.h"
#include "scene.h"

namespace renderer::kernel {

class Kernel {
  using Observable = util::Observable<Frame>;
  using Observer = util::Observer<Frame>;

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
  void SwapRenderingMode();
  void NotifyView();

 private:
  Renderer renderer_;
  Scene scene_;

  Observable observable_;
};

}  // namespace renderer::kernel
