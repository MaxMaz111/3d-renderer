#pragma once

#include <linalg.h>

namespace renderer::kernel {

constexpr int kDefaultWidth = 800;
constexpr int kDefaultHeight = 600;

constexpr Scalar AspectRatio(const Width width, const Height height) {
  return static_cast<Scalar>(width) / static_cast<Scalar>(height);
}

}  // namespace renderer::kernel
