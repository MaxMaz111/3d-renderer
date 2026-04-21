#pragma once

#include <QMainWindow>

#include "util/constants.h"

#include "view.h"

namespace renderer::view {

class MainWindow {
  using ViewT = ::renderer::view::View;
  static constexpr auto kDefaultWidth = kernel::kDefaultWidth;
  static constexpr auto kDefaultHeight = kernel::kDefaultHeight;

 public:
  MainWindow();

  void Show();
  ViewT* View();

 private:
  QMainWindow window_;
  ViewT view_;
};

}  // namespace renderer::view
