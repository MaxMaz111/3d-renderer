#pragma once

#include <QColor>
#include <QImage>
#include <vector>

#include "util/size.h"

#include "light_sample.h"
#include "linalg.h"

namespace renderer::kernel {

class Frame {
  using WidthT = ::renderer::Width;
  using HeightT = ::renderer::Height;

  enum class HDRMode { Enabled, Disabled };
  static constexpr Scalar kMinIntensity = 0;
  static constexpr Scalar kMaxIntensity = 1;

  struct PixelEntry {
    int red = 0;
    int green = 0;
    int blue = 0;
    Scalar intensity = kMinIntensity;
    int colors_blended_cnt = 0;
  };

 public:
  Frame(WidthT width, HeightT height);

  int Width() const;
  int Height() const;
  void Clear();
  void ResetTo(WidthT width, HeightT height);
  void SetColor(WidthT x, HeightT y, LightSample color);
  void AddColor(WidthT x, HeightT y, LightSample color);
  const QImage& Image() const;
  void ToggleHDR();

 private:
  void HandleDefaultImage() const;
  void HandleHDRImage() const;
  bool IsBounded(WidthT width, HeightT height) const;
  bool NeedResize(WidthT width, HeightT height) const;
  int Index(WidthT x, HeightT y) const;

  HDRMode hdr_mode_ = HDRMode::Disabled;
  std::vector<PixelEntry> pixel_data_;

  mutable QImage image_cache_;
};

}  // namespace renderer::kernel
