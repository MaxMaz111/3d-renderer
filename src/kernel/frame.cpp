#include "frame.h"

#include <cassert>

#include "util/constants.h"

#include "color.h"

namespace renderer::kernel {

Frame::Frame(WidthT width, HeightT height)
    : image_(width, height, QImage::Format_RGB32) {
  assert(width >= 0);
  assert(height >= 0);
  image_.fill(kBlackColor);
}

int Frame::Width() const {
  return image_.width();
}

int Frame::Height() const {
  return image_.height();
}

void Frame::Clear() {
  image_.fill(kBlackColor);
}

void Frame::ResetTo(WidthT width, HeightT height) {
  assert(width >= 0);
  assert(height >= 0);
  if (NeedResize(width, height)) {
    image_ = QImage(width, height, QImage::Format_RGB32);
  }
  Clear();
}

void Frame::SetColor(WidthT x, HeightT y, QRgb color) {
  assert(IsBounded(x, y));
  image_.setPixel(x, y, color);
}

void Frame::BlendColor(WidthT x, HeightT y, QRgb color) {
  assert(IsBounded(x, y));
  QRgb base = image_.pixel(x, y);
  Color::Blend(&base, color, kBlendFactor);
  image_.setPixel(x, y, base);
}

QRgb Frame::Color(WidthT x, HeightT y) const {
  assert(IsBounded(x, y));
  return image_.pixel(x, y);
}

const QImage& Frame::Image() const {
  return image_;
}

bool Frame::IsBounded(WidthT x, HeightT y) const {
  return 0 <= x && x < Width() && 0 <= y && y < Height();
}

bool Frame::NeedResize(WidthT width, HeightT height) const {
  return width != Width() || height != Height();
}

}  // namespace renderer::kernel
