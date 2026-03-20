#pragma once

#include "util/size.h"

#include "linalg.h"

namespace renderer::kernel {

class ZBuffer {
 public:
  ZBuffer() = default;
  ZBuffer(Width width, Height height);

  void Clear();
  void ResetTo(Width width, Height height);
  Scalar& Get(Width width, Height height);
  const Scalar& Get(Width width, Height height) const;

 private:
  int GetIndex(Width width, Height height) const;

  int width_;
  std::vector<Scalar> data_;
};

}  // namespace renderer::kernel
