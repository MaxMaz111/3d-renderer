#include "except.h"
#include "application.h"

int main(int argc, char* argv[]) {
  try {
    renderer::Application app(argc, argv);
    return app.Run();
  } catch (...) {
    except::React();
    return 0;
  }
}
