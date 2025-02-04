#include "render_window.h"

#include <QPainter>

namespace renderer {

RenderWindow::RenderWindow() {
  setTitle("3D Renderer");
  resize(kWindowWidth, kWindowHeight);
}

void RenderWindow::paintEvent(QPaintEvent* event) {
  QPainter painter(this);
  QImage image = RenderScene();
  painter.drawImage(0, 0, image);
}

QImage RenderWindow::RenderScene() {
  return scene_.Render();
}

}  // namespace renderer
