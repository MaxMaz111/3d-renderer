#include "application.h"

#include "view/main_window.h"

namespace renderer {

Application::Application(int argc, char* argv[])
    : render_window_(Width{800}, Height{600}),
      kernel_(argv[1]),
      controller_(&kernel_) {
  kernel_.Subscribe(render_window_.GetView()->GetObserver());
  render_window_.GetView()->GetObservable()->Subscribe(
      controller_.GetObserver());
}

}  // namespace renderer
