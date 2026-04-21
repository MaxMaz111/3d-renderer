#pragma once

#include "util/size.h"

#include "linalg.h"

namespace renderer::kernel {

class ZBuffer {
  using WidthT = Width;
  using HeightT = Height;

 public:
  ZBuffer() = default;
  ZBuffer(Width width, Height height);

  void Clear();
  void ResetTo(Width width, Height height);
  Scalar& Get(Width width, Height height);
  const Scalar& Get(Width width, Height height) const;
  int Width() const;
  int Height() const;

 private:
  int GetIndex(WidthT width, HeightT height) const;

  int width_ = 0;
  std::vector<Scalar> data_;
};

}  // namespace renderer::kernel
