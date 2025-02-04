#include "except.h"

#include <exception>
#include <iostream>

namespace except {
void React() {
  try {
    throw;
  } catch (std::exception& e) {
    std::cerr << "A known exception was caught, with a message: " << e.what()
              << std::endl;
  } catch (...) {
    std::cerr << "An unknown exception was caught" << std::endl;
    throw;
  }
}
}  // namespace except
