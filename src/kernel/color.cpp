#include "color.h"

#include <algorithm>

namespace renderer {

Color::Color() : r_(0), g_(0), b_(0) {}

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

Color Color::Blend(const Color& other, float factor) const {
  factor = std::max(0.0f, std::min(1.0f, factor));
  int new_red = static_cast<int>(r_ * (1 - factor) + other.r_ * factor);
  int new_green = static_cast<int>(g_ * (1 - factor) + other.g_ * factor);
  int new_blue = static_cast<int>(b_ * (1 - factor) + other.b_ * factor);
  return Color(new_red, new_green, new_blue);
}

bool Color::operator==(const Color& other) const {
  return r_ == other.r_ && g_ == other.g_ && b_ == other.b_;
}

bool Color::operator!=(const Color& other) const {
  return !(*this == other);
}

Color Color::GetRandomColor() {
  return {rand() % 256, rand() % 256, rand() % 256};
}

uint8_t Color::Clamp(int value) {
  return static_cast<uint8_t>(std::max(0, std::min(255, value)));
}

}  // namespace renderer
