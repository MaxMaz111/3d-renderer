#include "time_anchor.h"

#include <QDebug>

namespace renderer::util {

TimeAnchor::TimeAnchor(const std::string& name) : label_(name) {
  start_time_ = std::chrono::high_resolution_clock::now();
}

double TimeAnchor::Elapsed() const {
  auto current_time = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
      current_time - start_time_);
  return duration.count() / 1000.0;
}

void TimeAnchor::Reset() {
  start_time_ = std::chrono::high_resolution_clock::now();
}


TimeAnchor::~TimeAnchor() {
  auto end_time = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
      end_time - start_time_);
  qDebug() << "FPS: " << 1000000.0 / duration.count();
}

}  // namespace renderer::util
