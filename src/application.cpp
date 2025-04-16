#include "application.h"

#include <string>

#include "view/main_window.h"

namespace renderer {

Application::Application(int, char*[])
    : render_window_(Width{800}, Height{600}),
      kernel_(std::string("../obj/car.obj")),
      controller_(&kernel_) {
  kernel_.Subscribe(render_window_.GetView()->GetObserver());
  render_window_.GetView()->GetObservable()->Subscribe(
      controller_.GetObserver());
}

}  // namespace renderer
