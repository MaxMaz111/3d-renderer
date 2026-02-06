#include "frame.h"

#include <cassert>

namespace renderer::kernel {

Frame::Frame(WidthT width, HeightT height)
    : width_(width()), data_(width_ * height()) {
  assert(width() >= 0);
  assert(height() >= 0);
}

int Frame::Width() const {
  return width_;
}

int Frame::Height() const {
  return std::ssize(data_) / width_;
}

void Frame::ResetTo(WidthT width, HeightT height) {
  if (NeedResize(width, height)) {
    width_ = width();
    data_.resize(width_ * height());
  } else {
    std::ranges::fill(data_, Color{});
  }
}

void Frame::SetColor(WidthT x, HeightT y, Color color) {
  assert(IsBounded(x, y));
  data_[x() + y() * width_] = color;
}

void Frame::BlendColor(WidthT x, HeightT y, Color color) {
  assert(IsBounded(x, y));
  data_[x() + y() * width_].Blend(color);
}

const Color& Frame::GetColor(WidthT x, HeightT y) const {
  assert(IsBounded(x, y));
  return data_[x() + y() * width_];
}

bool Frame::IsBounded(WidthT x, HeightT y) const {
  return 0 <= x() && x() < Width() && 0 <= y() && y() < Height();
}

bool Frame::NeedResize(WidthT width, HeightT height) const {
  return width() != Width() || height() != Height();
}

}  // namespace renderer::kernel
