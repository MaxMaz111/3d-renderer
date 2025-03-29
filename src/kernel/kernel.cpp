#include "kernel.h"

namespace renderer {

Kernel::Kernel() : observable_([this]() { return renderer_.Render(scene_); }) {}

void Kernel::Subscribe(Kernel::Observer* observer) {
  assert(observer);
  observable_.Subscribe(observer);
}

}  // namespace renderer
