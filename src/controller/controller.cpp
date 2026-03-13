#include "controller.h"

#include <QKeyEvent>

namespace renderer {

namespace detail::controller {

State::KeyIterator::KeyIterator(KeyUnderlying mask) : remaining_(mask) {
  advance();
}

State::KeyIterator::value_type State::KeyIterator::operator*() const {
  return static_cast<value_type>(current_);
}

State::KeyIterator& State::KeyIterator::operator++() {
  remaining_ &= remaining_ - 1;
  advance();
  return *this;
}

bool State::KeyIterator::operator==(std::default_sentinel_t) const {
  return remaining_ == 0;
}

void State::KeyIterator::advance() {
  if (remaining_) {
    current_ = remaining_ & -remaining_;
  }
}

State::KeyRange::KeyRange(KeyUnderlying mask) : mask_(mask) {}

State::KeyIterator State::KeyRange::begin() const {
  return KeyIterator(mask_);
}

std::default_sentinel_t State::KeyRange::end() const {
  return {};
}

State::KeyRange State::HandlePressEvent(int key) {
  key_mask_ |= std::to_underlying(ConvertQtKey(key));
  return KeyRange(key_mask_);
}

State::KeyRange State::HandleReleaseEvent(int key) {
  key_mask_ &= ~std::to_underlying(ConvertQtKey(key));
  return KeyRange(key_mask_);
}

State::Key State::ConvertQtKey(int key) {
  switch (key) {
    case Qt::Key_W:
      return Key::W;
    case Qt::Key_A:
      return Key::A;
    case Qt::Key_S:
      return Key::S;
    case Qt::Key_D:
      return Key::D;
    case Qt::Key_Q:
      return Key::Q;
    case Qt::Key_E:
      return Key::E;
    case Qt::Key_B:
      return Key::B;
    case Qt::Key_Up:
      return Key::UpArrow;
    case Qt::Key_Left:
      return Key::LeftArrow;
    case Qt::Key_Down:
      return Key::DownArrow;
    case Qt::Key_Right:
      return Key::RightArrow;
    default:
      return Key::None;
  }
}

}  // namespace detail::controller

namespace controller {

Controller::Controller(kernel::Kernel* kernel_ptr, QLabel* plot_ptr)
    : kernel_ptr_(kernel_ptr) {
  plot_ptr->installEventFilter(this);
}

bool Controller::eventFilter(QObject* obj, QEvent* event) {
  switch (event->type()) {
    case QEvent::Type::KeyPress:
      KeyPressEventHandler(static_cast<QKeyEvent*>(event));
      break;
    case QEvent::Type::KeyRelease:
      KeyReleaseEventHandler(static_cast<QKeyEvent*>(event));
      break;
    case QEvent::Type::Resize:
      ResizeEventHandler(static_cast<QResizeEvent*>(event));
      break;
    default:
  }
  return QObject::eventFilter(obj, event);
}

void Controller::KeyPressEventHandler(const QKeyEvent* ev) {
  NotifyKernel(keys_.HandlePressEvent(ev->key()));
  kernel_ptr_->NotifyView();
}

void Controller::KeyReleaseEventHandler(const QKeyEvent* ev) {
  NotifyKernel(keys_.HandleReleaseEvent(ev->key()));
  kernel_ptr_->NotifyView();
}

void Controller::ResizeEventHandler(const QResizeEvent* ev) const {
  kernel_ptr_->SetScreenDimensions(Width{ev->size().width()},
                                   Height{ev->size().height()});
  kernel_ptr_->NotifyView();
}

void Controller::NotifyKernel(State::KeyRange range) const {
  for (const auto key : range) {
    switch (key) {
      case State::Key::None:
        break;
      case State::Key::W:
        kernel_ptr_->MoveForward();
        break;
      case State::Key::A:
        kernel_ptr_->MoveLeft();
        break;
      case State::Key::S:
        kernel_ptr_->MoveBackward();
        break;
      case State::Key::D:
        kernel_ptr_->MoveRight();
        break;
      case State::Key::Q:
        kernel_ptr_->SwivelLeft();
        break;
      case State::Key::E:
        kernel_ptr_->SwivelRight();
        break;
      case State::Key::B:
        kernel_ptr_->SwapRenderingMode();
        break;
      case State::Key::UpArrow:
        kernel_ptr_->RotateUp();
        break;
      case State::Key::LeftArrow:
        kernel_ptr_->RotateLeft();
        break;
      case State::Key::DownArrow:
        kernel_ptr_->RotateDown();
        break;
      case State::Key::RightArrow:
        kernel_ptr_->RotateRight();
        break;
    }
  }
}

}  // namespace controller

}  // namespace renderer
