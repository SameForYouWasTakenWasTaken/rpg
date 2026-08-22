#pragma once
#include <SFML/Graphics.hpp>

#include "Events/EventBus.hpp"
#include "Events/KeyPressedEvent.hpp"
#include "Events/KeyReleasedEvent.hpp"
#include "Rendering/Window.hpp"
#include "Types.hpp"

namespace ssg
{

class Input
{
  public:
    using Key = sf::Keyboard::Key;
    using MouseButton = sf::Mouse::Button;

    Input(EventBus& bus);

    static bool IsKeyDown(Key key);
    static bool IsKeyUp(Key key);

    static bool IsMouseButtonDown(MouseButton button);
    static bool IsMouseButtonUp(MouseButton button);

    static Vec2 GetMousePosition();
    static Vec2 GetMousePosition(const Window& window);

    void Update(float dt);
    void ProcessEvents(std::optional<sf::Event> event);

  private:
    static constexpr float REPEAT_DELAY = 1.0f;
    static constexpr float REPEAT_INTERVAL = 0.1f;

    struct KeyState
    {
        bool held = false;
        float repeatTimer = 0.0f;
        float repeatDelay = REPEAT_DELAY;
        float repeatInterval = REPEAT_INTERVAL;
    };
    std::array<KeyState, sf::Keyboard::KeyCount> m_Keys;
    EventBus& m_EventBus;

    void OnKeyPress(const sf::Event::KeyPressed& event);
    void OnKeyRelease(const sf::Event::KeyReleased& event);
    void OnMouseButtonPress(const sf::Event::MouseButtonPressed& event) const;
    void OnMouseButtonRelease(const sf::Event::MouseButtonReleased& event) const;
    void OnMouseMove(const sf::Event::MouseMoved& event) const;
    void OnMouseWheelScroll(const sf::Event::MouseWheelScrolled& event) const;
};
} // namespace ssg