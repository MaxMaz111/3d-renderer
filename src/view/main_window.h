#pragma once

#include <QLabel>
#include <QMainWindow>
#include "view.h"

namespace renderer {

class MainWindow {
 public:
  MainWindow();

  View* GetView();

 private:
  QMainWindow window_;
  View view_;
};

}  // namespace renderer
