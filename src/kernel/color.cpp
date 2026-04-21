#include "color.h"

#include <algorithm>

namespace renderer::kernel {

QRgb Color::Get(RedT r, GreenT g, BlueT b) {
  return qRgb(r, g, b);
}

QRgb Color::ScaleColor(QRgb color, Scalar intensity) {
  int r = ClampComponent(ExtractRed(color) * intensity);
  int g = ClampComponent(ExtractGreen(color) * intensity);
  int b = ClampComponent(ExtractBlue(color) * intensity);
  return qRgb(r, g, b);
}

void Color::AddColor(QRgb* base, QRgb new_color) {
  int r = ClampComponent(ExtractRed(*base) + ExtractRed(new_color));
  int g = ClampComponent(ExtractGreen(*base) + ExtractGreen(new_color));
  int b = ClampComponent(ExtractBlue(*base) + ExtractBlue(new_color));
  *base = qRgb(r, g, b);
}

void Color::Blend(QRgb* base, QRgb new_color, Scalar blend_factor) {
  int r = ClampComponent(ExtractRed(*base) * (1 - blend_factor) +
                         ExtractRed(new_color) * blend_factor);
  int g = ClampComponent(ExtractGreen(*base) * (1 - blend_factor) +
                         ExtractGreen(new_color) * blend_factor);
  int b = ClampComponent(ExtractBlue(*base) * (1 - blend_factor) +
                         ExtractBlue(new_color) * blend_factor);
  *base = qRgb(r, g, b);
}

int Color::ExtractRed(QRgb color) {
  return qRed(color);
}

int Color::ExtractGreen(QRgb color) {
  return qGreen(color);
}

int Color::ExtractBlue(QRgb color) {
  return qBlue(color);
}

int Color::ClampComponent(int value) {
  return std::clamp(value, kMinComponent, kMaxComponent);
}

Scalar Color::ClampComponentScalar(Scalar value) {
  return std::clamp(value, kMinComponentScalar, kMaxComponentScalar);
}

}  // namespace renderer::kernel
