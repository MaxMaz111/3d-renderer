#pragma once

#include <QColor>
#include <QImage>

#include "util/size.h"

#include "light_sample.h"
#include "linalg.h"

namespace renderer::kernel {

class Frame {
  using WidthT = ::renderer::Width;
  using HeightT = ::renderer::Height;

  enum class HDRMode { Enabled, Disabled };

 public:
  static constexpr Scalar kBlendFactor = 0.2f;

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
  std::vector<QRgb> base_colors_;
  std::vector<Scalar> intensities_;

  mutable QImage image_cache_;
};

}  // namespace renderer::kernel
