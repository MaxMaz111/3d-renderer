#include <spdlog/spdlog.h>

#include "kernel/obj_reader.h"
#include "kernel/renderer.h"
#include "kernel/scene.h"

int main(int argc, char* argv[]) {
  if (argc < 2) {
    spdlog::error("Usage: {} <path_to_obj_file>", argv[0]);
    return 1;
  }
  renderer::kernel::Renderer renderer(renderer::Width{800},
                                      renderer::Height{600});
  renderer::kernel::Scene scene =
      renderer::kernel::ObjReader::ReadFromFile(argv[1]);
  auto frame = renderer.Render(scene);
  frame.Image().save("output.png");
}
