#include "Window.hpp"

#include <optional>

#include "Engine.hpp"
#include "Events/WindowResizeEvent.hpp"
#include "Events/WindowSetView.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/Window/VideoMode.hpp"

namespace ssg
{
Window::Window(const WindowSettings& settings)
{
    m_sfRenderWindow.create(sf::VideoMode{{settings.Width, settings.Height}}, settings.title);

    SetSettings(settings);

    auto& eventBus = Engine::instance().eventBus;
    eventBus.Sink<WindowResizeEvent>().connect<&Window::OnResize>(this);
    eventBus.Sink<WindowCloseEvent>().connect<&Window::OnClose>(this);
}
Window::~Window()
{
    if (IsOpen())
        Close();
}

void Window::SetSettings(const WindowSettings& settings)
{
    SetSize(settings.Width, settings.Height);
    SetTitle(settings.title);
    SetFramerate(settings.Framerate);
}

void Window::SetTitle(const String& title) { m_sfRenderWindow.setTitle(title); }

void Window::SetFramerate(WindowSettings::Fps fps) { m_sfRenderWindow.setFramerateLimit(fps); }

void Window::SetSize(WindowSettings::Size width, WindowSettings::Size height)
{
    m_sfRenderWindow.setSize(sf::Vector2u{height, width});
    UpdateView([width, height](sf::View& view)
               { view.setSize({static_cast<float>(width), static_cast<float>(height)}); });
}

void Window::SetView(const sf::View& view) { m_sfRenderWindow.setView(view); }

std::optional<sf::Event> Window::PollSFMLEvents() { return m_sfRenderWindow.pollEvent(); }

void Window::OnResize(const WindowResizeEvent& event)
{
    UpdateView(
        [&event](sf::View& view)
        { view.setSize({static_cast<float>(event.Width), static_cast<float>(event.Height)}); });
}
void Window::OnClose(const WindowCloseEvent& event) { Close(); }

void Window::OnWindowSetView(const WindowSetViewEvent& event)
{
    if (event.window == this)
        SetView(event.view);
}

void Window::UpdateView(std::invocable<sf::View&> auto&& func)
{
    func(m_View);
    SetView(m_View);
}

} // namespace ssg