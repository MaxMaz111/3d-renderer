#pragma once

#include <QSize>
#include <optional>

namespace renderer {

enum ViewSignals : uint32_t {
  LEFT_ARROW = 1 << 0,
  UP_ARROW = 1 << 1,
  RIGHT_ARROW = 1 << 2,
  DOWN_ARROW = 1 << 3,
  KEY_A = 1 << 4,
  KEY_W = 1 << 5,
  KEY_D = 1 << 6,
  KEY_S = 1 << 7,
  KEY_Q = 1 << 8,
  KEY_E = 1 << 9,
  KEY_1 = 1 << 10,
  KEY_2 = 1 << 11,
  KEY_3 = 1 << 12,
  KEY_4 = 1 << 13,
  KEY_B = 1 << 14,
  RESIZE = 1 << 15,
  NONE = 0
};

struct ViewSignalData {
  using CompositeSignal = uint32_t;
  CompositeSignal signal = NONE;
  std::optional<QSize> size = std::nullopt;
};

}  // namespace renderer
