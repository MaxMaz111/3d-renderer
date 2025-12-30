#include "view.h"

#include <QSize>

#include "util/observer.hpp"

#include "view_signals.h"

namespace renderer::view {

View::View(QWidget* parent)
    : QLabel(parent),
      observer_([this](kernel::Frame frame) { SetFrame(frame); }),
      observable_(cur_signal_data_) {
  setFocusPolicy(Qt::StrongFocus);
  setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
}

void View::keyPressEvent(QKeyEvent* event) {
  SetSignal(cur_signal_data_, KeyToSignal(event->key()));
  observable_.Set(cur_signal_data_);
}

void View::keyReleaseEvent(QKeyEvent* event) {
  ClearSignal(cur_signal_data_, KeyToSignal(event->key()));
  observable_.Set(cur_signal_data_);
}

void View::resizeEvent(QResizeEvent*) {
  SetSignal(cur_signal_data_, ViewSignals::resize);
  cur_signal_data_.size = size();
  observable_.Set(cur_signal_data_);
  ClearSignal(cur_signal_data_, ViewSignals::resize);
}

void View::SetFrame(kernel::Frame frame) {
  QImage image(frame.GetWidth(), frame.GetHeight(), QImage::Format_RGB32);
  for (int x = 0; x < frame.GetWidth(); ++x) {
    for (int y = 0; y < frame.GetHeight(); ++y) {
      kernel::Color color = frame.GetColor(Width{x}, Height{y});
      image.setPixel(x, y,
                     qRgb(color.GetRed(), color.GetGreen(), color.GetBlue()));
    }
  }
  setPixmap(QPixmap::fromImage(image));
}

ViewSignals View::KeyToSignal(int key) {
  switch (key) {
    case Qt::Key_Left:
      return ViewSignals::left_arrow;
    case Qt::Key_Up:
      return ViewSignals::up_arrow;
    case Qt::Key_Right:
      return ViewSignals::right_arrow;
    case Qt::Key_Down:
      return ViewSignals::down_arrow;
    case Qt::Key_A:
      return ViewSignals::key_a;
    case Qt::Key_W:
      return ViewSignals::key_w;
    case Qt::Key_D:
      return ViewSignals::key_d;
    case Qt::Key_S:
      return ViewSignals::key_s;
    case Qt::Key_Q:
      return ViewSignals::key_q;
    case Qt::Key_E:
      return ViewSignals::key_e;
    case Qt::Key_1:
      return ViewSignals::key_1;
    case Qt::Key_2:
      return ViewSignals::key_2;
    case Qt::Key_3:
      return ViewSignals::key_3;
    case Qt::Key_4:
      return ViewSignals::key_4;
    case Qt::Key_B:
      return ViewSignals::key_b;
    default:
      return ViewSignals::none;
  }
}

util::HotInput<kernel::Frame>* View::GetObserver() {
  return &observer_;
}

void View::Subscribe(util::Observer<ViewSignalData>* observer) {
  observable_.Subscribe(observer);
}

void View::SetSignal(ViewSignalData& data, ViewSignals signal) {
  data.signal |= signal;
}

void View::ClearSignal(ViewSignalData& data, ViewSignals signal) {
  data.signal &= ~signal;
}

}  // namespace renderer::view
