#pragma once

#include <QColor>

#include "kernel/linalg.h"

#include "size.h"

namespace renderer::kernel {

constexpr int kDefaultWidth = 800;
constexpr int kDefaultHeight = 600;
constexpr QRgb kBlackColor = qRgb(0, 0, 0);
constexpr QRgb kWhiteColor = qRgb(255, 255, 255);

constexpr Scalar AspectRatio(const Width width, const Height height) {
  return static_cast<Scalar>(width()) / static_cast<Scalar>(height());
}

}  // namespace renderer::kernel
