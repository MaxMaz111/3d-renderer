#pragma once

#include <cstdint>

#include "linalg.h"

namespace renderer {

class Color {
 public:
  Color();
  Color(int red, int green, int blue);
  uint8_t GetRed() const;
  uint8_t GetGreen() const;
  uint8_t GetBlue() const;
  void SetRed(int red);
  void SetGreen(int green);
  void SetBlue(int blue);
  Color Invert() const;
  void Blend(const Color& other, Scalar factor = 0.2);
  bool operator==(const Color& other) const;
  bool operator!=(const Color& other) const;
  static Color GetRandomColor();

 private:
  static uint8_t Clamp(int value);

  uint8_t r_;
  uint8_t g_;
  uint8_t b_;
};
}  // namespace renderer
