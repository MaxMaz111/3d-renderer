#include "z_buffer.h"

namespace renderer::kernel {

ZBuffer::ZBuffer(Width width, Height height)
    : width_(width()), data_(width_ * height(), 1) {}

void ZBuffer::ResetTo(Width width, Height height) {
  width_ = width();
  data_.assign(width_ * height(), 1);
}

Scalar& ZBuffer::Get(Width width, Height height) {
  return data_[GetIndex(width, height)];
}

const Scalar& ZBuffer::Get(Width width, Height height) const {
  return data_[GetIndex(width, height)];
}

int ZBuffer::GetIndex(Width width, Height height) const {
  return width() + height() * width_;
}

}  // namespace renderer::kernel
