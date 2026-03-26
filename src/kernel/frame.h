#pragma once

#include <QColor>
#include <QImage>

#include "util/size.h"

#include "linalg.h"

namespace renderer::kernel {

class Frame {
  using WidthT = ::renderer::Width;
  using HeightT = ::renderer::Height;

 public:
  static constexpr Scalar kBlendFactor = 0.2f;

  Frame(WidthT width, HeightT height);

  int Width() const;
  int Height() const;
  void Clear();
  void ResetTo(WidthT width, HeightT height);
  void SetColor(WidthT x, HeightT y, QRgb color);
  void BlendColor(WidthT x, HeightT y, QRgb color);
  QRgb Color(WidthT x, HeightT y) const;
  const QImage& Image() const;
  QRgb* ScanLine(HeightT y);

 private:
  bool IsBounded(WidthT width, HeightT height) const;
  bool NeedResize(WidthT width, HeightT height) const;

  QImage image_;
};

}  // namespace renderer::kernel
