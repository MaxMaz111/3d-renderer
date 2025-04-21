#pragma once

#include <QLabel>
#include <QMouseEvent>

#include "../kernel/frame.h"
#include "../observer.hpp"
#include "view_signals.h"

namespace renderer {

class View : public QLabel {
 public:
  View() = delete;
  View(QWidget* parent);
  void keyPressEvent(QKeyEvent* event) override;
  void keyReleaseEvent(QKeyEvent* event) override;
  void resizeEvent(QResizeEvent* event) override;
  void SetFrame(Frame frame);
  static ViewSignals KeyToSignal(int key);
  HotInput<Frame>* GetObserver();
  void Subscribe(Observer<ViewSignalData>* observable);

 private:
  static void SetSignal(ViewSignalData& data, ViewSignals signal);

  static void ClearSignal(ViewSignalData& data, ViewSignals signal);

  HotInput<Frame> observer_;
  ObservableData<ViewSignalData> observable_;
  ViewSignalData cur_signal_data_;
};

}  // namespace renderer
