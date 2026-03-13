#include "kernel.h"

#include "util/constants.h"

#include "obj_reader.h"

namespace renderer::kernel {

Kernel::Kernel(const std::filesystem::path& filename)
    : renderer_(Width{kDefaultWidth}, Height{kDefaultHeight}),
      scene_(ObjReader::ReadFromFile(filename)),
      observable_(renderer_.Render(scene_)) {}

void Kernel::Subscribe(util::Observer<Frame>* observer) {
  assert(observer);
  observable_.Subscribe(observer);
}

void Kernel::SetScreenDimensions(Width width, Height height) {
  renderer_.ResetTo(width, height);
  scene_.SetAspectRatio(AspectRatio(width, height));
}

void Kernel::RotateLeft() {
  scene_.RotateLeft();
  observable_.Set(renderer_.Render(scene_));
}

void Kernel::RotateRight() {
  scene_.RotateRight();
}

void Kernel::RotateUp() {
  scene_.RotateUp();
}

void Kernel::RotateDown() {
  scene_.RotateDown();
}

void Kernel::MoveLeft() {
  scene_.MoveLeft();
}

void Kernel::MoveRight() {
  scene_.MoveRight();
}

void Kernel::MoveForward() {
  scene_.MoveForward();
}

void Kernel::MoveBackward() {
  scene_.MoveBackward();
}

void Kernel::SwivelLeft() {
  scene_.SwivelLeft();
}

void Kernel::SwivelRight() {
  scene_.SwivelRight();
}

void Kernel::SwapRenderingMode() {
  scene_.SwapRenderingMode();
}

void Kernel::NotifyView() {
  observable_.Set(renderer_.Render(scene_));
}

}  // namespace renderer::kernel
