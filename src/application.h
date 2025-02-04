#pragma once

#include <QApplication>

#include "render_window.h"

namespace renderer {
class Application : public QApplication {
 public:
  Application(int argc, char* argv[]);
  int Run();

 private:
  RenderWindow render_window_;
};
}  // namespace renderer
