#include "main_window.h"

#include "util/constants.h"

namespace renderer::view {

MainWindow::MainWindow() {
  window_.setGeometry(0, 0, kernel::kDefaultWidth, kernel::kDefaultHeight);
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
