#pragma once

#include <QLabel>
#include <QMainWindow>

#include "../size.h"
#include "view.h"

namespace renderer {

class MainWindow {
 public:
  MainWindow(Width width, Height height);
  View* GetView();

 private:
  QMainWindow window_;
  View view_;
};

}  // namespace renderer
