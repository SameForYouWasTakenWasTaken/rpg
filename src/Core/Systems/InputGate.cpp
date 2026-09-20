#include "InputGate.hpp"

#include <algorithm>

namespace ssg
{

void InputGate::RegisterProvider(const IInputCaptureProvider& provider)
{
    m_Providers.push_back(&provider);
}

bool InputGate::WantsKeyboard() const
{
    return std::ranges::all_of(m_Providers.begin(), m_Providers.end(), [](const auto& provider)
                               { return provider->WantsCaptureKeyboard(); });
}

bool InputGate::WantsMouse() const
{
    return std::ranges::all_of(m_Providers.begin(), m_Providers.end(),
                               [](const auto& provider) { return provider->WantsCaptureMouse(); });
}
} // namespace ssg