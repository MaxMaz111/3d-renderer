#pragma once

#include "kernel/kernel.h"

#include "view/view_signals.h"

namespace renderer {

namespace controller {

class Controller {
 public:
  Controller(kernel::Kernel* kernel_ptr);

  void HandleSignal(view::ViewSignalData signal) const;
  util::ColdInput<view::ViewSignalData>* GetObserver();

 private:
  kernel::Kernel* kernel_ptr_;
  util::ColdInput<view::ViewSignalData> observer_;
};

}  // namespace controller

}  // namespace renderer
