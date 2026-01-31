#include "color.h"

#include <algorithm>

namespace renderer::kernel {

Color::Color(int red, int green, int blue)
    : r_(Clamp(red)), g_(Clamp(green)), b_(Clamp(blue)) {}

uint8_t Color::GetRed() const {
  return r_;
}

uint8_t Color::GetGreen() const {
  return g_;
}

uint8_t Color::GetBlue() const {
  return b_;
}

void Color::SetRed(int red) {
  r_ = Clamp(red);
}

void Color::SetGreen(int green) {
  g_ = Clamp(green);
}

void Color::SetBlue(int blue) {
  b_ = Clamp(blue);
}

Color Color::Invert() const {
  return Color(255 - r_, 255 - g_, 255 - b_);
}

void Color::Blend(const Color& other, float factor) {
  r_ += other.GetRed() * factor;
  g_ += other.GetGreen() * factor;
  b_ += other.GetBlue() * factor;
}

bool Color::operator==(const Color& other) const {
  return r_ == other.r_ && g_ == other.g_ && b_ == other.b_;
}

bool Color::operator!=(const Color& other) const {
  return !(*this == other);
}

QRgb Color::GetQRgb() const {
  return qRgb(GetRed(), GetGreen(), GetBlue());
}

Color Color::GetRandomColor() {
  return {rand() % 256, rand() % 256, rand() % 256};
}

uint8_t Color::Clamp(int value) {
  return std::clamp(value, 0, 255);
}

}  // namespace renderer::kernel
