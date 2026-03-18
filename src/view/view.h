#pragma once

#include <QLabel>

#include "kernel/frame.h"

#include "util/observer.hpp"

namespace renderer::view {

class View {
  using Frame = kernel::Frame;
  using HotInput = util::HotInput<Frame>;

 public:
  View();

  QLabel* Label();
  HotInput* KernelPort();

 private:
  void SetFrame(const Frame& frame);

  QLabel label_;
  HotInput kernel_port_;
};

}  // namespace renderer::view
