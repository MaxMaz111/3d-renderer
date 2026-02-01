#include <benchmark/benchmark.h>

#include "kernel/obj_reader.h"
#include "kernel/renderer.h"
#include "kernel/scene.h"

using renderer::kernel::Scene;
using renderer::kernel::ObjReader;
using renderer::kernel::Renderer;
using renderer::kernel::Frame;

static void RenderCow(benchmark::State& state) {
  static Scene scene = ObjReader::ReadFromFile("../models/cow.obj");
  Renderer renderer(renderer::Width{800}, renderer::Height{600});

  for (auto _ : state) {
    Frame frame = renderer.Render(scene);
    benchmark::DoNotOptimize(frame);
  }
}

static void RenderPenis(benchmark::State& state) {
  static Scene scene = ObjReader::ReadFromFile("../models/penis.obj");
  Renderer renderer(renderer::Width{800}, renderer::Height{600});

  for (auto _ : state) {
    Frame frame = renderer.Render(scene);
    benchmark::DoNotOptimize(frame);
  }
}

BENCHMARK(RenderCow);
BENCHMARK(RenderPenis);

BENCHMARK_MAIN();
