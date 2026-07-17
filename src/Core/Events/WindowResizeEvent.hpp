#pragma once

#include <cstdint>
#include "IEvent.hpp"

namespace ssg
{
class WindowResizeEvent : public IEvent
{

public:
    uint32_t Width;
    uint32_t Height;

    WindowResizeEvent(uint32_t w, uint32_t h)
    : Width(w), Height(h) {}

    GENERATE_EVENT_TYPE();
};
}