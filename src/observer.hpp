#pragma once

#include <cassert>
#include <functional>
#include <list>
#include <type_traits>

namespace renderer {

namespace observer {

template <class Data>
bool constexpr kIsArithmetic = std::is_arithmetic_v<Data>;

template <class Data>
bool constexpr kIsPointer = std::is_pointer_v<Data>;

template <class Data>
bool constexpr kIsEnum = std::is_enum_v<Data>;

template <class Data>
bool constexpr kIsSimpleClass =
    kIsArithmetic<Data> || kIsPointer<Data> || kIsEnum<Data>;

}  // namespace observer

struct ByValue;
struct ByReference;

namespace observer {

template <class Data, class SendBy>
struct DataSentByImpl;

template <class Data>
struct DataSentByImpl<Data, ByValue> {
  using Type = Data;
};

template <class Data>
struct DataSentByImpl<Data, ByReference> {
  using Type = const Data&;
};

template <class Data, class SendBy>
using DataSentBy = typename DataSentByImpl<Data, SendBy>::Type;

template <bool Flag>
struct AutoSendByImpl;

template <>
struct AutoSendByImpl<true> {
  using Type = ByValue;
};

template <>
struct AutoSendByImpl<false> {
  using Type = ByReference;
};

template <class Data>
using AutoSendBy =
    std::conditional_t<std::is_same_v<Data, void>, void,
                       typename AutoSendByImpl<kIsSimpleClass<Data>>::Type>;

}  // namespace observer

template <class Data, class SendBy>
class Observer;

template <class Data, class SendBy>
class Observable;

template <class Data, class SendBy = observer::AutoSendBy<Data>>
class Observer {
  using Observable = Observable<Data, SendBy>;

  using DataSentBy = observer::DataSentBy<Data, SendBy>;
  using Signature = void(DataSentBy);
  using Action = std::function<Signature>;

  friend Observable;

 public:
  using Arg = DataSentBy;

  template <class T1, class T2, class T3>
  Observer(T1&& on_subscribe, T2&& on_notify, T3&& on_unsubscribe)
      : on_subscribe_(std::forward<T1>(on_subscribe)),
        on_notify_(std::forward<T2>(on_notify)),
        on_unsubscribe_(std::forward<T3>(on_unsubscribe)) {
    assert(on_subscribe_);
    assert(on_notify_);
    assert(on_unsubscribe_);
  }

  Observer(const Observer&) = delete;
  Observer(Observer&&) noexcept = delete;
  Observer& operator=(const Observer&) = delete;
  Observer& operator=(Observer&&) noexcept = delete;

  ~Observer() { Unsubscribe(); }

  void Unsubscribe();

  bool IsSubscribed() const { return observable_ != nullptr; }

  bool HasData() const { return IsSubscribed(); }

  DataSentBy GetData() const;

  static void DoNothing(DataSentBy) {}

 private:
  void SetObservable(Observable* observable) {
    assert(observable);
    observable_ = observable;
  }

  Observable* observable_ = nullptr;
  Action on_subscribe_;
  Action on_notify_;
  Action on_unsubscribe_;
};

template <class Data, class SendBy = observer::AutoSendBy<Data>>
class Observable {
  using Observer = Observer<Data, SendBy>;
  using ObserversContainer = std::list<Observer*>;

  using DataSentBy = observer::DataSentBy<Data, SendBy>;
  using Signature = DataSentBy();
  using GetAction = std::function<Signature>;

  using Listeners = std::list<Observer*>;

  friend Observer;

 public:
  using Return = DataSentBy;

  template <class F>
  Observable(F&& data) : data_(std::forward<F>(data)) {
    assert(data_);
  }

  Observable(const Observable&) = delete;
  Observable(Observable&&) noexcept = delete;
  Observable& operator=(const Observable&) = delete;
  Observable& operator=(Observable&&) noexcept = delete;

  ~Observable() { UnsubscribeAll(); }

  void Notify() const {
    for (Observer* obs : listeners_) {
      obs->on_notify_(data_());
    }
  }

