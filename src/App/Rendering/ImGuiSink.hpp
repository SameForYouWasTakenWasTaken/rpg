#pragma once
#include "Rendering/Sinks/IRenderSink.hpp"

namespace ssg::rendering
{
class ImGuiSink : public IRenderSink
{
  public:
    ImGuiSink(Window& window);

    void Begin() override;
    void HandleEvents(const sf::Event& e) override;
    void End(Window& window) override;

  private:
    Window& m_Window;
    sf::Clock m_Clock;
};
} // namespace ssg::rendering