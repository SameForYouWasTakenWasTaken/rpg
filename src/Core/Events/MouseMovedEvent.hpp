#pragma once

#include "Events/IEvent.hpp"

namespace ssg
{

class MouseMovedEvent : public IEvent
{
  public:
    float x;
    float y;

    MouseMovedEvent(float x_, float y_) : x(x_), y(y_) {}

    GENERATE_EVENT_TYPE();
};

} // namespace ssg
