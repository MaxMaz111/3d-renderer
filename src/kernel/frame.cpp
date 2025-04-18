#include "frame.h"

#include <cassert>

namespace renderer {

Frame::Frame(Width width, Height height)
    : width_(width),
      height_(height),
      frame_data_(static_cast<int>(height_),
                  std::vector<Color>(static_cast<int>(width_))) {
  assert(width_ > Width{0});
  assert(height_ > Height{0});
}

Width Frame::GetWidth() const {
  return width_;
}

Height Frame::GetHeight() const {
  return height_;
}

void Frame::SetColor(Width x, Height y, Color color) {
  assert(CheckBounds(x, y));
  frame_data_[static_cast<int>(y)][static_cast<int>(x)] = color;
}

const Color& Frame::GetColor(Width x, Height y) const {
  assert(CheckBounds(x, y));
  return frame_data_[static_cast<int>(y)][static_cast<int>(x)];
}

bool Frame::CheckBounds(Width x, Height y) const {
  return Width{0} <= x && x < width_ && Height{0} <= y && y < height_;
}

}  // namespace renderer
