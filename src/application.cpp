#include "application.h"

#include <string>

#include "view/main_window.h"

namespace renderer {

Application::Application(int, char* argv[])
    : render_window_1_(Width{800}, Height{600}),
      render_window_2_(Width{800}, Height{600}),
      kernel_(std::string(argv[1])),
      controller_1_(&kernel_),
      controller_2_(&kernel_) {
  kernel_.Subscribe(render_window_1_.GetView()->GetObserver());
  render_window_1_.GetView()->Subscribe(controller_1_.GetObserver());
  kernel_.Subscribe(render_window_2_.GetView()->GetObserver());
  render_window_2_.GetView()->Subscribe(controller_2_.GetObserver());

  render_window_1_.Show();
  render_window_2_.Show();
}

}  // namespace renderer
