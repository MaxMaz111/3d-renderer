#pragma once

#include <QColor>
#include <QImage>

#include "util/size.h"

#include "linalg.h"

namespace renderer::kernel {

class Frame {
  static constexpr Scalar kBlendFactor = 0.2f;

  using WidthT = ::renderer::Width;
  using HeightT = ::renderer::Height;

 public:
  Frame(WidthT width, HeightT height);

  Frame(const Frame&) = delete;
  Frame(Frame&&) noexcept = delete;
  Frame& operator=(const Frame&) = delete;
  Frame& operator=(Frame&&) noexcept = delete;

  int Width() const;
  int Height() const;
  void Clear();
  void ResetTo(WidthT width, HeightT height);
  void SetColor(WidthT x, HeightT y, QRgb color);
  void BlendColor(WidthT x, HeightT y, QRgb color);
  QRgb Color(WidthT x, HeightT y) const;
  const QImage& Image() const;

 private:
  bool IsBounded(WidthT width, HeightT height) const;
  bool NeedResize(WidthT width, HeightT height) const;

  QImage image_;
};

}  // namespace renderer::kernel
