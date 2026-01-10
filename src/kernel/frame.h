#pragma once

#include <vector>

#include "util/size.h"

#include "color.h"

namespace renderer::kernel {

class Frame {
  using WidthT = Width;
  using HeightT = Height;

 public:
  Frame(Width width, Height height);

  int Width() const;
  int Height() const;
  void SetColor(WidthT x, HeightT y, Color color);
  void BlendColor(WidthT x, HeightT y, Color color);
  const Color& GetColor(WidthT x, HeightT y) const;

 private:
  bool CheckBounds(WidthT width, HeightT height) const;

  int width_;
  std::vector<Color> data_;
};

}  // namespace renderer::kernel
