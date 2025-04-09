#include "controller.h"

#include "../size.h"

namespace renderer {

Controller::Controller(Kernel* kernel_ptr)
    : kernel_ptr_(kernel_ptr),
      observer_(
          [this](ViewSignalData signal_data) { HandleSignal(signal_data); },
          Observer::DoNothing, Observer::DoNothing) {}

void Controller::HandleSignal(ViewSignalData signal_data) {
  switch (signal_data.signal) {
    case ViewSignals::LEFT_ARROW:
      kernel_ptr_->RotateLeft();
      break;
    case ViewSignals::UP_ARROW:
      kernel_ptr_->RotateUp();
      break;
    case ViewSignals::RIGHT_ARROW:
      kernel_ptr_->RotateRight();
      break;
    case ViewSignals::DOWN_ARROW:
      kernel_ptr_->RotateDown();
      break;
    case ViewSignals::A_KEY:
      break;
    case ViewSignals::W_KEY:
      break;
    case ViewSignals::D_KEY:
      break;
    case ViewSignals::S_KEY:
      break;
    case ViewSignals::RESIZE:
      assert(signal_data.size != std::nullopt);
      kernel_ptr_->SetScreenDimensions(Width{signal_data.size->width()},
                                       Height{signal_data.size->height()});
      break;
    case ViewSignals::NONE:
      break;
    default:
      break;
  }
}

Controller::Observer* Controller::GetObserver() {
  return &observer_;
}

}  // namespace renderer
