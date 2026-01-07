#pragma once

#include "kernel/kernel.h"

#include "view/view.h"

namespace renderer {

namespace detail::controller {

class State {
  using KeyUnderlying = uint32_t;
  static_assert(std::is_unsigned_v<KeyUnderlying>);

 public:
  enum class Key : KeyUnderlying {
    None = 0,
    W = 1 << 0,
    A = 1 << 1,
    S = 1 << 2,
    D = 1 << 3,
    Q = 1 << 4,
    E = 1 << 5,
    B = 1 << 6,
    UpArrow = 1 << 7,
    LeftArrow = 1 << 8,
    DownArrow = 1 << 9,
    RightArrow = 1 << 10,
  };

  class KeyIterator {
   public:
    using value_type = Key;

    explicit KeyIterator(KeyUnderlying mask);

    value_type operator*() const;
    KeyIterator& operator++();
    bool operator==(std::default_sentinel_t) const;

   private:
    void advance();

    KeyUnderlying remaining_{0};
    KeyUnderlying current_{0};
  };

  class KeyRange {
   public:
    explicit KeyRange(KeyUnderlying mask);

    KeyIterator begin() const;

    std::default_sentinel_t end() const;

   private:
    KeyUnderlying mask_;
  };

  KeyRange HandlePressEvent(int key);
  KeyRange HandleReleaseEvent(int key);

 private:
  static Key ConvertQtKey(int key);

  KeyUnderlying key_mask_{};
};

}  // namespace detail::controller

namespace controller {

class Controller : public QObject {
  Q_OBJECT
 public:
  using State = detail::controller::State;
  Controller(kernel::Kernel* kernel_ptr, view::View* view_ptr);

 protected:
  bool eventFilter(QObject* obj, QEvent* event) override;

 private:
  void KeyPressEventHandler(const QKeyEvent*);
  void KeyReleaseEventHandler(const QKeyEvent*);
  void ResizeEventHandler(const QResizeEvent*) const;
  void NotifyKernel(State::KeyRange range) const;

  kernel::Kernel* kernel_ptr_;
  State keys_;
};

}  // namespace controller

}  // namespace renderer
