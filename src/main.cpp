#include <QApplication>

#include "application.h"
#include "except.h"

int main(int argc, char* argv[]) {
  QApplication runtime(argc, argv);
  try {
    renderer::Application app;
    return runtime.exec();
  } catch (...) {
    except::React();
    return 0;
  }
}
