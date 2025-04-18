#include "timer.h"

#include <iostream>

namespace renderer {

Timer::Timer(const std::string& name) : label_(name) {
  system("clear");
  start_time_ = std::chrono::high_resolution_clock::now();
  std::cout << label_ << " started" << std::endl;
}

Timer::~Timer() {
  auto end_time = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
      end_time - start_time_);
  std::cout << label_ << " ended - Elapsed time: " << duration.count() / 1000.0
            << " ms" << std::endl;
  std::cout << "FPS: " << 1000000.0 / duration.count() << std::endl;
}

double Timer::Elapsed() const {
  auto current_time = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
      current_time - start_time_);
  return duration.count() / 1000.0;
}

void Timer::Reset() {
  start_time_ = std::chrono::high_resolution_clock::now();
  std::cout << label_ << " reset" << std::endl;
}

}  // namespace renderer
