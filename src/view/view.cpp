#include "view.h"

namespace renderer::view {

View::View() : kernel_port_([this](const Frame& frame) { SetFrame(frame); }) {
  label_.setFocusPolicy(Qt::StrongFocus);
  label_.setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
}

QLabel* View::Label() {
  return &label_;
}

View::HotInput* View::KernelPort() {
  return &kernel_port_;
}

void View::SetFrame(const Frame& frame) {
  label_.setPixmap(QPixmap::fromImage(frame.Image()));
}

}  // namespace renderer::view
