#pragma once

#include <type_traits>

namespace renderer::util {

template <typename T, class Tag>
  requires std::is_arithmetic_v<T>
class Alias {
 public:
  explicit constexpr Alias(T value) : value_(value) {}

  constexpr T operator()() const { return value_; }

 private:
  T value_;
};

}  // namespace renderer::util
