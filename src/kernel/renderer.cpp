#include "renderer.h"
#include "../constants.h"

namespace renderer {

Renderer::SentData Renderer::Render(const Scene& scene) {
  QPixmap data(constants::kWindowWidth, constants::kWindowHeight);
  data.fill(Qt::blue);
  return data;
}

}  // namespace renderer
