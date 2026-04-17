#pragma once

#include <QColor>

#include "util/alias.h"

#include "linalg.h"

namespace renderer::kernel {

using Red = util::Alias<uint8_t, struct red_tag>;
using Green = util::Alias<uint8_t, struct green_tag>;
using Blue = util::Alias<uint8_t, struct blue_tag>;

class Color {
  using RedT = Red;
  using GreenT = Green;
  using BlueT = Blue;

 public:
  static QRgb Get(Red r, Green g, Blue b);
  static QRgb ScaleColor(QRgb color, Scalar intensity);
  static void AddColor(QRgb* base, QRgb new_color);
  static void Blend(QRgb* base, QRgb new_color, Scalar blend_factor);
  static int ExtractRed(QRgb color);
  static int ExtractGreen(QRgb color);
  static int ExtractBlue(QRgb color);

 private:
  static constexpr int kMinComponent = 0;
  static constexpr int kMaxComponent = 255;
  static constexpr Scalar kMinComponentScalar = 0;
  static constexpr Scalar kMaxComponentScalar = 1;

  static int ClampComponent(int value);
  static Scalar ClampComponentScalar(Scalar value);
};

}  // namespace renderer::kernel
