#include "controller.h"

#include "../size.h"

namespace renderer {

Controller::Controller(Kernel* kernel_ptr)
    : kernel_ptr_(kernel_ptr), observer_([this](ViewSignalData signal_data) {
        HandleSignal(signal_data);
      }) {}

void Controller::HandleSignal(ViewSignalData signal_data) {
  if (signal_data.signal & ViewSignals::RESIZE) {
    assert(signal_data.size != std::nullopt);
    kernel_ptr_->SetScreenDimensions(Width{signal_data.size->width()},
                                     Height{signal_data.size->height()});
  }
  if (signal_data.signal & ViewSignals::LEFT_ARROW) {
    kernel_ptr_->RotateLeft();
  }
  if (signal_data.signal & ViewSignals::UP_ARROW) {
    kernel_ptr_->RotateUp();
  }
  if (signal_data.signal & ViewSignals::RIGHT_ARROW) {
    kernel_ptr_->RotateRight();
  }
  if (signal_data.signal & ViewSignals::DOWN_ARROW) {
    kernel_ptr_->RotateDown();
  }
  if (signal_data.signal & ViewSignals::KEY_A) {
    kernel_ptr_->MoveLeft();
  }
  if (signal_data.signal & ViewSignals::KEY_W) {
    kernel_ptr_->MoveForward();
  }
  if (signal_data.signal & ViewSignals::KEY_D) {
    kernel_ptr_->MoveRight();
  }
  if (signal_data.signal & ViewSignals::KEY_S) {
    kernel_ptr_->MoveBackward();
  }
  if (signal_data.signal & ViewSignals::KEY_Q) {
    kernel_ptr_->SwivelLeft();
  }
  if (signal_data.signal & ViewSignals::KEY_E) {
    kernel_ptr_->SwivelRight();
  }
  if (signal_data.signal & ViewSignals::KEY_1) {
    kernel_ptr_->SetCurrentCamera(0);
  }
  if (signal_data.signal & ViewSignals::KEY_2) {
    kernel_ptr_->SetCurrentCamera(1);
  }
  if (signal_data.signal & ViewSignals::KEY_3) {
    kernel_ptr_->SetCurrentCamera(2);
  }
  if (signal_data.signal & ViewSignals::KEY_4) {
    kernel_ptr_->SetCurrentCamera(3);
  }
}

ColdInput<ViewSignalData>* Controller::GetObserver() {
  return &observer_;
}

}  // namespace renderer
