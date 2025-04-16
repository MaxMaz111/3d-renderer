#pragma once

#include <QLabel>
#include <QMouseEvent>

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
  void SetData(QPixmap pixmap);
  static ViewSignals KeyToSignal(int key);
  HotInput<QPixmap>* GetObserver();
  ObservableData<ViewSignalData>* GetObservable();

 private:
  HotInput<QPixmap> observer_;
  ObservableData<ViewSignalData> observable_;
  ViewSignalData cur_signal_data_;
};

}  // namespace renderer
