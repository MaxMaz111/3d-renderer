#include <QApplication>
#include <spdlog/spdlog.h>

#include "util/except.h"

#include "application.h"

int main(int argc, char* argv[]) {
  if (argc < 2) {
    spdlog::error("Usage: {} <path_to_obj_file>", argv[0]);
    return 1;
  }
  QApplication runtime(argc, argv);
  try {
    renderer::Application app(argc, argv);
    return runtime.exec();
  } catch (...) {
    renderer::util::React();
    return 0;
  }
}
