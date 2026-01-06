#pragma once

#include <chrono>
#include <string>

namespace renderer::util {

class TimeAnchor {
 public:
  TimeAnchor(const std::string& name = "Timer");

  double Elapsed() const;
  void Reset();
  ~TimeAnchor();

 private:
  std::chrono::time_point<std::chrono::high_resolution_clock> start_time_;
  std::string label_;
};

}  // namespace renderer::util
