#include "frame.h"

#include <algorithm>
#include <cassert>

#include "util/constants.h"

#include "color.h"

namespace renderer::kernel {

Frame::Frame(WidthT width, HeightT height)
    : base_colors_(width * height, kBlackColor),
      intensities_(width * height, 0),
      image_cache_(width, height, QImage::Format_RGB32) {
  assert(width >= 0);
  assert(height >= 0);
  image_cache_.fill(kBlackColor);
}

int Frame::Width() const {
  return image_cache_.width();
}

int Frame::Height() const {
  return image_cache_.height();
}

void Frame::Clear() {
  image_cache_.fill(kBlackColor);
  std::ranges::fill(base_colors_, kBlackColor);
  std::ranges::fill(intensities_, 0);
}

void Frame::ResetTo(WidthT width, HeightT height) {
  assert(width >= 0);
  assert(height >= 0);
  if (NeedResize(width, height)) {
    image_cache_ = QImage(width, height, QImage::Format_RGB32);
    base_colors_.assign(width * height, kBlackColor);
    intensities_.assign(width * height, 0);
  }
  Clear();
}

void Frame::SetColor(WidthT x, HeightT y, LightSample color) {
  assert(IsBounded(x, y));
  const int idx = Index(x, y);
  base_colors_[idx] = color.color;
  intensities_[idx] = color.intensity;
}

void Frame::AddColor(WidthT x, HeightT y, LightSample color) {
  assert(IsBounded(x, y));
  const int idx = Index(x, y);
  Color::AddColor(&base_colors_[idx],
                  Color::ScaleColor(color.color, color.intensity));
  intensities_[idx] += color.intensity * kBlendFactor;
}

const QImage& Frame::Image() const {

  switch (hdr_mode_) {
    case HDRMode::Disabled:
      HandleDefaultImage();
      break;
    case HDRMode::Enabled:
      HandleHDRImage();
      break;
  }
  return image_cache_;
}

void Frame::ToggleHDR() {
  if (hdr_mode_ == HDRMode::Disabled) {
    hdr_mode_ = HDRMode::Enabled;
  } else {
    hdr_mode_ = HDRMode::Disabled;
  }
}

void Frame::HandleDefaultImage() const {
  for (int y = 0; y < Height(); ++y) {
    for (int x = 0; x < Width(); ++x) {
      const int idx = Index(WidthT{x}, HeightT{y});
      const auto& color =
          Color::ScaleColor(base_colors_[idx], intensities_[idx]);
      image_cache_.setPixel(x, y, color);
    }
  }
}

void Frame::HandleHDRImage() const {
  auto max_intensity = *std::ranges::max_element(intensities_);
  if (max_intensity == 0) {
    max_intensity = 1;
  }

  for (int y = 0; y < Height(); ++y) {
    for (int x = 0; x < Width(); ++x) {
      const int idx = Index(WidthT{x}, HeightT{y});
      const auto& color = Color::ScaleColor(base_colors_[idx],
                                            intensities_[idx] / max_intensity);
      image_cache_.setPixel(x, y, color);
    }
  }
}

bool Frame::IsBounded(WidthT x, HeightT y) const {
  return 0 <= x && x < Width() && 0 <= y && y < Height();
}

bool Frame::NeedResize(WidthT width, HeightT height) const {
  return width != Width() || height != Height();
}

int Frame::Index(WidthT x, HeightT y) const {
  return x + y * Width();
}

}  // namespace renderer::kernel
