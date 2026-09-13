#pragma once
#include "InputCaptureProvider.hpp"
#include "Types.hpp"

namespace ssg
{
class InputGate
{
  public:
    void RegisterProvider(const IInputCaptureProvider& provider);

    [[nodiscard]] bool WantsKeyboard() const;
    [[nodiscard]] bool WantsMouse() const;

  private:
    Vector<const IInputCaptureProvider*> m_Providers;
};
} // namespace ssg