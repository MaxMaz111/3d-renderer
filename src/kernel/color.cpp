#include "color.h"

#include <algorithm>

namespace renderer::kernel {

QRgb Color::Get(kernel::Red r, kernel::Green g, kernel::Blue b) {
  return qRgb(r(), g(), b());
}

QRgb Color::ScaleColor(QRgb color, Scalar intensity) {
  int r = ClampComponent(Red(color) * intensity);
  int g = ClampComponent(Green(color) * intensity);
  int b = ClampComponent(Blue(color) * intensity);
  return qRgb(r, g, b);
}

void Color::Blend(QRgb* base, QRgb new_color, Scalar blend_factor) {
  int r = ClampComponent(Red(*base) * (1 - blend_factor) +
                         Red(new_color) * blend_factor);
  int g = ClampComponent(Green(*base) * (1 - blend_factor) +
                         Green(new_color) * blend_factor);
  int b = ClampComponent(Blue(*base) * (1 - blend_factor) +
                         Blue(new_color) * blend_factor);
  *base = qRgb(r, g, b);
}

int Color::ClampComponent(int value) {
  return std::clamp(value, kMinComponent, kMaxComponent);
}

Scalar Color::ClampComponentScalar(Scalar value) {
  return std::clamp(value, kMinComponentScalar, kMaxComponentScalar);
}

int Color::Red(QRgb color) {
  return qRed(color);
}

int Color::Green(QRgb color) {
  return qGreen(color);
}

int Color::Blue(QRgb color) {
  return qBlue(color);
}

}  // namespace renderer::kernel
