#pragma once
#include "Rendering/Sinks/IRenderSink.hpp"
#include "Systems/Input/ImGuiCaptureProvider.hpp"
#include "Systems/InputGate.hpp"

namespace ssg::rendering
{
class ImGuiSink : public IRenderSink
{
  public:
    ImGuiSink(InputGate& gate, Window& window);

    void Begin() override;
    void HandleEvents(const sf::Event& e) override;
    void End(Window& window) override;

  private:
    Window& m_Window;
    sf::Clock m_Clock;
    ImGuiCaptureProvider m_Provider;
};
} // namespace ssg::rendering