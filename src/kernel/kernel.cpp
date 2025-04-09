#include "kernel.h"

#include "obj_reader.h"

namespace renderer {

Kernel::Kernel(const std::string& filename)
    : observable_([this]() { return renderer_.Render(scene_); }),
      scene_(ObjReader::ReadFromFile(filename)) {}

void Kernel::Subscribe(Kernel::Observer* observer) {
  assert(observer);
  observable_.Subscribe(observer);
}

void Kernel::SetScreenDimensions(Width width, Height height) {
  scene_.SetScreenDimensions(width, height);
  observable_.Notify();
}

}  // namespace renderer
