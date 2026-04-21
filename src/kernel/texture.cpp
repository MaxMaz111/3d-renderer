#include "texture.h"

#include <algorithm>
#include <cmath>

#include "util/constants.h"

namespace renderer::kernel {

class TextureImpl {
 public:
  explicit TextureImpl(QImage image) : image_(std::move(image)) {}

  QRgb Sample(const Point2& tex_coord) const {
    if (image_.isNull()) {
      return kWhiteColor;
    }
    const auto [x, y] = GetPixel(tex_coord);
    return image_.pixel(x, y);
  }

 private:
  struct Pixel {
    int x;
    int y;
  };

  Pixel GetPixel(const Point2& tex_coord) const {
    const Scalar u = std::clamp(tex_coord.x(), Scalar{0}, Scalar{1});
    const Scalar v = std::clamp(tex_coord.y(), Scalar{0}, Scalar{1});

    int x = std::floor(u * image_.width());
    int y = std::floor(v * image_.height());

    x = std::clamp(x, 0, image_.width() - 1);
    y = std::clamp(y, 0, image_.height() - 1);
    return {x, y};
  }

  QImage image_;
};

Texture::Texture(QImage image)
    : impl_(std::make_shared<const TextureImpl>(std::move(image))) {}

QRgb Texture::Sample(const Point2& tex_coord) const {
  if (impl_ == nullptr) {
    return kWhiteColor;
  }
  return impl_->Sample(tex_coord);
}

}  // namespace renderer::kernel
