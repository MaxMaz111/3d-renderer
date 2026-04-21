#include <benchmark/benchmark.h>
#include <memory>

#include "kernel/renderer.h"
#include "kernel/scene.h"
#include "kernel/scene_loader.h"

#include "util/constants.h"

namespace {

using renderer::Height;
using renderer::Width;
using renderer::kernel::Frame;
using renderer::kernel::SceneLoader;
using renderer::kernel::Renderer;
using renderer::kernel::Scene;

class RenderFixture : public benchmark::Fixture {
 public:
  void SetUp(const benchmark::State& state) override {
    scene = std::make_unique<Scene>(
        SceneLoader::ReadFromJson(paths_.at(state.range(0))));
    renderer =
        std::make_unique<Renderer>(Width{1920},
                                   Height{1080});
  }

  void TearDown(const benchmark::State&) override {
    renderer.reset();
    scene.reset();
  }

  std::unique_ptr<Scene> scene;
  std::unique_ptr<Renderer> renderer;

  const std::vector<std::filesystem::path> paths_{
      "./models/basic_scene.json",
  };
};

BENCHMARK_DEFINE_F(RenderFixture, Render)(benchmark::State& state) {
  state.SetLabel(paths_.at(state.range(0)));
  for (auto _ : state) {
    const Frame& frame = renderer->Render(*scene);
    benchmark::DoNotOptimize(&frame);
  }
}

BENCHMARK_REGISTER_F(RenderFixture, Render)
    ->Arg(0)
    ->Unit(benchmark::kMillisecond);

}  // namespace

BENCHMARK_MAIN();
