#pragma once

#include <QLabel>
#include <QMouseEvent>

#include "../observer.hpp"
#include "view_signals.h"

namespace renderer {

class View : public QLabel {
  using Observer = Observer<QPixmap>;
  using Observable = Observable<ViewSignalData>;

 public:
  View() = delete;
  View(QWidget* parent);
  void keyPressEvent(QKeyEvent* event) override;
  void resizeEvent(QResizeEvent* event) override;
  void SetData(const QPixmap& pixmap);
  ViewSignalData KeyToSignal(int key) const;
  Observer* GetObserver();
  Observable* GetObservable();

 private:
  void SetAndNotify(ViewSignalData signal);

  static constexpr std::array<int, 8> kAvailableKeys = {
      Qt::Key_Left, Qt::Key_Up, Qt::Key_Right, Qt::Key_Down,
      Qt::Key_A,    Qt::Key_W,  Qt::Key_D,     Qt::Key_S};

  Observer observer_;
  Observable observable_;
  ViewSignalData cur_signal_data_;
};

}  // namespace renderer
