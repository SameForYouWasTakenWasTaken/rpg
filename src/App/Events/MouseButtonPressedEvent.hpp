#pragma once

#include <SFML/Window/Mouse.hpp>

#include "Events/IEvent.hpp"

namespace ssg
{

class MouseButtonPressedEvent : public IEvent
{
  public:
    sf::Mouse::Button button;
    float x;
    float y;

    MouseButtonPressedEvent(sf::Mouse::Button b, float x_, float y_) : button(b), x(x_), y(y_) {}

    GENERATE_EVENT_TYPE();
};

} // namespace ssg
