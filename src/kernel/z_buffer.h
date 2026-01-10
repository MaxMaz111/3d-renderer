#pragma once

#include "linalg.h"
#include "size.h"

namespace renderer::kernel {

class ZBuffer {
 public:
  ZBuffer() = default;
  ZBuffer(Width width, Height height);

  void ResetTo(Width width, Height height);
  Scalar& Get(Width width, Height height);
  const Scalar& Get(Width width, Height height) const;

 private:
  inline int GetIndex(Width width, Height height) const;

  int width_;
  std::vector<Scalar> data_;
};

}  // namespace renderer::kernel
