#include <QApplication>

#include "application.h"
#include "except.h"

int main(int argc, char* argv[]) {
  // assert(false);
  QApplication runtime(argc, argv);
  try {
    renderer::Application app(argc, argv);
    return runtime.exec();
  } catch (...) {
    except::React();
    return 0;
  }
}
