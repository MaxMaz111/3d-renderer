#include "controller.h"

#include "util/size.h"

namespace renderer {

namespace controller {

Controller::Controller(kernel::Kernel* kernel_ptr)
    : kernel_ptr_(kernel_ptr),
      observer_([this](view::ViewSignalData signal_data) {
        HandleSignal(signal_data);
      }) {}

void Controller::HandleSignal(view::ViewSignalData signal_data) const {
  if (HasSignal(signal_data.signal, view::ViewSignals::resize)) {
    assert(signal_data.size != std::nullopt);
    kernel_ptr_->SetScreenDimensions(Width{signal_data.size->width()},
                                     Height{signal_data.size->height()});
  }
  if (HasSignal(signal_data.signal, view::ViewSignals::left_arrow)) {
    kernel_ptr_->RotateLeft();
  }
  if (HasSignal(signal_data.signal, view::ViewSignals::up_arrow)) {
    kernel_ptr_->RotateUp();
  }
  if (HasSignal(signal_data.signal, view::ViewSignals::right_arrow)) {
    kernel_ptr_->RotateRight();
  }
  if (HasSignal(signal_data.signal, view::ViewSignals::down_arrow)) {
    kernel_ptr_->RotateDown();
  }
  if (HasSignal(signal_data.signal, view::ViewSignals::key_a)) {
    kernel_ptr_->MoveLeft();
  }
  if (HasSignal(signal_data.signal, view::ViewSignals::key_w)) {
    kernel_ptr_->MoveForward();
  }
  if (HasSignal(signal_data.signal, view::ViewSignals::key_d)) {
    kernel_ptr_->MoveRight();
  }
  if (HasSignal(signal_data.signal, view::ViewSignals::key_s)) {
    kernel_ptr_->MoveBackward();
  }
  if (HasSignal(signal_data.signal, view::ViewSignals::key_q)) {
    kernel_ptr_->SwivelLeft();
  }
  if (HasSignal(signal_data.signal, view::ViewSignals::key_e)) {
    kernel_ptr_->SwivelRight();
  }
  if (HasSignal(signal_data.signal, view::ViewSignals::key_1)) {
    kernel_ptr_->SetCurrentCamera(0);
  }
  if (HasSignal(signal_data.signal, view::ViewSignals::key_2)) {
    kernel_ptr_->SetCurrentCamera(1);
  }
  if (HasSignal(signal_data.signal, view::ViewSignals::key_3)) {
    kernel_ptr_->SetCurrentCamera(2);
  }
  if (HasSignal(signal_data.signal, view::ViewSignals::key_4)) {
    kernel_ptr_->SetCurrentCamera(3);
  }
  if (HasSignal(signal_data.signal, view::ViewSignals::key_b)) {
    kernel_ptr_->SwapTransparency();
  }
}

util::ColdInput<view::ViewSignalData>* Controller::GetObserver() {
  return &observer_;
}

}  // namespace controller

}  // namespace renderer
