#include "application.h"

#include <string>

#include "view/main_window.h"

namespace renderer {

Application::Application(int, char* argv[])
    : kernel_(std::string(argv[1])),
      render_window_(Width{800}, Height{600}),
      controller_(&kernel_) {
  kernel_.Subscribe(render_window_.GetView()->GetObserver());
  render_window_.GetView()->Subscribe(controller_.GetObserver());
  render_window_.Show();
}

}  // namespace renderer
