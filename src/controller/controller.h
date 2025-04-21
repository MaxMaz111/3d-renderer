#pragma once

#include <QMouseEvent>
#include <QObject>

#include "../kernel/kernel.h"
#include "../view/view_signals.h"

namespace renderer {

class Controller {
 public:
  Controller() = delete;
  Controller(Kernel* kernel_ptr);
  void HandleSignal(ViewSignalData signal);
  ColdInput<ViewSignalData>* GetObserver();

 private:
  Kernel* kernel_ptr_;
  ColdInput<ViewSignalData> observer_;
};

}  // namespace renderer
