#pragma once

#include "kernel/kernel.h"

#include "view/main_window.h"

#include "controller/controller.h"

namespace renderer {

class Application {
  using Kernel = kernel::Kernel;
  using MainWindow = view::MainWindow;
  using Controller = controller::Controller;

 public:
  Application(int argc, char* argv[]);

 private:
  Kernel kernel_;
  MainWindow render_window_;
  Controller controller_;
};

}  // namespace renderer
