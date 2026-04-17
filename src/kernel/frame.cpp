#include "frame.h"

#include <algorithm>
#include <cassert>

#include "util/constants.h"

#include "color.h"

namespace renderer::kernel {

Frame::Frame(WidthT width, HeightT height)
    : pixel_data_(width * height),
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
  std::ranges::fill(pixel_data_, PixelEntry{});
}

void Frame::ResetTo(WidthT width, HeightT height) {
  assert(width >= 0);
  assert(height >= 0);
  if (NeedResize(width, height)) {
    image_cache_ = QImage(width, height, QImage::Format_RGB32);
    pixel_data_.assign(width * height, PixelEntry{});
  }
  Clear();
}

void Frame::SetColor(WidthT x, HeightT y, LightSample color) {
  assert(IsBounded(x, y));
  const int idx = Index(x, y);
  auto& pixel = pixel_data_[idx];
  pixel.red = Color::ExtractRed(color.color);
  pixel.green = Color::ExtractGreen(color.color);
  pixel.blue = Color::ExtractBlue(color.color);
  pixel.intensity = color.intensity;
  pixel.colors_blended_cnt = 1;
}

void Frame::AddColor(WidthT x, HeightT y, LightSample color) {
  assert(IsBounded(x, y));
  const int idx = Index(x, y);
  auto& pixel = pixel_data_[idx];
  pixel.red += Color::ExtractRed(color.color);
  pixel.green += Color::ExtractGreen(color.color);
  pixel.blue += Color::ExtractBlue(color.color);
  pixel.intensity = 1;
  pixel.colors_blended_cnt += 1;
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
      const auto& pixel = pixel_data_[idx];
      if (pixel.colors_blended_cnt == 0) {
        image_cache_.setPixel(x, y, kBlackColor);
        continue;
      }
      const Scalar blend_count = pixel.colors_blended_cnt;
      const int mean_red = pixel.red / blend_count;
      const int mean_green = pixel.green / blend_count;
      const int mean_blue = pixel.blue / blend_count;
      image_cache_.setPixel(
          x, y,
          Color::ScaleColor(
              Color::Get(Red{mean_red}, Green{mean_green}, Blue{mean_blue}),
              pixel.intensity));
    }
  }
}

void Frame::HandleHDRImage() const {
  if (pixel_data_.empty()) {
    return;
  }

  const auto max_it =
      std::ranges::max_element(pixel_data_, {}, &PixelEntry::intensity);
  Scalar max_intensity = max_it->intensity;

  if (max_intensity <= kEpsilon) {
    max_intensity = kMaxIntensity;
  }

  for (int y = 0; y < Height(); ++y) {
    for (int x = 0; x < Width(); ++x) {
      const int idx = Index(WidthT{x}, HeightT{y});
      const auto& pixel = pixel_data_[idx];
      if (pixel.colors_blended_cnt == 0) {
        image_cache_.setPixel(x, y, kBlackColor);
        continue;
      }
      const Scalar blend_count = pixel.colors_blended_cnt;
      const int mean_red = pixel.red / blend_count;
      const int mean_green = pixel.green / blend_count;
      const int mean_blue = pixel.blue / blend_count;
      image_cache_.setPixel(
          x, y,
          Color::ScaleColor(
              Color::Get(Red{mean_red}, Green{mean_green}, Blue{mean_blue}),
              pixel.intensity / max_intensity));
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
