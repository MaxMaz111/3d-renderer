#include <QApplication>

#include "util/except.h"

#include "application.h"

int main(int argc, char* argv[]) {
  QApplication runtime(argc, argv);
  try {
    renderer::Application app(argc, argv);
    return runtime.exec();
  } catch (...) {
    renderer::util::React();
    return 0;
  }
}
