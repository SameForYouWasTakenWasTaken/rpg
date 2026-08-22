#pragma once

#include <SFML/Graphics.hpp>
#include <concepts>
#include <cstdint>

#include "Events/WindowCloseEvent.hpp"
#include "Events/WindowResizeEvent.hpp"
#include "Events/WindowSetView.hpp"
#include "SFML/Graphics/Color.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/View.hpp"
#include "Types.hpp"

namespace ssg
{
class WindowSetViewEvent;

struct WindowSettings
{
    using Size = uint32_t;
    using Fps = uint32_t;

    Size Width{100}, Height{100};
    Fps Framerate{60};
    String title{"Window"};
};

class Window
{
  public:
    Window(const WindowSettings& settings);
    ~Window();

    Window(const Window&) = delete;
    Window(Window&&) = delete;
    Window& operator=(const Window&) = delete;
    Window& operator=(Window&&) = delete;

    void SetSettings(const WindowSettings&);
    void SetTitle(const String&);
    void SetFramerate(WindowSettings::Fps);
    void SetSize(WindowSettings::Size, WindowSettings::Size);
    void SetView(const sf::View& view);

    template <typename... TArgs> void Draw(TArgs&&... args)
    {
        m_sfRenderWindow.draw(std::forward<TArgs>(args)...);
    }
    void Display() { m_sfRenderWindow.display(); }
    void Clear(sf::Color c = sf::Color::Black) { m_sfRenderWindow.clear(c); }

    void Close() { m_sfRenderWindow.close(); }

    bool IsOpen() { return m_sfRenderWindow.isOpen(); };

    void UpdateView(std::invocable<sf::View&> auto&& func);
    const sf::Window& GetSFMLWindow() const { return m_sfRenderWindow; }

    std::optional<sf::Event> PollSFMLEvents();

  private:
    void OnResize(const WindowResizeEvent& event);
    void OnClose(const WindowCloseEvent& event);
    void OnWindowSetView(const WindowSetViewEvent& event);
    sf::View m_View;
    sf::RenderWindow m_sfRenderWindow;
};
} // namespace ssg