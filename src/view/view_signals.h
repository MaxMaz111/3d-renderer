#pragma once

#include <QSize>
#include <optional>

namespace renderer {

enum ViewSignals {
  LEFT_ARROW,
  UP_ARROW,
  RIGHT_ARROW,
  DOWN_ARROW,
  A_KEY,
  W_KEY,
  D_KEY,
  S_KEY,
  RESIZE,
  NONE
};

struct ViewSignalData {
  ViewSignals signal = NONE;
  std::optional<QSize> size = std::nullopt;
};

}  // namespace renderer
