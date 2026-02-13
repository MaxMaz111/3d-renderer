#include "color.h"

#include <cstdlib>

namespace renderer::kernel {

Color::Color(RedT red, GreenT green, BlueT blue)
    : r_(red()), g_(green()), b_(blue()) {}

uint8_t Color::Red() const {
  return r_;
}

uint8_t Color::Green() const {
  return g_;
}

uint8_t Color::Blue() const {
  return b_;
}


Color Color::Invert() const {
  return Color(RedT{255 - r_}, GreenT{255 - g_}, BlueT{255 - b_});
}

void Color::Blend(const Color& other, float factor) {
  r_ = r_ + other.Red() * factor;
  g_ = g_ + other.Green() * factor;
  b_ = b_ + other.Blue() * factor;
}

QRgb Color::GetQRgb() const {
  return qRgb(Red(), Green(), Blue());
}

Color Color::GetRandomColor() {
  return {RedT{std::rand() % 256}, GreenT{std::rand() % 256},
          BlueT{std::rand() % 256}};
}

}  // namespace renderer::kernel
