#include "kernel.h"

#include "util/constants.h"

#include "scene_loader.h"

namespace renderer::kernel {

Kernel::Kernel(const std::filesystem::path& filename)
    : renderer_(Width{kDefaultWidth}, Height{kDefaultHeight}),
      scene_(SceneLoader::ReadFromJson(filename)),
      observable_(
          [this]() -> const Frame& { return renderer_.Render(scene_); }) {}

void Kernel::Subscribe(Observer* observer) {
  assert(observer);
  observable_.Subscribe(observer);
}

void Kernel::SetScreenDimensions(Width width, Height height) {
  renderer_.ResetTo(width, height);
  scene_.Camera().SetAspectRatio(AspectRatio(width, height));
}

void Kernel::RotateLeft() {
  scene_.Camera().RotateLeft();
}

void Kernel::RotateRight() {
  scene_.Camera().RotateRight();
}

void Kernel::RotateUp() {
  scene_.Camera().RotateUp();
}

void Kernel::RotateDown() {
  scene_.Camera().RotateDown();
}

void Kernel::MoveLeft() {
  scene_.Camera().MoveLeft();
}

void Kernel::MoveRight() {
  scene_.Camera().MoveRight();
}

void Kernel::MoveForward() {
  scene_.Camera().MoveForward();
}

void Kernel::MoveBackward() {
  scene_.Camera().MoveBackward();
}

void Kernel::SwivelLeft() {
  scene_.Camera().SwivelLeft();
}

void Kernel::SwivelRight() {
  scene_.Camera().SwivelRight();
}

void Kernel::SwapRenderingMode() {
  scene_.Camera().SwapRenderingMode();
}

void Kernel::NotifyView() {
  observable_.Notify();
}

}  // namespace renderer::kernel
