#pragma once

#include <QMainWindow>

#include "view.h"

namespace renderer::view {

class MainWindow {
 public:
  MainWindow();

  void Show();
  ::renderer::view::View* View();

 private:
  QMainWindow window_;
  view::View view_;
};

}  // namespace renderer::view
