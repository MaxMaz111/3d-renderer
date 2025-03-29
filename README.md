# 3d-renderer

This project uses CMake (version ≥ 3.16), Qt6 (Widgets, Core, Gui), and Eigen (version ≥ 3.3). Below are instructions for installing the required libraries on Linux and macOS, along with basic steps to build this project.

---

## 1. Prerequisites

- C++20-compatible compiler (e.g., g++ ≥ 10, clang ≥ 10, or Apple Clang ≥ 12)
- CMake ≥ 3.16
- Qt6 (Widgets, Core, Gui)
- Eigen3 ≥ 3.3

## 2. Installing Dependencies

### 2.1 Linux (Example: Ubuntu/Debian)

1. Update your package list:

   ```bash
   sudo apt-get update
   ```

2. Install CMake:

   ```bash
   sudo apt-get install cmake
   ```

   - If your distribution’s default CMake is older than 3.16, consider installing a newer version from a backport repository or building CMake from source.

3. Install Qt6:

   - On some newer distributions, Qt6 packages may be available:
     ```bash
     sudo apt-get install qt6-base-dev
     ```
   - If Qt6 is not available in your default repositories, you may need to download it from the official Qt website or install a Qt 6 PPA (if available for your distro).

4. Install Eigen3:
   ```bash
   sudo apt-get install libeigen3-dev
   ```

### 2.2 macOS

1. Install Homebrew (if you haven’t already). See instructions at:
   [https://brew.sh/](https://brew.sh/)

2. Install CMake:

   ```bash
   brew install cmake
   ```

3. Install Qt (by default, Homebrew installs the latest Qt, which may be Qt6):

   ```bash
   brew install qt
   ```

   After installation, make sure Homebrew’s Qt bin directory is in your PATH or update your CMake configuration accordingly.

4. Install Eigen3:
   ```bash
   brew install eigen
   ```

## 3. Building the Project

Once all dependencies are installed:

1. Clone or download this repository:

   ```bash
   git clone git@github.com:MaxMaz111/3d-renderer.git
   cd 3d-renderer
   ```

2. Create a build directory and navigate into it:

   ```bash
   mkdir build
   cd build
   ```

3. Run CMake:

   ```bash
   cmake ..
   ```

   - If CMake has trouble finding Qt on macOS, ensure you have exported Qt’s paths, for example:
     ```bash
     export CMAKE_PREFIX_PATH="/usr/local/opt/qt:"$CMAKE_PREFIX_PATH
     ```
     (adjust for your Qt install path if needed).

4. Compile the project:

   ```bash
   make renderer
   ```

5. Run the compiled executable (e.g., “renderer”):
   ```bash
   ./renderer
   ```

---