  void Subscribe(Observer* obs) {
    assert(obs);
    if (obs->IsSubscribed()) {
      obs->Unsubscribe();
    }
    listeners_.push_back(obs);
    obs->SetObservable(this);
    obs->on_subscribe_(data_());
  }

  void UnsubscribeAll() {
    while (!listeners_.empty()) {
      listeners_.front()->Unsubscribe();
    }
  }

  DataSentBy GetData() const { return data_(); }

 private:
  void Detach(Observer* obs) {
    assert(obs);
    obs->on_unsubscribe_(data_());
    listeners_.remove(obs);
  }

  GetAction data_;
  Listeners listeners_;
};

template <class Data, class SendBy>
void Observer<Data, SendBy>::Unsubscribe() {
  if (!IsSubscribed()) {
    return;
  }
  observable_->Detach(this);
  observable_ = nullptr;
}

template <class Data, class SendBy>
typename Observer<Data, SendBy>::DataSentBy Observer<Data, SendBy>::GetData()
    const {
  assert(observable_);
  return observable_->GetData();
}

namespace observer {

template <class Data>
class Storage {
 public:
  template <class... Args>
  explicit Storage(Args&&... args) : data_(std::forward<Args>(args)...) {}

 protected:
  template <class... Args>
  void Set(Args&&... args) {
    data_ = Data(std::forward<Args>(args)...);
  }

  Data data_{};
};

}  // namespace observer

template <class Data, class SendBy = observer::AutoSendBy<Data>>
class ObservableMono : protected Observable<Data, SendBy> {
  using Base = Observable<Data, SendBy>;

  using Observer = Observer<Data, SendBy>;

 public:
  using Base::Base;

  using Base::Notify;

  void Subscribe(Observer* obs) {
    Base::UnsubscribeAll();
    Base::Subscribe(obs);
  }
};

namespace observer {

template <class Data, class SendBy,
          template <class T1, class T2> class Observable>
class ObservableDataImpl : protected observer::Storage<Data>,
                           public Observable<Data, SendBy> {
  using StorageBase = observer::Storage<Data>;
  using ObservableBase = Observable<Data, SendBy>;

 public:
  template <class... Args>
  explicit ObservableDataImpl(Args&&... args)
      : StorageBase(std::forward<Args>(args)...),
        ObservableBase([&data = StorageBase::data_]() ->
                       typename ObservableBase::Return { return data; }) {}

  template <class... Args>
  void Set(Args&&... args) {
    StorageBase::Set(std::forward<Args>(args)...);
    ObservableBase::Notify();
  }
};

}  // namespace observer

template <class Data, class SendBy = observer::AutoSendBy<Data>>
using ObservableData = observer::ObservableDataImpl<Data, SendBy, Observable>;

template <class Data, class SendBy = observer::AutoSendBy<Data>>
using ObservableDataMono =
    observer::ObservableDataImpl<Data, SendBy, ObservableMono>;

template <class Data, class SendBy = observer::AutoSendBy<Data>>
class Input : public Observer<Data, SendBy> {
  using Base = Observer<Data, SendBy>;

 public:
  template <class T1, class T2>
  explicit Input(T1&& on_subscribe, T2&& on_notify)
      : Base(std::forward<T1>(on_subscribe), std::forward<T2>(on_notify),
             Base::DoNothing) {}
};

template <class Data, class SendBy = observer::AutoSendBy<Data>>
class HotInput : public Observer<Data, SendBy> {
  using Base = Observer<Data, SendBy>;

 public:
  template <class T>
  explicit HotInput(T&& action) : Base(action, action, Base::DoNothing) {}
};

template <class Data, class SendBy = observer::AutoSendBy<Data>>
class ColdInput : public Observer<Data, SendBy> {
  using Base = Observer<Data, SendBy>;

 public:
  template <class T>
  explicit ColdInput(T&& action)
      : Base(Base::DoNothing, std::forward<T>(action), Base::DoNothing) {}
};

}  // namespace renderer
