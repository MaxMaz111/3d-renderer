#pragma once

#include <QSize>
#include <optional>

namespace renderer {

enum ViewSignals : uint32_t {
  LEFT_ARROW = 1 << 0,
  UP_ARROW = 1 << 1,
  RIGHT_ARROW = 1 << 2,
  DOWN_ARROW = 1 << 3,
  A_KEY = 1 << 4,
  W_KEY = 1 << 5,
  D_KEY = 1 << 6,
  S_KEY = 1 << 7,
  Q_KEY = 1 << 8,
  E_KEY = 1 << 9,
  RESIZE = 1 << 10,
  NONE = 0
};

struct ViewSignalData {
  using CompositeSignal = uint32_t;
  CompositeSignal signal = NONE;
  std::optional<QSize> size = std::nullopt;
};

}  // namespace renderer
