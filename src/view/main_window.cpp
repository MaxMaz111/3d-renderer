#include "main_window.h"

namespace renderer {

MainWindow::MainWindow(Width width, Height height) : view_(&window_) {
  window_.setGeometry(0, 0, static_cast<int>(width), static_cast<int>(height));
  window_.setWindowTitle("renderer");
  window_.setCentralWidget(GetView());
}

void MainWindow::Show() {
  window_.show();
}

View* MainWindow::GetView() {
  return &view_;
}

}  // namespace renderer
