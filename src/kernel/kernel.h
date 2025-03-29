#pragma once

#include "renderer.h"
#include "scene.h"

#include "../observer.hpp"

namespace renderer {

class Kernel {
  using SentData = QPixmap;
  using Observable = Observable<SentData>;
  using Observer = Observer<SentData>;

 public:
  Kernel();

  void Subscribe(Observer* observer);

 private:
  Renderer renderer_;
  Scene scene_;

  Observable observable_;
};

}  // namespace renderer
