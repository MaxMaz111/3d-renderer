# 3d-renderer

Qt6-based standalone C++ application for rendering 3d models using projective geometry.

## Demo

https://github.com/user-attachments/assets/1243ca00-b903-4074-9fe9-6a3a29e6daa7

## Requirements

- C++23 compiler
- CMake 3.16+
- Qt6(`Widgets`, `Core`, `Gui`)
- TBB
- Git

## Install dependencies

### macOS (Homebrew)

```bash
brew install qt tbb
```

If CMake cannot find Qt, configure with the Qt CMake path:

```bash
cmake -S . -B build -DCMAKE_PREFIX_PATH="$(brew --prefix qt)"
```

### Ubuntu/Debian

```bash
sudo apt update
sudo apt install -y qt6-base-dev libtbb-dev
```

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

The app expects a scene JSON path as the first argument:

```bash
./build/renderer_app ./models/basic_scene.json
```

## Scene JSON

`models` is an array of objects with:
- `path` (string, required)
- `is_normalized` (bool, optional, fallbacks to `false`) — normalizes model to a
	unit bounding box centered at origin
- `position`(`[x, y, z]`, optional, fallbacks to `[0, 0, 0]`)

`lights` is an array of objects with:
- `direction`(`[x, y, z]`, optional, fallbacks to `[0, 0, -1]`)

Relative model paths are resolved from the JSON file directory.

## Example

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

## Other targets

```bash
./build/renderer_tests
./build/renderer_benchmarks
```
