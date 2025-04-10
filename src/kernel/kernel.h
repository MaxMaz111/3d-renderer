#pragma once

#include "../observer.hpp"
#include "../size.h"
#include "renderer.h"
#include "scene.h"

namespace renderer {

class Kernel {
  using Observable = Observable<QPixmap>;
  using Observer = Observer<QPixmap>;

 public:
  Kernel() = delete;
  Kernel(const std::string& filename);
  void Subscribe(Observer* observer);
  void SetScreenDimensions(Width width, Height height);
  void RotateLeft();
  void RotateRight();
  void RotateUp();
  void RotateDown();
  void MoveLeft();
  void MoveRight();
  void MoveForward();
  void MoveBackward();

 private:
  Renderer renderer_;
  Scene scene_;

  Observable observable_;
};

}  // namespace renderer
