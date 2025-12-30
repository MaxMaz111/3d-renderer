#pragma once

#include <QSize>
#include <optional>

namespace renderer {

namespace view {

enum class ViewSignals : uint32_t {
  left_arrow = 1 << 0,
  up_arrow = 1 << 1,
  right_arrow = 1 << 2,
  down_arrow = 1 << 3,
  key_a = 1 << 4,
  key_w = 1 << 5,
  key_d = 1 << 6,
  key_s = 1 << 7,
  key_q = 1 << 8,
  key_e = 1 << 9,
  key_1 = 1 << 10,
  key_2 = 1 << 11,
  key_3 = 1 << 12,
  key_4 = 1 << 13,
  key_b = 1 << 14,
  resize = 1 << 15,
  none = 0
};

constexpr ViewSignals operator|(ViewSignals lhs, ViewSignals rhs) {
  return ViewSignals{std::to_underlying(lhs) | std::to_underlying(rhs)};
}

constexpr ViewSignals operator&(ViewSignals lhs, ViewSignals rhs) {
  return ViewSignals{std::to_underlying(lhs) & std::to_underlying(rhs)};
}

constexpr ViewSignals& operator|=(ViewSignals& lhs, ViewSignals rhs) {
  return lhs = lhs | rhs;
}

constexpr ViewSignals& operator&=(ViewSignals& lhs, ViewSignals rhs) {
  return lhs = lhs & rhs;
}

constexpr ViewSignals operator~(ViewSignals signal) {
  return ViewSignals{~std::to_underlying(signal)};
}

constexpr bool HasSignal(ViewSignals signal_mask, ViewSignals signal) {
  return std::to_underlying(signal_mask & signal);
}

struct ViewSignalData {
  ViewSignals signal = ViewSignals::none;
  std::optional<QSize> size = std::nullopt;
};

}  // namespace view

}  // namespace renderer
