#include "view.h"

#include <QSize>

#include "observer.hpp"
#include "view_signals.h"

namespace renderer {

View::View(QWidget* parent)
    : QLabel(parent),
      observer_([this](Frame frame) { SetFrame(frame); }),
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
  SetSignal(cur_signal_data_, RESIZE);
  cur_signal_data_.size = size();
  observable_.Set(cur_signal_data_);
  ClearSignal(cur_signal_data_, RESIZE);
}

void View::SetFrame(Frame frame) {
  QImage image(static_cast<int>(frame.GetWidth()),
               static_cast<int>(frame.GetHeight()), QImage::Format_RGB32);
  for (int x = 0; x < static_cast<int>(frame.GetWidth()); ++x) {
    for (int y = 0; y < static_cast<int>(frame.GetHeight()); ++y) {
      Color color = frame.GetColor(Width{x}, Height{y});
      image.setPixel(x, y,
                     qRgb(color.GetRed(), color.GetGreen(), color.GetBlue()));
    }
  }
  setPixmap(QPixmap::fromImage(image));
}

ViewSignals View::KeyToSignal(int key) {
  switch (key) {
    case Qt::Key_Left:
      return ViewSignals::LEFT_ARROW;
    case Qt::Key_Up:
      return ViewSignals::UP_ARROW;
    case Qt::Key_Right:
      return ViewSignals::RIGHT_ARROW;
    case Qt::Key_Down:
      return ViewSignals::DOWN_ARROW;
    case Qt::Key_A:
      return ViewSignals::KEY_A;
    case Qt::Key_W:
      return ViewSignals::KEY_W;
    case Qt::Key_D:
      return ViewSignals::KEY_D;
    case Qt::Key_S:
      return ViewSignals::KEY_S;
    case Qt::Key_Q:
      return ViewSignals::KEY_Q;
    case Qt::Key_E:
      return ViewSignals::KEY_E;
    case Qt::Key_1:
      return ViewSignals::KEY_1;
    case Qt::Key_2:
      return ViewSignals::KEY_2;
    case Qt::Key_3:
      return ViewSignals::KEY_3;
    case Qt::Key_4:
      return ViewSignals::KEY_4;
    case Qt::Key_B:
      return ViewSignals::KEY_B;
    default:
      return ViewSignals::NONE;
  }
}

HotInput<Frame>* View::GetObserver() {
  return &observer_;
}

void View::Subscribe(Observer<ViewSignalData>* observer) {
  observable_.Subscribe(observer);
}

void View::SetSignal(ViewSignalData& data, ViewSignals signal) {
  data.signal |= signal;
}

void View::ClearSignal(ViewSignalData& data, ViewSignals signal) {
  data.signal &= ~signal;
}

}  // namespace renderer
