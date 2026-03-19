#include "kernel/frame.h"
#include "kernel/frame.h"
#include "kernel/obj_reader.h"
#include "kernel/renderer.h"

int main() {
  renderer::kernel::Renderer renderer(renderer::Width{800}, renderer::Height{600});
  renderer::kernel::Scene scene(renderer::kernel::ObjReader::ReadFromFile("../models/penis.obj"));

  const renderer::kernel::Frame* frame = nullptr;
  for (int i = 0; i < 10; ++i) {
    frame = &renderer.Render(scene);
  }

  if (frame != nullptr) {
    frame->Image().save("../kek.png", "PNG", -1);
  }
}