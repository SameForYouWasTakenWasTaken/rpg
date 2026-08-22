#include "InputSystem.hpp"

#include "Events/KeyRepeatedEvent.hpp"
#include "Events/MouseButtonPressedEvent.hpp"
#include "Events/MouseButtonReleasedEvent.hpp"
#include "Events/MouseMovedEvent.hpp"
#include "Events/MouseWheelScrolledEvent.hpp"
#include "SFML/Window/Keyboard.hpp"

namespace ssg
{
bool Input::IsKeyDown(Key key) { return sf::Keyboard::isKeyPressed(key); }
bool Input::IsKeyUp(Key key) { return !sf::Keyboard::isKeyPressed(key); }

bool Input::IsMouseButtonDown(MouseButton button) { return sf::Mouse::isButtonPressed(button); }

bool Input::IsMouseButtonUp(MouseButton button) { return !sf::Mouse::isButtonPressed(button); }

Vec2 Input::GetMousePosition()
{
    return Vec2{sf::Mouse::getPosition().x, sf::Mouse::getPosition().y};
}

Vec2 Input::GetMousePosition(const Window& window)
{
    return Vec2{sf::Mouse::getPosition(window.GetSFMLWindow()).x,
                sf::Mouse::getPosition(window.GetSFMLWindow()).y};
}
Input::Input(EventBus& bus) : m_EventBus(bus) {}

void Input::Update(float dt)
{
    for (std::size_t i = 0; i < m_Keys.size(); ++i)
    {
        auto& state = m_Keys[i];

        if (!state.held)
            continue;

        state.repeatTimer += dt;

        if (state.repeatTimer < state.repeatDelay)
            continue;

        state.repeatTimer -= state.repeatInterval;

        m_EventBus.Queue<KeyRepeatedEvent>(static_cast<Key>(i));
    }
}
void Input::ProcessEvents(std::optional<sf::Event> event)
{
    if (auto pEvent = EventBus::IsSFMLEvent<sf::Event::KeyPressed>(event))
        OnKeyPress(*pEvent);

    if (auto pEvent = EventBus::IsSFMLEvent<sf::Event::KeyReleased>(event))
        OnKeyRelease(*pEvent);

    if (auto pEvent = EventBus::IsSFMLEvent<sf::Event::MouseButtonPressed>(event))
        OnMouseButtonPress(*pEvent);

    if (auto pEvent = EventBus::IsSFMLEvent<sf::Event::MouseButtonReleased>(event))
        OnMouseButtonRelease(*pEvent);

    if (auto pEvent = EventBus::IsSFMLEvent<sf::Event::MouseMoved>(event))
        OnMouseMove(*pEvent);

    if (auto pEvent = EventBus::IsSFMLEvent<sf::Event::MouseWheelScrolled>(event))
        OnMouseWheelScroll(*pEvent);
}

void Input::OnKeyPress(const sf::Event::KeyPressed& event)
{
    auto& state = m_Keys[static_cast<std::size_t>(event.code)];

    // Ignore repeated KeyPressed events.
    if (state.held)
        return;

    state.held = true;
    state.repeatTimer = 0.0f;

    m_EventBus.Queue<KeyPressedEvent>(event.code, event.alt, event.control, event.shift,
                                      event.system);
}

void Input::OnKeyRelease(const sf::Event::KeyReleased& event)
{
    auto& state = m_Keys[static_cast<std::size_t>(event.code)];

    state.held = false;
    state.repeatTimer = 0.0f;

    m_EventBus.Queue<KeyReleasedEvent>(event.code, event.alt, event.control, event.shift,
                                       event.system);
}

void Input::OnMouseButtonPress(const sf::Event::MouseButtonPressed& event) const
{
    m_EventBus.Queue<MouseButtonPressedEvent>(event.button, static_cast<float>(event.position.x),
                                              static_cast<float>(event.position.y));
}

void Input::OnMouseButtonRelease(const sf::Event::MouseButtonReleased& event) const
{
    m_EventBus.Queue<MouseButtonReleasedEvent>(event.button, static_cast<float>(event.position.x),
                                               static_cast<float>(event.position.y));
}

void Input::OnMouseMove(const sf::Event::MouseMoved& event) const
{
    m_EventBus.Queue<MouseMovedEvent>(static_cast<float>(event.position.x),
                                      static_cast<float>(event.position.y));
}

void Input::OnMouseWheelScroll(const sf::Event::MouseWheelScrolled& event) const
{
    m_EventBus.Queue<MouseWheelScrolledEvent>(event.wheel, event.delta,
                                              static_cast<float>(event.position.x),
                                              static_cast<float>(event.position.y));
}
} // namespace ssg