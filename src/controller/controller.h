#pragma once

#include <QMouseEvent>
#include <QObject>

#include "../kernel/kernel.h"
#include "../view/view_signals.h"

namespace renderer {

class Controller {
  using Observer = Observer<ViewSignalData>;

 public:
  Controller() = delete;
  Controller(Kernel* kernel_ptr);
  void HandleSignal(ViewSignalData signal);
  Observer* GetObserver();

 private:
  Kernel* kernel_ptr_;
  Observer observer_;
};

}  // namespace renderer
