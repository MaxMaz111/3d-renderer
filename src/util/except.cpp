#include "except.h"

#include <exception>
#include <spdlog/spdlog.h>

namespace renderer::util {

void React() noexcept {
  try {
    throw;
  } catch (std::exception& e) {
    spdlog::error("A known exception was caught, with a message: {}", e.what());
  } catch (...) {
    spdlog::error("An unknown exception was caught");
  }
}

}  // namespace renderer::util
