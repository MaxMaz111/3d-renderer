#pragma once

#include <type_traits>

namespace renderer::util {

template <typename T, class Tag>
  requires std::is_arithmetic_v<T>
class Alias {
 public:
  explicit constexpr Alias(T value) noexcept : value_(value) {}

  template <typename U>
    requires std::is_arithmetic_v<U> && std::is_convertible_v<U, T>
  explicit constexpr Alias(U value) noexcept : value_(static_cast<T>(value)) {}

  constexpr T operator()() const noexcept { return value_; }

 private:
  T value_;
};

}  // namespace renderer::util
