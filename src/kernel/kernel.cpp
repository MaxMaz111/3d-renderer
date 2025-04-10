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

void Kernel::RotateLeft() {
  scene_.RotateLeft();
  observable_.Notify();
}

void Kernel::RotateRight() {
  scene_.RotateRight();
  observable_.Notify();
}

void Kernel::RotateUp() {
  scene_.RotateUp();
  observable_.Notify();
}

void Kernel::RotateDown() {
  scene_.RotateDown();
  observable_.Notify();
}

void Kernel::MoveLeft() {
  scene_.MoveLeft();
  observable_.Notify();
}

void Kernel::MoveRight() {
  scene_.MoveRight();
  observable_.Notify();
}

void Kernel::MoveForward() {
  scene_.MoveForward();
  observable_.Notify();
}

void Kernel::MoveBackward() {
  scene_.MoveBackward();
  observable_.Notify();
}

}  // namespace renderer
