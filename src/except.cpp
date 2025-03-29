#include "except.h"

#include <QDebug>
#include <exception>

namespace except {

void React() noexcept {
  try {
    throw;
  } catch (std::exception& e) {
    qDebug() << "A known exception was caught, with a message: " << e.what();
  } catch (...) {
    qDebug() << "An unknown exception was caught";
  }
}

}  // namespace except
