#include "view.h"

#include <QSize>

#include "../constants.h"

namespace renderer {

View::View(QWidget* parent)
    : QLabel(parent),
      observer_([this](const ReceivedData& data) { SetData(data); },
                [this](const ReceivedData& data) { SetData(data); },
                [](const ReceivedData& data) {}) {
  setFocusPolicy(Qt::StrongFocus);
}

void View::mouseMoveEvent(QMouseEvent* event) {
  qDebug() << "Mouse moved to " << event->pos();
}

void View::mousePressEvent(QMouseEvent* event) {
  qDebug() << "Mouse pressed at " << event->pos();
}

void View::mouseReleaseEvent(QMouseEvent* event) {
  qDebug() << "Mouse released at " << event->pos();
}

void View::keyPressEvent(QKeyEvent* event) {
  qDebug() << "Key pressed at " << event->key();
}

void View::focusInEvent(QFocusEvent* event) {
  qDebug() << "Focus in";
}

void View::focusOutEvent(QFocusEvent* event) {
  qDebug() << "Focus out";
}

void View::SetData(const QPixmap& pixmap) {
  assert(pixmap.size() ==
         QSize(constants::kWindowWidth, constants::kWindowHeight));
  setPixmap(pixmap);
  update();
}

View::Observer* View::GetObserver() {
  return &observer_;
}

}  // namespace renderer
