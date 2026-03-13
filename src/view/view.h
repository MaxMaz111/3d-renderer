#pragma once

#include <QLabel>

#include "kernel/frame.h"

#include "util/observer.hpp"

namespace renderer::view {

class View {
 public:
  View();

  void SetFrame(const kernel::Frame& frame);
  QLabel* Label();
  util::HotInput<kernel::Frame>* KernelPort();

 private:
  QLabel label_;
  util::HotInput<kernel::Frame> kernel_port_;
};

}  // namespace renderer::view
