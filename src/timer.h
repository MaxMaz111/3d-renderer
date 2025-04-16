#pragma once

#include <chrono>
#include <string>

namespace renderer {

class Timer {
 public:
  Timer(const std::string& name = "Timer");
  ~Timer();
  double Elapsed() const;
  void Reset();

 private:
  std::chrono::time_point<std::chrono::high_resolution_clock> start_time_;
  std::string label_;
};

}  // namespace renderer
