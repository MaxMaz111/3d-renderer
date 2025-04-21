#include <QApplication>
#include <QProcess>

#include "application.h"
#include "except.h"

int main(int argc, char* argv[]) {
  QProcess::execute("xset r rate 10 1000");
  QApplication runtime(argc, argv);
  try {
    renderer::Application app(argc, argv);
    return runtime.exec();
  } catch (...) {
    except::React();
    return 0;
  }
}
