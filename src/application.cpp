#include "application.h"

namespace renderer {

Application::Application(int, char* argv[])
    : kernel_(argv[1]), controller_(&kernel_, render_window_.View()->Label()) {
  kernel_.Subscribe(render_window_.View()->KernelPort());
  render_window_.Show();
}

}  // namespace renderer
