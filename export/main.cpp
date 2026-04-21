#include <spdlog/spdlog.h>

#include "kernel/renderer.h"
#include "kernel/scene.h"
#include "kernel/scene_loader.h"

int main(int argc, char* argv[]) {
  if (argc < 2) {
    spdlog::error("Usage: {} <path_to_scene_json>", argv[0]);
    return 1;
  }
  renderer::kernel::Renderer renderer(renderer::Width{3840},
                                      renderer::Height{2160});

  renderer::kernel::Scene scene =
      renderer::kernel::SceneLoader::ReadFromJson(argv[1]);
  scene.Camera().SetDimensions(renderer::Width{3840},
                                      renderer::Height{2160});
  
  auto frame = renderer.Render(scene);
  frame.Image().save("output.png");
}
