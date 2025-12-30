#pragma once

#include <QLabel>
#include <QMouseEvent>

#include "kernel/frame.h"

#include "util/observer.hpp"

#include "view_signals.h"

namespace renderer {

namespace detail::view {

using UnderlyingSignal = uint32_t;

enum class Signal : UnderlyingSignal {
  none = 0,
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
  resize = 1 << 15
};

class SignalMask {
 public:
  SignalMask(UnderlyingSignal);
  SignalMask operator|(const Signal& other);

 private:
  UnderlyingSignal mask_ =
      std::to_underlying(::renderer::view::ViewSignals::none);
};

}  // namespace detail::view

namespace view {

class View : public QLabel {
 public:
  View(QWidget* parent);

  void keyPressEvent(QKeyEvent* event) override;
  void keyReleaseEvent(QKeyEvent* event) override;
  void resizeEvent(QResizeEvent* event) override;
  void SetFrame(kernel::Frame frame);
  static ViewSignals KeyToSignal(int key);
  util::HotInput<kernel::Frame>* GetObserver();
  void Subscribe(util::Observer<ViewSignalData>* observable);

 private:
  static void SetSignal(ViewSignalData& data, ViewSignals signal);
  static void ClearSignal(ViewSignalData& data, ViewSignals signal);

  util::HotInput<kernel::Frame> observer_;
  util::ObservableData<ViewSignalData> observable_;
  ViewSignalData cur_signal_data_;
};

}  // namespace view

}  // namespace renderer
