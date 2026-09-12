#pragma once
#include "Systems/InputCaptureProvider.hpp"

namespace ssg
{
class ImGuiCaptureProvider : public IInputCaptureProvider
{
  public:
    ImGuiCaptureProvider() = default;
    [[nodiscard]] bool WantsCaptureMouse() const override;
    [[nodiscard]] bool WantsCaptureKeyboard() const override;

  private:
};
} // namespace ssg