#pragma once

#include <numbers>

#include "util/size.h"

#include "linalg.h"
#include "view_point.h"

namespace renderer::kernel {

class Camera : public ViewPoint {
  using WidthT = Width;
  using HeightT = Height;

  static constexpr Scalar kMoveSpeed = 0.03;
  static constexpr Scalar kRotationSpeed = std::numbers::pi_v<Scalar> / 120;

 public:
  enum class RenderingMode { AllSolid, AllTransparent };

  Camera(WidthT width, HeightT height);

  void RotateLeft();
  void RotateRight();
  void RotateUp();
  void RotateDown();
  void MoveLeft();
  void MoveRight();
  void MoveForward();
  void MoveBackward();
  void SwivelLeft();
  void SwivelRight();
  void SwapRenderingMode();
  RenderingMode CurrentRenderingMode() const;

 private:
  RenderingMode mode_ = RenderingMode::AllSolid;
};

}  // namespace renderer::kernel
