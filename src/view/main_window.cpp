#include "main_window.h"
#include "../constants.h"

namespace renderer {

MainWindow::MainWindow() : view_(&window_) {
  window_.setGeometry(0, 0, constants::kWindowWidth, constants::kWindowHeight);
  window_.setWindowTitle("renderer");
  window_.setCentralWidget(MainWindow::GetView());
  window_.show();
}

View* MainWindow::GetView() {
  return &view_;
}

}  // namespace renderer
