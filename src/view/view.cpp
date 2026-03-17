#include "view.h"

namespace renderer::view {

View::View()
    : kernel_port_([this](const kernel::Frame& frame) { SetFrame(frame); }) {
  label_.setFocusPolicy(Qt::StrongFocus);
  label_.setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
}

void View::SetFrame(const kernel::Frame& frame) {
  label_.setPixmap(QPixmap::fromImage(frame.Image()));
}

QLabel* View::Label() {
  return &label_;
}

util::HotInput<kernel::Frame>* View::KernelPort() {
  return &kernel_port_;
}

}  // namespace renderer::view
