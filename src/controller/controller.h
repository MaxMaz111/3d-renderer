#pragma once

#include "../kernel/kernel.h"

#include <QMouseEvent>
#include <QObject>

namespace renderer {

class Controller {
 public:
  Controller() = delete;
  Controller(Kernel* kernel_ptr);

 private:
  Kernel* kernel_ptr_;
};

}  // namespace renderer
