#pragma once

#include <type_traits>

namespace renderer::util {

template <typename T, class Tag>
class Alias {
 public:
  template <typename U>
    requires std::is_convertible_v<U, T>
  explicit constexpr Alias(U value) noexcept : value_(static_cast<T>(value)) {}

  constexpr operator T() const noexcept { return value_; }

 private:
  T value_;
};

}  // namespace renderer::util
