#pragma once

#include <SFML/Window/Mouse.hpp>

#include "Events/IEvent.hpp"

namespace ssg
{

class MouseWheelScrolledEvent : public IEvent
{
  public:
    sf::Mouse::Wheel wheel;
    float delta;
    float x;
    float y;

    MouseWheelScrolledEvent(sf::Mouse::Wheel w, float d, float x_, float y_)
        : wheel(w), delta(d), x(x_), y(y_)
    {
    }

    GENERATE_EVENT_TYPE();
};

} // namespace ssg
