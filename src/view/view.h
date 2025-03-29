#pragma once

#include <QLabel>
#include <QMouseEvent>
#include "../observer.hpp"

namespace renderer {

class View : public QLabel {
  using ReceivedData = QPixmap;
  using Observer = Observer<ReceivedData>;

 public:
  View() = delete;
  View(QWidget* parent);

  void mouseMoveEvent(QMouseEvent* event) override;
  void mousePressEvent(QMouseEvent* event) override;
  void mouseReleaseEvent(QMouseEvent* event) override;
  void keyPressEvent(QKeyEvent* event) override;
  void focusInEvent(QFocusEvent* event) override;
  void focusOutEvent(QFocusEvent* event) override;

  void SetData(const ReceivedData& pixmap);

  Observer* GetObserver();

 private:
  Observer observer_;
};

}  // namespace renderer
