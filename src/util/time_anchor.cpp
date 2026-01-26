#include "time_anchor.h"

#include <chrono>

namespace renderer::util {

TimeAnchor::TimeAnchor(
    std::string name,
    std::function<void(const std::string&, double)> on_destroy)
    : start_time_(Clock::now()),
      name_(std::move(name)),
      on_destroy_(std::move(on_destroy)) {}

TimeAnchor::~TimeAnchor() {
  if (on_destroy_) {
    on_destroy_(name_, Elapsed());
  }
}

TimeAnchor::TimeAnchor(TimeAnchor&& other) noexcept
    : start_time_(other.start_time_),
      name_(std::move(other.name_)),
      on_destroy_(std::move(other.on_destroy_)) {}

TimeAnchor& TimeAnchor::operator=(TimeAnchor&& other) noexcept {
  if (this != &other) {
    start_time_ = other.start_time_;
    name_ = std::move(other.name_);
    on_destroy_ = std::move(other.on_destroy_);
  }
  return *this;
}

double TimeAnchor::Elapsed() const {
  return std::chrono::duration<double, std::milli>(Clock::now() - start_time_)
      .count();
}

void TimeAnchor::Reset() {
  start_time_ = Clock::now();
}

}  // namespace renderer::util
