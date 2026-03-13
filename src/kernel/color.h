#pragma once

#include <QColor>

#include "util/alias.h"

#include "linalg.h"

namespace renderer::kernel {

using Red = util::Alias<uint8_t, struct red_tag>;
using Green = util::Alias<uint8_t, struct green_tag>;
using Blue = util::Alias<uint8_t, struct blue_tag>;

class Color {
 public:
  static QRgb Get(Red r, Green g, Blue b);
  static QRgb ScaleColor(QRgb color, Scalar intensity);
  static void Blend(QRgb* base, QRgb new_color, Scalar blend_factor);

 private:
  static constexpr int kMinComponent = 0;
  static constexpr int kMaxComponent = 255;
  static constexpr Scalar kMinComponentScalar = 0.0f;
  static constexpr Scalar kMaxComponentScalar = 1.0f;

  static int ClampComponent(int value);
  static Scalar ClampComponentScalar(Scalar value);
  static int Red(QRgb color);
  static int Green(QRgb color);
  static int Blue(QRgb color);
};

}  // namespace renderer::kernel
