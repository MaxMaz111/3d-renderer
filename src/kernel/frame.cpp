#include "frame.h"

#include <cassert>

namespace renderer::kernel {

Frame::Frame(Width width, Height height)
    : width_(width),
      height_(height),
      frame_data_(height_, std::vector<Color>(width_)) {
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
  frame_data_[y][x] = color;
}

void Frame::BlendColor(Width x, Height y, Color color) {
  assert(CheckBounds(x, y));
  frame_data_[y][x].Blend(color);
}

const Color& Frame::GetColor(Width x, Height y) const {
  assert(CheckBounds(x, y));
  return frame_data_[y][x];
}

bool Frame::CheckBounds(Width x, Height y) const {
  return Width{0} <= x && x < width_ && Height{0} <= y && y < height_;
}

}  // namespace renderer::kernel
