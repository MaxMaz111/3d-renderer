#pragma once

#include <cstdint>

namespace renderer {

class Color {
 public:
  Color() = default;
  Color(int red, int green, int blue);

  uint8_t GetRed() const;
  uint8_t GetGreen() const;
  uint8_t GetBlue() const;
  void SetRed(int red);
  void SetGreen(int green);
  void SetBlue(int blue);
  Color Invert() const;
  void Blend(const Color& other, float factor = 0.2);
  bool operator==(const Color& other) const;
  bool operator!=(const Color& other) const;
  static Color GetRandomColor();

 private:
  static uint8_t Clamp(int value);

  uint8_t r_ = 0;
  uint8_t g_ = 0;
  uint8_t b_ = 0;
};
}  // namespace renderer
