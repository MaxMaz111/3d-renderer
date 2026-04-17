# 3d-renderer

Qt6-based standalone C++ application for rendering 3D models using projective geometry.

## Demo

https://github.com/user-attachments/assets/1243ca00-b903-4074-9fe9-6a3a29e6daa7

## Requirements

- C++23 compiler (Clang/GCC/MSVC with C++23 support)
- CMake 3.16+
- Qt6(`Widgets`, `Core`, `Gui`)
- TBB
- Git

## Install dependencies

### macOS (Homebrew)

```bash
brew install qt tbb
```

If CMake cannot find Qt:

```bash
cmake -S . -B build -DCMAKE_PREFIX_PATH="$(brew --prefix qt)"
```

### Ubuntu/Debian

```bash
sudo apt update
sudo apt install -y qt6-base-dev libtbb-dev
```

## Build

### Clone

```bash
git clone --recurse-submodules git@github.com:MaxMaz111/3d-renderer.git
cd 3d-renderer
```

If you already cloned without submodules:

```bash
git submodule update --init --recursive
```

### Configure and compile

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
```

Debug build:

```bash
cmake -S . -B build-debug -DCMAKE_BUILD_TYPE=Debug
cmake --build build-debug -j
```

## Run

The executable expects a scene JSON path as the first argument:

```bash
./build/renderer_app ./models/basic_scene.json
```

## Interactive controls

- `W` / `S`: move camera forward / backward
- `A` / `D`: strafe camera left / right
- `Left` / `Right`: yaw camera left / right
- `Up` / `Down`: pitch camera up / down
- `Q` / `E`: roll camera left / right
- `B`: toggle rendering mode (`AllSolid` <-> `AllTransparent`)
- `H`: toggle HDR normalization
- `L`: add a shadow-map light at current camera pose

Window resize updates rendering resolution automatically.

## Export mode (headless image output)

`renderer_export` renders one frame at `3840x2160` and writes `output.png` in
the current working directory:

```bash
./build/renderer_export ./models/basic_scene.json
```

## Scene JSON format

Top-level keys:

- `models` (required): array of model entries
- `lights` (required): array of directional lights (can be empty)

### Model entry

- `path` (required, string): model file path
- `is_normalized` (optional, bool, default `false`): normalize model to unit
	bounding box centered at origin
- `position` (optional, `[x, y, z]`, default `[0, 0, 0]`): world translation

### Light entry

- `direction` (optional, `[x, y, z]`, default `[0, 0, -1]`)

Notes:

- Relative model paths are resolved from the JSON file directory.
- Missing required fields or invalid array/value formats trigger JSON parsing
	errors.
- If a model file cannot be loaded, a warning is logged and that model is
	skipped.

Example:

```json
{
	"models": [
		{
			"path": "cube.obj",
			"is_normalized": true,
			"position": [0.0, 0.0, 0.0]
		},
		{
			"path": "cow.obj",
			"is_normalized": true,
			"position": [2.5, 0.0, 0.0]
		}
	],
	"lights": [
		{ "direction": [0.0, -1.0, -1.0] },
		{ "direction": [1.0, 0.0, -1.0] }
	]
}
```

## Tests

Build tests with the normal CMake build, then run either:

```bash
./build/renderer_tests
```

or through CTest:

```bash
ctest --test-dir build --output-on-failure
```

## Benchmarks

```bash
./build/renderer_benchmarks
```

Google Benchmark CLI arguments are supported, for example:

```bash
./build/renderer_benchmarks --benchmark_min_time=1.0
```

## Development utilities

Format all source files under `src/`:

```bash
python3 clang_format_all.py
```

## Project structure

- `app/`: GUI application entry point
- `export/`: offscreen render entry point
- `src/kernel/`: rendering pipeline core
- `src/controller/`: keyboard and resize event handling
- `src/view/`: Qt view layer
- `tests/`: GoogleTest test cases
- `benchmarks/`: Google Benchmark workloads
- `models/`: sample scenes and assets
- `third-party/`: vendored dependencies/submodules

## Troubleshooting

- `Qt6 not found`: pass `-DCMAKE_PREFIX_PATH="$(brew --prefix qt)"` on macOS.
- `Scene file not found`: verify the JSON path passed to the executable.
- Empty render: confirm your scene has model entries and light entries.
- Clone/build issues related to dependencies: run
	`git submodule update --init --recursive`.
