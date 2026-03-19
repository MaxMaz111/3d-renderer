# 3d-renderer

Qt6-based standalone C++ application for rendering 3d models using projective geometry.

## Demo

https://github.com/user-attachments/assets/1243ca00-b903-4074-9fe9-6a3a29e6daa7

## Requirements

- C++23 compiler
- CMake 3.16+
- Qt6 (`Widgets`, `Core`, `Gui`)
- Git

## Quick start

```bash
git clone --recurse-submodules git@github.com:MaxMaz111/3d-renderer.git
cd 3d-renderer

cmake -S . -B build
cmake --build build -j
```

If you already cloned without submodules:

```bash
git submodule update --init --recursive
```

## Run

The app expects a model path as the first argument:

```bash
./build/renderer_app ./models/cube.obj
```

## Other targets

```bash
./build/renderer_tests
./build/renderer_benchmarks
```
