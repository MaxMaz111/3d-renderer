#pragma once

#include "controller/controller.h"
#include "kernel/kernel.h"
#include "view/main_window.h"

namespace renderer {

class Application {
 public:
  Application();

 private:
  MainWindow render_window_;
  Kernel kernel_;
  Controller controller_;
};

}  // namespace renderer
