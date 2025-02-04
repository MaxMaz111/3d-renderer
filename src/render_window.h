#pragma once

#include <QImage>
#include <QRasterWindow>

#include "scene.h"

static constexpr int kWindowWidth = 800;
static constexpr int kWindowHeight = 600;

namespace renderer {
class RenderWindow : public QRasterWindow {

 public:
  RenderWindow();

 private:
  void paintEvent(QPaintEvent* event) override;

  QImage RenderScene();

  Scene scene_;
};

}  // namespace renderer
