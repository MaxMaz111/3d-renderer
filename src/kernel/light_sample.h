#pragma once

#include <QRgb>

#include "linalg.h"

namespace renderer::kernel {

struct LightSample {
  QRgb color;
  Scalar intensity;
};

}  // namespace renderer::kernel
