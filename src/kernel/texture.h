#pragma once

#include <QColor>
#include <QImage>
#include <memory>

#include "linalg.h"

namespace renderer::kernel {

class TextureImpl;

class Texture {
 public:
  Texture() = default;
  explicit Texture(QImage image);

  QRgb Sample(const Point2& tex_coord) const;

 private:
  std::shared_ptr<const TextureImpl> impl_;
};

}  // namespace renderer::kernel
