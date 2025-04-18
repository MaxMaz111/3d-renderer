#pragma once

#include <vector>

#include "../size.h"
#include "kernel/color.h"

namespace renderer {

class Frame {
 public:
  Frame(Width width, Height height);
  Width GetWidth() const;
  Height GetHeight() const;
  void SetColor(Width x, Height y, Color color);
  const Color& GetColor(Width x, Height y) const;

 private:
  bool CheckBounds(Width width, Height height) const;

  Width width_;
  Height height_;
  std::vector<std::vector<Color>> frame_data_;
};

}  // namespace renderer
