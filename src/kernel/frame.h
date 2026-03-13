#pragma once

#include <QColor>
#include <vector>

#include "util/size.h"

#include "linalg.h"

namespace renderer::kernel {

class Frame {
  static constexpr Scalar kBlendFactor = 0.05f;

  using WidthT = Width;
  using HeightT = Height;

 public:
  Frame(WidthT width, HeightT height);

  int Width() const;
  int Height() const;
  void ResetTo(WidthT width, HeightT height);
  void SetColor(WidthT x, HeightT y, QRgb color);
  void BlendColor(WidthT x, HeightT y, QRgb color);
  QRgb GetColor(WidthT x, HeightT y) const;

 private:
  bool IsBounded(WidthT width, HeightT height) const;
  bool NeedResize(WidthT width, HeightT height) const;

  int width_;
  std::vector<QRgb> data_;
};

}  // namespace renderer::kernel
