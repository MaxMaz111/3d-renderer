#include "main_window.h"

namespace renderer::view {

MainWindow::MainWindow(Width width, Height height) {
  window_.setGeometry(0, 0, width, height);
  window_.setWindowTitle("renderer");
  window_.setCentralWidget(View()->Label());
}

void MainWindow::Show() {
  window_.show();
}

View* MainWindow::View() {
  return &view_;
}

}  // namespace renderer::view
