#include "application.h"

namespace renderer {
Application::Application(int argc, char* argv[]) : QApplication(argc, argv) {}
int Application::Run() {
  render_window_.show();
  return exec();
}
}  // namespace renderer
