#pragma once

#include <QColor>

#include "alias.h"

namespace renderer::kernel {

using Red = util::Alias<uint8_t, struct red_tag>;
using Green = util::Alias<uint8_t, struct green_tag>;
using Blue = util::Alias<uint8_t, struct blue_tag>;

class Color {
  using RedT = Red;
  using GreenT = Green;
  using BlueT = Blue;

 public:
  Color() = default;
  Color(RedT red, GreenT green, BlueT blue);

  uint8_t Red() const;
  uint8_t Green() const;
  uint8_t Blue() const;
  Color Invert() const;
  void Blend(const Color& other, float factor = 0.1);
  QRgb GetQRgb() const;

  static Color GetRandomColor();

 private:
  uint8_t r_ = 0;
  uint8_t g_ = 0;
  uint8_t b_ = 0;
};

}  // namespace renderer::kernel
