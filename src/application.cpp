#include "application.h"

namespace renderer {

Application::Application() : controller_(&kernel_) {
  kernel_.Subscribe(render_window_.GetView()->GetObserver());
}

}  // namespace renderer
