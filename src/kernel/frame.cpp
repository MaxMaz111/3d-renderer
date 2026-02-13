#include "frame.h"

#include <algorithm>
#include <cassert>
#include <cmath>

namespace renderer::kernel {

namespace {

QRgb Blend(QRgb base, QRgb new_color, Scalar blend_factor) {
  int r = std::round(qRed(base) * (1 - blend_factor) +
                     qRed(new_color) * blend_factor);
  int g = std::round(qGreen(base) * (1 - blend_factor) +
                     qGreen(new_color) * blend_factor);
  int b = std::round(qBlue(base) * (1 - blend_factor) +
                     qBlue(new_color) * blend_factor);
  return qRgb(r, g, b);
}

}  // namespace

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
  }
  std::ranges::fill(data_, qRgb(0, 0, 0));
}

void Frame::SetColor(WidthT x, HeightT y, QRgb color) {
  assert(IsBounded(x, y));
  data_[x() + y() * width_] = color;
}

void Frame::BlendColor(WidthT x, HeightT y, QRgb color) {
  assert(IsBounded(x, y));
  QRgb base = data_[x() + y() * width_];
  data_[x() + y() * width_] = Blend(base, color, kBlendFactor);
}

QRgb Frame::GetColor(WidthT x, HeightT y) const {
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
