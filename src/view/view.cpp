#include "view.h"

#include <QSize>

#include "view_signals.h"

namespace renderer {

View::View(QWidget* parent)
    : QLabel(parent),
      observer_([this](const QPixmap& data) { SetData(data); },
                [this](const QPixmap& data) { SetData(data); },
                Observer::DoNothing),
      observable_([this]() { return cur_signal_data_; }) {
  setFocusPolicy(Qt::StrongFocus);
  setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
}

void View::keyPressEvent(QKeyEvent* event) {
  if (std::find(kAvailableKeys.begin(), kAvailableKeys.end(), event->key()) !=
      kAvailableKeys.end()) {
    cur_signal_data_ = KeyToSignal(event->key());
    observable_.Notify();
  }
}

void View::resizeEvent(QResizeEvent* event) {
  cur_signal_data_ = {RESIZE, size()};
  observable_.Notify();
  // update();
  // ^ maybe needed on higher fps?
}

void View::SetData(const QPixmap& pixmap) {
  setPixmap(pixmap);
  // update();
  // ^ maybe needed on higher fps?
}

ViewSignalData View::KeyToSignal(int key) const {
  switch (key) {
    case Qt::Key_Left:
      return {ViewSignals::LEFT_ARROW};
    case Qt::Key_Up:
      return {ViewSignals::UP_ARROW};
    case Qt::Key_Right:
      return {ViewSignals::RIGHT_ARROW};
    case Qt::Key_Down:
      return {ViewSignals::DOWN_ARROW};
    case Qt::Key_A:
      return {ViewSignals::A_KEY};
    case Qt::Key_W:
      return {ViewSignals::W_KEY};
    case Qt::Key_D:
      return {ViewSignals::D_KEY};
    case Qt::Key_S:
      return {ViewSignals::S_KEY};
    default:
      return {ViewSignals::NONE};
  }
}

View::Observer* View::GetObserver() {
  return &observer_;
}

View::Observable* View::GetObservable() {
  return &observable_;
}

}  // namespace renderer
