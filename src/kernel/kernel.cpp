#include "kernel.h"

#include "obj_reader.h"

namespace renderer {

Kernel::Kernel(const std::string& filename)
    : scene_(ObjReader::ReadFromFile(filename)),
      observable_(renderer_.Render(scene_)) {}

void Kernel::Subscribe(Observer<QPixmap>* observer) {
  assert(observer);
  observable_.Subscribe(observer);
}

void Kernel::SetScreenDimensions(Width width, Height height) {
  scene_.SetScreenDimensions(width, height);
  observable_.Set(renderer_.Render(scene_));
}

void Kernel::RotateLeft() {
  scene_.RotateLeft();
  observable_.Set(renderer_.Render(scene_));
}

void Kernel::RotateRight() {
  scene_.RotateRight();
  observable_.Set(renderer_.Render(scene_));
}

void Kernel::RotateUp() {
  scene_.RotateUp();
  observable_.Set(renderer_.Render(scene_));
}

void Kernel::RotateDown() {
  scene_.RotateDown();
  observable_.Set(renderer_.Render(scene_));
}

void Kernel::MoveLeft() {
  scene_.MoveLeft();
  observable_.Set(renderer_.Render(scene_));
}

void Kernel::MoveRight() {
  scene_.MoveRight();
  observable_.Set(renderer_.Render(scene_));
}

void Kernel::MoveForward() {
  scene_.MoveForward();
  observable_.Set(renderer_.Render(scene_));
}

void Kernel::MoveBackward() {
  scene_.MoveBackward();
  observable_.Set(renderer_.Render(scene_));
}

void Kernel::SwivelLeft() {
  scene_.SwivelLeft();
  observable_.Set(renderer_.Render(scene_));
}

void Kernel::SwivelRight() {
  scene_.SwivelRight();
  observable_.Set(renderer_.Render(scene_));
}

}  // namespace renderer
