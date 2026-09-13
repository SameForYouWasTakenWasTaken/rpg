#include "ImGuiSink.hpp"

#include "glad/gl.h"
#include "imgui-SFML.h"

namespace ssg::rendering
{

ImGuiSink::ImGuiSink(InputGate& gate, Window& window) : m_Window(window)
{
    if (!gladLoaderLoadGL())
        throw std::runtime_error("Couldn't initialize glad!");

    if (!ImGui::SFML::Init(window.GetSFMLWindow()))
        throw std::runtime_error("Failed to initialize ImGUI");

    gate.RegisterProvider(m_Provider);
}
void ImGuiSink::Begin()
{
    sf::Time deltaTime = m_Clock.restart();
    ImGui::SFML::Update(m_Window.GetSFMLWindow(), deltaTime);
}
void ImGuiSink::HandleEvents(const sf::Event& e)
{
    ImGui::SFML::ProcessEvent(m_Window.GetSFMLWindow(), e);
}
void ImGuiSink::End(Window& window) { ImGui::SFML::Render(window.GetSFMLWindow()); }
} // namespace ssg::rendering