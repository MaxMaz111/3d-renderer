#include "view.h"

namespace renderer::view {

View::View()
    : kernel_port_([this](const kernel::Frame& frame) { SetFrame(frame); }) {
  label_.setFocusPolicy(Qt::StrongFocus);
  label_.setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
}

void View::SetFrame(const kernel::Frame& frame) {
  QImage image(frame.Width(), frame.Height(), QImage::Format_RGB32);
  for (int x = 0; x < frame.Width(); ++x) {
    for (int y = 0; y < frame.Height(); ++y) {
      kernel::Color color = frame.GetColor(Width{x}, Height{y});
      image.setPixel(x, y,
                     qRgb(color.GetRed(), color.GetGreen(), color.GetBlue()));
    }
  }
  label_.setPixmap(QPixmap::fromImage(image));
}

QLabel* View::Label() {
  return &label_;
}

util::HotInput<kernel::Frame>* View::KernelPort() {
  return &kernel_port_;
}

}  // namespace renderer::view
