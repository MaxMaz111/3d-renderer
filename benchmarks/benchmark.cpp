#include <benchmark/benchmark.h>
#include <iostream>
#include <memory>

#include "kernel/obj_reader.h"
#include "kernel/renderer.h"
#include "kernel/scene.h"

#include "constants.h"

using renderer::Height;
using renderer::Width;
using renderer::kernel::Frame;
using renderer::kernel::ObjReader;
using renderer::kernel::Renderer;
using renderer::kernel::Scene;

class RenderFixture : public benchmark::Fixture {
 public:
  void SetUp(const benchmark::State& state) override {
    scene = std::make_unique<Scene>(
        ObjReader::ReadFromFile(paths_.at(state.range(0))));
    renderer =
        std::make_unique<Renderer>(Width{renderer::kernel::kDefaultWidth},
                                   Height{renderer::kernel::kDefaultHeight});
  }

  void TearDown(const benchmark::State&) override {
    renderer.reset();
    scene.reset();
  }

  std::unique_ptr<Scene> scene;
  std::unique_ptr<Renderer> renderer;

  const std::vector<std::filesystem::path> paths_{
      "../models/penis.obj",
      "../models/teapot.obj",
      "../models/cow.obj",
      "../models/cube.obj",
  };
};

BENCHMARK_DEFINE_F(RenderFixture, Render)(benchmark::State& state) {
  state.SetLabel(paths_.at(state.range(0)));
  for (auto _ : state) {
    Frame frame = renderer->Render(*scene);
    benchmark::DoNotOptimize(frame);
  }
}

BENCHMARK_REGISTER_F(RenderFixture, Render)
    ->Arg(0)
    ->Arg(1)
    ->Arg(2)
    ->Arg(3)
    ->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();
