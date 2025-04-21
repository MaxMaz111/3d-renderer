#pragma once

#include "controller/controller.h"
#include "kernel/kernel.h"
#include "view/main_window.h"

namespace renderer {

class Application {
 public:
  Application(int argc, char* argv[]);

 private:
  MainWindow render_window_1_, render_window_2_;
  Kernel kernel_;
  Controller controller_1_, controller_2_;
};

}  // namespace renderer
