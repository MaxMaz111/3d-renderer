#pragma once

#include <chrono>
#include <functional>
#include <string>

namespace renderer::util {

class TimeAnchor {
 public:
  using Clock = std::chrono::high_resolution_clock;
  using TimePoint = Clock::time_point;
  using Callback = std::function<void(const std::string&, double)>;

  explicit TimeAnchor(
      std::string name = "Timer",
      std::function<void(const std::string&, double)> on_destroy = nullptr);
  ~TimeAnchor();

  TimeAnchor(const TimeAnchor&) = delete;
  TimeAnchor& operator=(const TimeAnchor&) = delete;
  TimeAnchor(TimeAnchor&&) noexcept;
  TimeAnchor& operator=(TimeAnchor&&) noexcept;

  double Elapsed() const;
  void Reset();

 private:
  TimePoint start_time_;
  std::string name_;
  Callback on_destroy_;
};

}  // namespace renderer::util
