#include <benchmark/benchmark.h>

#include "kernel/obj_reader.h"
#include "kernel/renderer.h"
#include "kernel/scene.h"

using namespace renderer::kernel;

static void RenderCow(benchmark::State& state) {
  static Scene scene = ObjReader::ReadFromFile("../models/cow.obj");
  Renderer renderer;

  for (auto _ : state) {
    Frame frame = renderer.Render(scene);
    benchmark::DoNotOptimize(frame);
  }
}

static void RenderPenis(benchmark::State& state) {
  static Scene scene = ObjReader::ReadFromFile("../models/penis.obj");
  Renderer renderer;

  for (auto _ : state) {
    Frame frame = renderer.Render(scene);
    benchmark::DoNotOptimize(frame);
  }
}

BENCHMARK(RenderCow);
BENCHMARK(RenderPenis);

BENCHMARK_MAIN();
