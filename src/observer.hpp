#pragma once

#include <cassert>
#include <functional>
#include <list>

namespace renderer {

template <typename T>
class Observable;

template <typename T>
class Observer {
  using Observable = Observable<T>;
  friend Observable;

 public:
  template <class T1, class T2, class T3>
  Observer(T1&& t1, T2&& t2, T3&& t3)
      : on_notify_(std::forward<T1>(t1)),
        on_subscribe_(std::forward<T2>(t2)),
        on_unsubscribe_(std::forward<T3>(t3)) {}

  Observer(const Observer&) = delete;
  Observer(Observer&&) noexcept = delete;
  Observer& operator=(const Observer&) = delete;
  Observer& operator=(Observer&&) noexcept = delete;

  ~Observer() { Unsubscribe(); }

  void Unsubscribe() {
    if (!IsSubscribed()) {
      return;
    }
    observable_->Detach(this);
    observable_ = nullptr;
  }

  bool IsSubscribed() const { return observable_; }

  static void DoNothing(T) {}

 private:
  void SetObservable(Observable* observable) { observable_ = observable; }

  Observable* observable_ = nullptr;

  std::function<void(T)> on_notify_;
  std::function<void(T)> on_subscribe_;
  std::function<void(T)> on_unsubscribe_;
};

template <typename T>
class Observable {
  using Observer = Observer<T>;
  friend Observer;

 public:
  template <class T1>
  Observable(T1&& data) : data_(std::forward<T1>(data)) {}

  Observable(const Observable&) = delete;
  Observable(Observable&&) noexcept = delete;
  Observable& operator=(const Observable&) = delete;
  Observable& operator=(Observable&&) noexcept = delete;

  ~Observable() {
    while (!observers_.empty()) {
      assert(observers_.front());
      observers_.front()->Unsubscribe();
    }
  }

  void Subscribe(Observer* observer) {
    assert(observer);
    if (observer->IsSubscribed()) {
      observer->Unsubscribe();
    }
    observers_.push_back(observer);
    observer->SetObservable(this);
    observer->on_subscribe_(data_());
  }

  void Notify() const {
    for (Observer* observer : observers_) {
      assert(observer);
      observer->on_notify_(data_());
    }
  }

 private:
  void Detach(Observer* observer) {
    assert(observer);
    observer->on_unsubscribe_(data_());
    observers_.remove(observer);
  }

  std::function<T()> data_;
  std::list<Observer*> observers_;
};

}  // namespace renderer
