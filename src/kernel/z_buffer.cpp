#include "z_buffer.h"

#include <limits>

namespace renderer::kernel {

ZBuffer::ZBuffer(WidthT width, HeightT height)
    : width_(width),
      data_(width_ * height, std::numeric_limits<Scalar>::infinity()) {}

void ZBuffer::Clear() {
  std::ranges::fill(data_, std::numeric_limits<Scalar>::infinity());
}

void ZBuffer::ResetTo(WidthT width, HeightT height) {
  width_ = width;
  data_.assign(width_ * height, std::numeric_limits<Scalar>::infinity());
}

Scalar& ZBuffer::Get(WidthT width, HeightT height) {
  return data_[GetIndex(width, height)];
}

const Scalar& ZBuffer::Get(WidthT width, HeightT height) const {
  return data_[GetIndex(width, height)];
}

int ZBuffer::Width() const {
  return width_;
}

int ZBuffer::Height() const {
  return width_ == 0 ? 0 : data_.size() / width_;
}

int ZBuffer::GetIndex(WidthT width, HeightT height) const {
  return width + height * width_;
}

}  // namespace renderer::kernel
