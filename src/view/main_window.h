#pragma once

#include <QMainWindow>

#include "util/size.h"

#include "view.h"

namespace renderer::view {

class MainWindow {
 public:
  MainWindow(Width width, Height height);

  void Show();
  View* View();

 private:
  QMainWindow window_;
  view::View view_;
};

}  // namespace renderer::view
