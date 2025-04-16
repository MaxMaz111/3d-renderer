#include "view.h"

#include <QSize>

#include "observer.hpp"
#include "../timer.h"
#include "view_signals.h"

namespace renderer {

View::View(QWidget* parent)
    : QLabel(parent),
      observer_([this](auto data) { SetData(data); }),
      observable_(cur_signal_data_) {
  setFocusPolicy(Qt::StrongFocus);
  setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
}

void View::keyPressEvent(QKeyEvent* event) {
  Timer key_timer("Key timer");
  cur_signal_data_.signal |= KeyToSignal(event->key());
  observable_.Set(cur_signal_data_);
}

void View::keyReleaseEvent(QKeyEvent* event) {
  assert(cur_signal_data_.signal & KeyToSignal(event->key()));
  cur_signal_data_.signal ^= KeyToSignal(event->key());
  observable_.Set(cur_signal_data_);
}

void View::resizeEvent(QResizeEvent*) {
  cur_signal_data_.signal ^= RESIZE;
  cur_signal_data_.size = size();
  observable_.Set(cur_signal_data_);
  cur_signal_data_.signal ^= RESIZE;
}

void View::SetData(QPixmap pixmap) {
  setPixmap(pixmap);
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
      return ViewSignals::A_KEY;
    case Qt::Key_W:
      return ViewSignals::W_KEY;
    case Qt::Key_D:
      return ViewSignals::D_KEY;
    case Qt::Key_S:
      return ViewSignals::S_KEY;
    case Qt::Key_Q:
      return ViewSignals::Q_KEY;
    case Qt::Key_E:
      return ViewSignals::E_KEY;
    default:
      return ViewSignals::NONE;
  }
}

HotInput<QPixmap>* View::GetObserver() {
  return &observer_;
}

ObservableData<ViewSignalData>* View::GetObservable() {
  return &observable_;
}

}  // namespace renderer
