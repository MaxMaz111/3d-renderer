#include "frame.h"

#include <cassert>

namespace renderer::kernel {

Frame::Frame(WidthT width, HeightT height)
    : width_(width), data_(width_ * height) {
  assert(width >= WidthT{0});
  assert(height >= HeightT{0});
}

int Frame::Width() const {
  return width_;
}

int Frame::Height() const {
  return std::ssize(data_) / width_;
}

void Frame::SetColor(WidthT x, HeightT y, Color color) {
  assert(IsBounded(x, y));
  data_[x + y * width_] = color;
}

void Frame::BlendColor(WidthT x, HeightT y, Color color) {
  assert(IsBounded(x, y));
  data_[x + y * width_].Blend(color);
}

const Color& Frame::GetColor(WidthT x, HeightT y) const {
  assert(IsBounded(x, y));
  return data_[x + y * width_];
}

bool Frame::IsBounded(WidthT x, HeightT y) const {
  return WidthT{0} <= x && x < Width() && HeightT{0} <= y && y < Height();
}

}  // namespace renderer::kernel
